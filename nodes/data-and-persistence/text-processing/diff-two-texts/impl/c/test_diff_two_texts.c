// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

// mkdtemp and rmdir are POSIX functions.
#define _POSIX_C_SOURCE 200809L

#include "diff_two_texts.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct test_case {
    const char *name, *old_text, *new_text, *want;
};

static const struct test_case cases[] = {
    {"simple", "a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"},
    {"change-a-line", "one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n"},
    {"identical", "a\nb\n", "a\nb\n", " a\n b\n"},
    {"lines-of-a-poem", "The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n"},
    {"both-empty", "", "", ""},
    {"old-empty", "", "a\nb\n", "+a\n+b\n"},
    {"new-empty", "a\nb\n", "", "-a\n-b\n"},
    {"all-different", "a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"},
    {"insert-at-start", "b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"},
    {"delete-at-end", "a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"},
    {"swap", "a\nb\n", "b\na\n", "-a\n b\n+a\n"},
    {"repeated-lines", "a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n"},
    {"empty-line", "a\n\nb\n", "a\nb\n", " a\n-\n b\n"},
    {"no-final-newline", "a\nb", "a\nb\n", " a\n b\n"},
    {"crlf", "a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"},
    {"trailing-space", "a \nb\n", "a\nb\n", "-a \n+a\n b\n"},
    {"normalization", "caf\303\251\n", "cafe\314\201\n", "-caf\303\251\n+cafe\314\201\n"},
};

// write_file writes text to the file at path, and returns 0 if it can.
static int write_file(const char *path, const char *text) {
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return -1;
    }
    size_t len = strlen(text);
    int ok = fwrite(text, 1, len, f) == len;
    return fclose(f) == 0 && ok ? 0 : -1;
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        char *got = diff_lines(c->old_text, c->new_text);
        if (got == NULL || strcmp(got, c->want) != 0) {
            fprintf(stderr, "FAIL %s: got \"%s\"\n", c->name, got != NULL ? got : "(null)");
            failures++;
        }
        free(got);
    }

    // 1000 lines, and 10 of them change. Each line has at most 5 bytes.
    char *old_text = malloc(1000 * 5 + 1), *new_text = malloc(1000 * 5 + 1);
    size_t old_len = 0, new_len = 0;
    for (int i = 0; i < 1000; i++) {
        old_len += (size_t)snprintf(old_text + old_len, 1000 * 5 + 1 - old_len, "%d\n", i);
        new_len += (size_t)snprintf(new_text + new_len, 1000 * 5 + 1 - new_len, i % 100 == 50 ? "x%d\n" : "%d\n", i);
    }
    char *got = diff_lines(old_text, new_text);
    size_t lines = 0;
    for (const char *p = got; p != NULL && *p != '\0'; p++) {
        lines += *p == '\n';
    }
    const char *start = got != NULL ? strstr(got, " 49\n") : NULL;
    if (lines != 1010 || start == NULL || strncmp(start, " 49\n-50\n+x50\n", 13) != 0 ||
        strcmp(got + strlen(got) - 5, " 999\n") != 0) {
        fprintf(stderr, "FAIL many-lines\n");
        failures++;
    }
    free(got);
    free(old_text);
    free(new_text);

    char dir[] = "/tmp/diff-two-texts-XXXXXX";
    if (mkdtemp(dir) == NULL) {
        perror("mkdtemp");
        return 1;
    }
    char old_path[64], new_path[64], missing[64];
    snprintf(old_path, sizeof old_path, "%s/old.txt", dir);
    snprintf(new_path, sizeof new_path, "%s/new.txt", dir);
    snprintf(missing, sizeof missing, "%s/missing.txt", dir);
    got = NULL;
    if (write_file(old_path, "a\r\nb\n") != 0 || write_file(new_path, "a\nb\n") != 0 ||
        (got = diff_files(old_path, new_path)) == NULL || strcmp(got, "-a\r\n+a\n b\n") != 0) {
        fprintf(stderr, "FAIL diff-files\n");
        failures++;
    }
    free(got);
    errno = 0;
    if (diff_files(missing, new_path) != NULL || errno != ENOENT) {
        fprintf(stderr, "FAIL not-found\n");
        failures++;
    }
    remove(old_path);
    remove(new_path);
    rmdir(dir);
    if (failures == 0) {
        printf("ok\n");
    }
    return failures == 0 ? 0 : 1;
}
