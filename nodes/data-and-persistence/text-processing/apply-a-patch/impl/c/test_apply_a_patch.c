// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

// mkdtemp and rmdir are POSIX functions.
#define _POSIX_C_SOURCE 200809L

#include "apply_a_patch.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct test_case {
    const char *name, *old_text, *patch, *want;
};

static const struct test_case cases[] = {
    {"change-a-line", "a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n"},
    {"two-hunks", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n"},
    {"git-header", "def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n"},
    {"add-at-end", "a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n"},
    {"old-empty", "", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n"},
    {"delete-all", "a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", ""},
    {"no-count", "a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n"},
    {"zero-context", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n"},
    {"insert-at-start", "b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n"},
    {"old-no-final-newline", "a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n"},
    {"new-no-final-newline", "a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb"},
    {"empty-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n"},
    {"space-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n"},
    {"crlf", "a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n"},
    {"header-like-line", "x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n"},
    {"non-ascii", "caf\303\251\n", "@@ -1 +1 @@\n-caf\303\251\n+na\303\257ve\n", "na\303\257ve\n"},
};

struct error_case {
    const char *name, *old_text, *patch;
    enum patch_status want;
};

static const struct error_case errors[] = {
    {"mismatch", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n", PATCH_MISMATCH},
    {"malformed", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-b\n", PATCH_MALFORMED},
    {"no-hunk", "a\n", "no hunk here\n", PATCH_MALFORMED},
    {"bad-mark", "a\n", "@@ -1 +1 @@\n*a\n", PATCH_MALFORMED},
    {"past-the-end", "a\n", "@@ -2 +2 @@\n-a\n+b\n", PATCH_MISMATCH},
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
    char *got;
    size_t line;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        enum patch_status st = apply_patch(c->old_text, c->patch, &got, &line);
        if (st != PATCH_OK || strcmp(got, c->want) != 0) {
            fprintf(stderr, "FAIL %s: status %d\n", c->name, (int)st);
            failures++;
        }
        free(got);
    }
    for (size_t i = 0; i < sizeof errors / sizeof errors[0]; i++) {
        const struct error_case *c = &errors[i];
        enum patch_status st = apply_patch(c->old_text, c->patch, &got, &line);
        if (st != c->want || got != NULL) {
            fprintf(stderr, "FAIL %s: status %d\n", c->name, (int)st);
            failures++;
        }
    }

    // 1000 lines, and 10 hunks with no context change 10 of them.
    enum { CAP = 32000 };
    char *old_text = malloc(CAP), *patch = malloc(CAP), *want = malloc(CAP);
    if (old_text == NULL || patch == NULL || want == NULL) {
        return 1;
    }
    size_t old_len = 0, patch_len = 0, want_len = 0;
    for (int i = 0; i < 1000; i++) {
        old_len += (size_t)snprintf(old_text + old_len, CAP - old_len, "%d\n", i);
        if (i % 100 == 50) {
            patch_len += (size_t)snprintf(patch + patch_len, CAP - patch_len, "@@ -%d +%d @@\n-%d\n+x%d\n", i + 1,
                                          i + 1, i, i);
            want_len += (size_t)snprintf(want + want_len, CAP - want_len, "x%d\n", i);
        } else {
            want_len += (size_t)snprintf(want + want_len, CAP - want_len, "%d\n", i);
        }
    }
    if (apply_patch(old_text, patch, &got, &line) != PATCH_OK || strcmp(got, want) != 0) {
        fprintf(stderr, "FAIL many-hunks\n");
        failures++;
    }
    free(got);
    free(old_text);
    free(patch);
    free(want);

    char dir[] = "/tmp/apply-a-patch-XXXXXX";
    if (mkdtemp(dir) == NULL) {
        perror("mkdtemp");
        return 1;
    }
    char old_path[64], patch_path[64], missing[64];
    snprintf(old_path, sizeof old_path, "%s/old.txt", dir);
    snprintf(patch_path, sizeof patch_path, "%s/a.patch", dir);
    snprintf(missing, sizeof missing, "%s/missing.txt", dir);
    got = NULL;
    if (write_file(old_path, "a\r\nb\r\n") != 0 || write_file(patch_path, "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n") != 0 ||
        apply_patch_file(old_path, patch_path, &got, &line) != PATCH_OK || strcmp(got, "a\r\nc\r\n") != 0) {
        fprintf(stderr, "FAIL apply-patch-file\n");
        failures++;
    }
    free(got);
    errno = 0;
    if (apply_patch_file(missing, patch_path, &got, &line) != PATCH_IO || errno != ENOENT) {
        fprintf(stderr, "FAIL not-found\n");
        failures++;
    }
    remove(old_path);
    remove(patch_path);
    rmdir(dir);
    if (failures == 0) {
        printf("ok\n");
    }
    return failures == 0 ? 0 : 1;
}
