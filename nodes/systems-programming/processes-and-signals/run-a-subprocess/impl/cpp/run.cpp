// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

#include "run.hpp"

#include <array>
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <spawn.h>
#include <stdexcept>
#include <sys/wait.h>
#include <system_error>
#include <unistd.h>

extern char **environ;

namespace {

[[noreturn]] void fail(int code, const char *what) {
    throw std::system_error(code, std::generic_category(), what);
}

// Closes a file descriptor when it goes out of scope.
class Fd {
  public:
    explicit Fd(int fd = -1) : fd_(fd) {}
    Fd(const Fd &) = delete;
    Fd &operator=(const Fd &) = delete;
    ~Fd() { reset(); }
    int get() const { return fd_; }
    void reset(int fd = -1) {
        if (fd_ >= 0) {
            close(fd_);
        }
        fd_ = fd;
    }

  private:
    int fd_;
};

void make_pipe(Fd &read_end, Fd &write_end) {
    int fds[2];
    if (pipe(fds) != 0) {
        fail(errno, "pipe");
    }
    read_end.reset(fds[0]);
    write_end.reset(fds[1]);
    // The pipes must not leak into other children.
    fcntl(fds[0], F_SETFD, FD_CLOEXEC);
    fcntl(fds[1], F_SETFD, FD_CLOEXEC);
}

} // namespace

RunResult run(const std::vector<std::string> &argv) {
    // posix_spawnp takes an array of char pointers that ends with a null pointer.
    std::vector<char *> args;
    for (const auto &arg : argv) {
        args.push_back(const_cast<char *>(arg.c_str()));
    }
    args.push_back(nullptr);

    Fd out_read, out_write, err_read, err_write;
    make_pipe(out_read, out_write);
    make_pipe(err_read, err_write);

    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_addopen(&actions, 0, "/dev/null", O_RDONLY, 0);
    // dup2 clears FD_CLOEXEC on the new descriptor, so 1 and 2 stay open.
    posix_spawn_file_actions_adddup2(&actions, out_write.get(), 1);
    posix_spawn_file_actions_adddup2(&actions, err_write.get(), 2);
    pid_t pid;
    int rc = posix_spawnp(&pid, args[0], &actions, nullptr, args.data(), environ);
    posix_spawn_file_actions_destroy(&actions);
    out_write.reset();
    err_write.reset();
    if (rc != 0) {
        fail(rc, args[0]);
    }

    // Read the two pipes as data comes. If this loop read one pipe to its
    // end first, a child that fills the other pipe would block for ever.
    RunResult result{0, {}, {}};
    std::array<std::string *, 2> sinks{&result.out, &result.err};
    std::array<pollfd, 2> fds{pollfd{out_read.get(), POLLIN, 0}, pollfd{err_read.get(), POLLIN, 0}};
    std::array<char, 65536> chunk;
    int failed = 0;
    for (int open_fds = 2; open_fds > 0 && failed == 0;) {
        if (poll(fds.data(), fds.size(), -1) < 0) {
            if (errno != EINTR) {
                failed = errno;
            }
            continue;
        }
        for (std::size_t i = 0; i < fds.size(); i++) {
            if (fds[i].fd < 0 || fds[i].revents == 0) {
                continue;
            }
            ssize_t n = read(fds[i].fd, chunk.data(), chunk.size());
            if (n > 0) {
                sinks[i]->append(chunk.data(), static_cast<std::size_t>(n));
            } else if (n == 0) {
                fds[i].fd = -1; // a negative fd makes poll skip the entry
                open_fds--;
            } else if (errno != EINTR) {
                failed = errno;
            }
        }
    }
    out_read.reset();
    err_read.reset();

    int wstatus;
    while (waitpid(pid, &wstatus, 0) < 0) {
        if (errno != EINTR) {
            fail(errno, "waitpid");
        }
    }
    if (failed != 0) {
        fail(failed, "read");
    }
    if (!WIFEXITED(wstatus)) {
        throw std::runtime_error(argv[0] + " was stopped by signal " + std::to_string(WTERMSIG(wstatus)));
    }
    result.status = WEXITSTATUS(wstatus);
    return result;
}
