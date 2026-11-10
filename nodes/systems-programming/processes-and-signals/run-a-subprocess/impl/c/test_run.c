// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

#include "run.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    char *argv[10];
    int status;
    const char *out;
    const char *err;
};

static const struct test_case cases[] = {
    {"hello", {"printf", "hello", nullptr}, 0, "hello", ""},
    {"two-arguments", {"echo", "one", "two", nullptr}, 0, "one two\n", ""},
    {"exit-status", {"sh", "-c", "exit 3", nullptr}, 3, "", ""},
    {"both-streams", {"sh", "-c", "echo out; echo err >&2; exit 2", nullptr}, 2, "out\n", "err\n"},
    {"metacharacters",
     {"printf", "%s|", "a b", "$HOME", "*", "x;y", "\"q\"", "'s'", nullptr},
     0,
     "a b|$HOME|*|x;y|\"q\"|'s'|",
     ""},
    {"empty-argument", {"printf", "[%s]", "", nullptr}, 0, "[]", ""},
    {"newline-argument", {"printf", "%s", "line one\nline two", nullptr}, 0, "line one\nline two", ""},
    {"trailing-newlines", {"printf", "a\\n\\n", nullptr}, 0, "a\n\n", ""},
    {"non-ascii", {"printf", "%s\\n", "xin chào, 日本語, 🙂", nullptr}, 0, "xin chào, 日本語, 🙂\n", ""},
    {"stderr-only", {"sh", "-c", "echo warning >&2", nullptr}, 0, "", "warning\n"},
    {"no-output", {"true", nullptr}, 0, "", ""},
    {"false", {"false", nullptr}, 1, "", ""},
    {"status-255", {"sh", "-c", "exit 255", nullptr}, 255, "", ""},
    {"empty-stdin", {"sh", "-c", "if read -r line; then echo got; else echo eof; fi", nullptr}, 0, "eof\n", ""},
};

static int same(const char *data, size_t len, const char *want) {
    return len == strlen(want) && memcmp(data, want, len) == 0;
}

static int all_bytes(const char *data, size_t len, char c, size_t want) {
    if (len != want) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (data[i] != c) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        struct run_result r;
        if (run(c->argv, &r) != 0) {
            printf("FAIL %s: %s\n", c->name, strerror(errno));
            failures++;
            continue;
        }
        if (r.status != c->status || r.signal != 0 || !same(r.out, r.out_len, c->out) ||
            !same(r.err, r.err_len, c->err)) {
            printf("FAIL %s: status %d, out [%s], err [%s]\n", c->name, r.status, r.out, r.err);
            failures++;
        }
        run_result_free(&r);
    }

    char *large[] = {"sh", "-c",
                     "head -c 1048576 /dev/zero | tr '\\0' x; head -c 1048576 /dev/zero | tr '\\0' y >&2",
                     nullptr};
    struct run_result r;
    if (run(large, &r) != 0) {
        printf("FAIL large-output: %s\n", strerror(errno));
        failures++;
    } else {
        if (r.status != 0 || !all_bytes(r.out, r.out_len, 'x', 1048576) ||
            !all_bytes(r.err, r.err_len, 'y', 1048576)) {
            printf("FAIL large-output: %zu and %zu bytes\n", r.out_len, r.err_len);
            failures++;
        }
        run_result_free(&r);
    }

    char *missing[] = {"no-such-program-7f3a", nullptr};
    if (run(missing, &r) == 0) {
        // POSIX allows posix_spawnp to succeed and the child to exit with 127.
        if (r.status != 127) {
            printf("FAIL missing-program: status %d\n", r.status);
            failures++;
        }
        run_result_free(&r);
    } else if (errno != ENOENT) {
        printf("FAIL missing-program: %s\n", strerror(errno));
        failures++;
    }

    char *killed[] = {"sh", "-c", "kill -9 $$", nullptr};
    if (run(killed, &r) != 0 || r.status != -1 || r.signal != 9) {
        printf("FAIL signal\n");
        failures++;
    } else {
        run_result_free(&r);
    }

    if (failures > 0) {
        return 1;
    }
    puts("all cases pass");
    return 0;
}
