```c run id=poll
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int requests[2], replies[2];
static int done;

static void on_request(int fd)
{
    char buf[64];
    ssize_t n = read(fd, buf, sizeof buf - 1);
    buf[n] = '\0';
    printf("request: %s\n", buf);
    write(replies[1], "ok", 2);
}

static void on_reply(int fd)
{
    char buf[64];
    ssize_t n = read(fd, buf, sizeof buf - 1);
    buf[n] = '\0';
    printf("reply: %s\n", buf);
    done = 1;
}

int main(void)
{
    if (pipe(requests) != 0 || pipe(replies) != 0)
        return 1;
    struct pollfd fds[2] = { { requests[0], POLLIN, 0 }, { replies[0], POLLIN, 0 } };
    void (*handlers[2])(int) = { on_request, on_reply };

    write(requests[1], "GET /", 5);
    while (!done) {
        if (poll(fds, 2, 1000) <= 0)
            return 1;
        for (int i = 0; i < 2; i++)
            if (fds[i].revents & POLLIN)
                handlers[i](fds[i].fd);
    }
    return 0;
}
```

```output for=poll
request: GET /
reply: ok
```

- Verdict: idiom. ISO C has no events. On POSIX systems, `poll` and `select` wait until one of several file descriptors is ready, and an array of function pointers maps each descriptor to its handler. Linux has `epoll`, the BSDs and macOS have `kqueue`, and Windows has I/O completion ports. Libraries such as libuv and libevent wrap them.
