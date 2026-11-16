// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

#include "read_ports.h"

#include <string.h>

static int failures = 0;

static void check(const char *name, const char *const texts[], size_t n, const int want_ports[], size_t want_n,
                  const char *want_log) {
    int ports[8];
    char log[1024];
    FILE *f = tmpfile();
    if (f == NULL) {
        fprintf(stderr, "case %s failed: no temporary file\n", name);
        failures++;
        return;
    }
    size_t got_n = read_ports(texts, n, ports, f);
    rewind(f);
    size_t len = fread(log, 1, sizeof log - 1, f);
    log[len] = '\0';
    fclose(f);
    int same = got_n == want_n && strcmp(log, want_log) == 0;
    for (size_t i = 0; same && i < got_n; i++) {
        same = ports[i] == want_ports[i];
    }
    if (!same) {
        fprintf(stderr, "case %s failed: log %s\n", name, log);
        failures++;
    }
}

int main(void) {
    check("one-bad", (const char *const[]){"8080", "x", "443"}, 3, (const int[]){8080, 443}, 2, "level=ERROR msg=\"read port failed\" line=2 err=\"not a number: 'x'\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n");
    check("all-good", (const char *const[]){"80", "443"}, 2, (const int[]){80, 443}, 2, "level=INFO msg=\"ports read\" ok=2 total=2\n");
    check("out-of-range", (const char *const[]){"70000"}, 1, (const int[]){0}, 0, "level=ERROR msg=\"read port failed\" line=1 err=\"out of range: 70000\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("empty-list", (const char *const[]){NULL}, 0, (const int[]){0}, 0, "level=INFO msg=\"ports read\" ok=0 total=0\n");
    check("empty-text", (const char *const[]){""}, 1, (const int[]){0}, 0, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: ''\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("boundary", (const char *const[]){"1", "65535", "0"}, 3, (const int[]){1, 65535}, 2, "level=ERROR msg=\"read port failed\" line=3 err=\"out of range: 0\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n");
    check("newline", (const char *const[]){"80\nlevel=INFO msg=\"all good\""}, 1, (const int[]){0}, 0, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\nlevel=INFO msg=\\\"all good\\\"'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("backslash", (const char *const[]){"C:\\port"}, 1, (const int[]){0}, 0, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: 'C:\\\\port'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("carriage-return", (const char *const[]){"80\r"}, 1, (const int[]){0}, 0, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\r'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("tab", (const char *const[]){"8\t0"}, 1, (const int[]){0}, 0, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '8\\t0'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
