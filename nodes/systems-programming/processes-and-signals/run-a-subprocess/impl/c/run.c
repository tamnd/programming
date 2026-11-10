// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

#define _POSIX_C_SOURCE 200809L

#include "run.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <spawn.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

struct buffer {
    char *data;
    size_t len, cap;
};

// append reads once from fd into b. It returns 1 after data, 0 at end of
// file, and -1 on an error.
static int append(int fd, struct buffer *b) {
    if (b->cap - b->len < 4096) {
        size_t cap = b->cap ? b->cap * 2 : 8192;
        char *data = realloc(b->data, cap);
        if (data == nullptr) {
            return -1;
        }
        b->data = data;
        b->cap = cap;
    }
    // Keep one byte for the final NUL.
    ssize_t n = read(fd, b->data + b->len, b->cap - b->len - 1);
    if (n < 0) {
        return errno == EINTR ? 1 : -1;
    }
    b->len += (size_t)n;
    return n > 0;
}

static int pipe_cloexec(int fds[2]) {
    if (pipe(fds) != 0) {
        return -1;
    }
    // The pipes must not leak into other children.
    fcntl(fds[0], F_SETFD, FD_CLOEXEC);
    fcntl(fds[1], F_SETFD, FD_CLOEXEC);
    return 0;
}

int run(char *const argv[], struct run_result *result) {
    int out[2], err[2];
    if (pipe_cloexec(out) != 0) {
        return -1;
    }
    if (pipe_cloexec(err) != 0) {
        int saved = errno;
        close(out[0]);
        close(out[1]);
        errno = saved;
        return -1;
    }

    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_addopen(&actions, 0, "/dev/null", O_RDONLY, 0);
    // dup2 clears FD_CLOEXEC on the new descriptor, so 1 and 2 stay open.
    posix_spawn_file_actions_adddup2(&actions, out[1], 1);
    posix_spawn_file_actions_adddup2(&actions, err[1], 2);
    pid_t pid;
    int rc = posix_spawnp(&pid, argv[0], &actions, nullptr, argv, environ);
    posix_spawn_file_actions_destroy(&actions);
    close(out[1]);
    close(err[1]);
    if (rc != 0) {
        close(out[0]);
        close(err[0]);
        errno = rc;
        return -1;
    }

    // Read the two pipes as data comes. If this loop read one pipe to its
    // end first, a child that fills the other pipe would block for ever.
    struct buffer bufs[2] = {0};
    struct pollfd fds[2] = {{.fd = out[0], .events = POLLIN}, {.fd = err[0], .events = POLLIN}};
    int open_fds = 2, failed = 0;
    while (open_fds > 0 && !failed) {
        if (poll(fds, 2, -1) < 0) {
            if (errno == EINTR) {
                continue;
            }
            failed = errno;
            break;
        }
        for (int i = 0; i < 2; i++) {
            if (fds[i].fd < 0 || fds[i].revents == 0) {
                continue;
            }
            int got = append(fds[i].fd, &bufs[i]);
            if (got < 0) {
                failed = errno ? errno : ENOMEM;
            } else if (got == 0) {
                close(fds[i].fd);
                fds[i].fd = -1;
                open_fds--;
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        if (fds[i].fd >= 0) {
            close(fds[i].fd);
        }
    }

    int wstatus;
    while (waitpid(pid, &wstatus, 0) < 0) {
        if (errno != EINTR) {
            failed = failed ? failed : errno;
            break;
        }
    }
    if (!failed) {
        for (int i = 0; i < 2; i++) {
            if (bufs[i].data == nullptr && (bufs[i].data = malloc(1)) == nullptr) {
                failed = ENOMEM;
                break;
            }
            bufs[i].data[bufs[i].len] = '\0';
        }
    }
    if (failed) {
        free(bufs[0].data);
        free(bufs[1].data);
        errno = failed;
        return -1;
    }

    *result = (struct run_result){
        .status = WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : -1,
        .signal = WIFSIGNALED(wstatus) ? WTERMSIG(wstatus) : 0,
        .out = bufs[0].data,
        .out_len = bufs[0].len,
        .err = bufs[1].data,
        .err_len = bufs[1].len,
    };
    return 0;
}

void run_result_free(struct run_result *result) {
    free(result->out);
    free(result->err);
    result->out = nullptr;
    result->err = nullptr;
}
