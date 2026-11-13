// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

#define _POSIX_C_SOURCE 200809L

#include "watch_file.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *path = "watch_tree/watched.txt";
static int failures = 0;

static void expect(int ok, const char *what) {
    if (!ok) {
        fprintf(stderr, "%s\n", what);
        failures++;
    }
}

static void fresh(void) {
    if (system("rm -rf watch_tree && mkdir watch_tree") != 0) exit(2);
}

struct test_case {
    const char *name;
    const char *steps[4];
    const char *expect[4];
};

static const struct test_case cases[] = {
    {"create", {"create:hello\n"}, {"created"}},
    {"create-then-write", {"create:a\n", "write:a much longer line\n"}, {"created", "modified"}},
    {"create-then-delete", {"create:a\n", "delete"}, {"created", "deleted"}},
    {"append", {"create:a\n", "append:b\n", "append:c\n"}, {"created", "modified", "modified"}},
    {"no-change", {"create:a\n", "none", "none"}, {"created", "none", "none"}},
    {"same-size", {"create:abc\n", "write:xyz\n"}, {"created", "modified"}},
    {"touch-only", {"create:a\n", "touch"}, {"created", "modified"}},
    {"replace-by-rename", {"create:old\n", "replace:new\n"}, {"created", "modified"}},
    {"delete-and-create", {"create:a\n", "recreate:b\n"}, {"created", "modified"}},
    {"never-created", {"none"}, {"none"}},
};

static void write_with_mtime(const char *text, time_t secs) {
    FILE *f = fopen(path, "w");
    if (!f || fputs(text, f) < 0 || fclose(f) != 0) exit(2);
    struct timespec t[2] = {{secs, 0}, {secs, 0}};
    if (utimensat(AT_FDCWD, path, t, 0) != 0) exit(2);
}

int main(void) {
    for (size_t c = 0; c < sizeof cases / sizeof cases[0]; c++) {
        fresh();
        size_t n = 0;
        while (n < 4 && cases[c].steps[n]) n++;
        enum watch_change got[4];
        int rc = watch_changes(path, cases[c].steps, n, got);
        int ok = rc == 0;
        for (size_t i = 0; ok && i < n; i++) ok = strcmp(watch_change_name(got[i]), cases[c].expect[i]) == 0;
        expect(ok, cases[c].name);
    }

    fresh();
    const char *mkdir_step[] = {"mkdir"};
    enum watch_change got[1];
    errno = 0;
    expect(watch_changes(path, mkdir_step, 1, got) == -1 && errno == EISDIR, "directory");

    // The same size and the same mtime hide a change.
    fresh();
    write_with_mtime("abc\n", 5);
    struct file_watcher w;
    enum watch_change change;
    expect(watcher_init(&w, path) == 0, "init");
    write_with_mtime("xyz\n", 5);
    expect(watcher_poll(&w, &change) == 0 && change == WATCH_NONE, "same size and time");
    write_with_mtime("xyz!\n", 5);
    expect(watcher_poll(&w, &change) == 0 && change == WATCH_MODIFIED, "new size");

    if (system("rm -rf watch_tree") != 0) failures++;
    if (failures) return 1;
    puts("all tests pass");
    return 0;
}
