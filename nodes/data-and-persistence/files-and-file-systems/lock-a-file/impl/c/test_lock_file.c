// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

#define _POSIX_C_SOURCE 200809L

#include "lock_file.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static const char *path = "lock_tree/data.txt";
static int failures = 0;

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf lock_tree && mkdir lock_tree && printf 'alpha\\n' > lock_tree/data.txt") != 0) exit(2);
}

int main(void) {
    static const struct {
        const char *name;
        enum lock_mode held, want;
        int expect;
    } cases[] = {
        {"no-lock-exclusive", LOCK_MODE_NONE, LOCK_MODE_EXCLUSIVE, 1},
        {"no-lock-shared", LOCK_MODE_NONE, LOCK_MODE_SHARED, 1},
        {"exclusive-with-exclusive", LOCK_MODE_EXCLUSIVE, LOCK_MODE_EXCLUSIVE, 0},
        {"shared-with-shared", LOCK_MODE_SHARED, LOCK_MODE_SHARED, 1},
        {"shared-with-exclusive", LOCK_MODE_SHARED, LOCK_MODE_EXCLUSIVE, 0},
        {"exclusive-with-shared", LOCK_MODE_EXCLUSIVE, LOCK_MODE_SHARED, 0},
    };
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        fresh();
        expect(try_lock(path, cases[i].held, cases[i].want) == cases[i].expect, cases[i].name);
    }

    // A release frees the lock.
    fresh();
    int a = try_lock_file(path, LOCK_MODE_EXCLUSIVE);
    expect(a >= 0, "first lock");
    errno = 0;
    expect(try_lock_file(path, LOCK_MODE_EXCLUSIVE) == -1 && errno == EWOULDBLOCK, "second lock is busy");
    expect(unlock_file(a) == 0, "unlock");
    int b = try_lock_file(path, LOCK_MODE_EXCLUSIVE);
    expect(b >= 0, "lock after the unlock");

    // A child process sees the lock.
    pid_t pid = fork();
    if (pid == 0) _exit(try_lock_file(path, LOCK_MODE_SHARED) == -1 && errno == EWOULDBLOCK ? 0 : 1);
    int status = 0;
    waitpid(pid, &status, 0);
    expect(WIFEXITED(status) && WEXITSTATUS(status) == 0, "child process sees the lock");
    unlock_file(b);

    errno = 0;
    expect(try_lock_file("lock_tree/missing.txt", LOCK_MODE_EXCLUSIVE) == -1 && errno == ENOENT, "missing-file");
    struct stat st;
    expect(stat("lock_tree/missing.txt", &st) != 0, "missing-file: no file is made");

    if (system("rm -rf lock_tree") != 0) return 2;
    if (failures == 0) printf("all tests pass\n");
    return failures == 0 ? 0 : 1;
}
