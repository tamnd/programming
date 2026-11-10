// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

#include "parse_args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define MAX_ARGS 8

// run copies the arguments into a mutable argv with a program name first,
// because getopt_long can permute argv.
static enum parse_result run(const char *const *args, struct count_args *out, char **argv) {
    int argc = 0;
    argv[argc++] = (char *)"count";
    for (; args[argc - 1] != nullptr; argc++) argv[argc] = (char *)args[argc - 1];
    argv[argc] = nullptr;
    return parse_args(argc, argv, out);
}

static void check(const char *name, const char *const *args, bool verbose, int limit,
                  const char *const *files) {
    char *argv[MAX_ARGS + 2];
    struct count_args got;
    bool ok = run(args, &got, argv) == PARSE_OK && got.verbose == verbose && got.limit == limit;
    size_t n = 0;
    while (files[n] != nullptr) n++;
    ok = ok && got.nfiles == n;
    for (size_t i = 0; ok && i < n; i++) ok = strcmp(got.files[i], files[i]) == 0;
    if (!ok) {
        fprintf(stderr, "case %s: wrong result\n", name);
        failures++;
    }
}

static void check_error(const char *name, const char *const *args) {
    char *argv[MAX_ARGS + 2];
    struct count_args got;
    if (run(args, &got, argv) == PARSE_OK) {
        fprintf(stderr, "case %s: no error\n", name);
        failures++;
    }
}

#define L(...) ((const char *const[]){__VA_ARGS__ __VA_OPT__(, ) nullptr})

int main(void) {
    check("files-only", L("a.txt", "b.txt"), false, 10, L("a.txt", "b.txt"));
    check("short-options", L("-v", "-n", "3", "a.txt"), true, 3, L("a.txt"));
    check("long-options", L("--verbose", "--limit", "5", "a.txt", "b.txt"), true, 5, L("a.txt", "b.txt"));
    check("no-args", L(), false, 10, L());
    check("long-equals", L("--limit=7", "notes.md"), false, 7, L("notes.md"));
    check("repeated", L("-n", "3", "--limit", "5", "a.txt"), false, 5, L("a.txt"));
    check("zero", L("-n", "0", "a.txt"), false, 0, L("a.txt"));
    check("end-of-options", L("-v", "--", "-n", "--limit=4"), true, 10, L("-n", "--limit=4"));
    check("only-terminator", L("--"), false, 10, L());
    check("second-terminator", L("--", "--"), false, 10, L("--"));
    check("dash", L("-"), false, 10, L("-"));
    check("dash-after-option", L("-n", "2", "-", "b.txt"), false, 2, L("-", "b.txt"));
    check("empty-arg", L(""), false, 10, L(""));
    check("space", L("-v", "my file.txt"), true, 10, L("my file.txt"));
    check("non-ascii", L("--limit=4", "日本語.txt", "café.md"), false, 4, L("日本語.txt", "café.md"));

    check_error("unknown-short", L("-x", "a.txt"));
    check_error("unknown-long", L("--color", "a.txt"));
    check_error("missing-value", L("-n"));
    check_error("missing-value-long", L("--limit"));
    check_error("not-a-number", L("-n", "ten", "a.txt"));
    check_error("not-a-number-long", L("--limit=ten"));
    check_error("empty-value", L("--limit=", "a.txt"));
    check_error("verbose-with-value", L("--verbose=yes"));

    if (failures != 0) return EXIT_FAILURE;
    puts("ok");
    return EXIT_SUCCESS;
}
