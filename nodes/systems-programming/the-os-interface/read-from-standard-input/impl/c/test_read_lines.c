// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

#include "read_lines.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "read_lines_input.txt";
static int failures = 0;

// check writes the data to a file, makes the file the standard input and
// compares the lines with want, an array that ends with NULL.
static void check(const char *name, const char *data, size_t n, const char *const *want) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
    assert(freopen(path, "rb", stdin) != NULL);

    char **lines;
    size_t count;
    if (read_lines(stdin, &lines, &count) != 0) {
        fprintf(stderr, "case %s: read_lines failed\n", name);
        failures++;
        return;
    }
    size_t i = 0;
    for (; want[i] != NULL; i++) {
        if (i >= count || strcmp(lines[i], want[i]) != 0) break;
    }
    if (want[i] != NULL || i != count) {
        fprintf(stderr, "case %s: got %zu lines, line %zu differs\n", name, count, i);
        failures++;
    }
    free_lines(lines, count);
}

#define CASE(name, data, ...) check(name, data, sizeof(data) - 1, (const char *const[]){__VA_ARGS__})

int main(void) {
    CASE("lf", "alpha\nbeta\ngamma\n", "alpha", "beta", "gamma", NULL);
    CASE("blank-lines", "first\n\n\nlast line\n", "first", "", "", "last line", NULL);
    CASE("empty", "", NULL);
    CASE("only-newline", "\n", "", NULL);
    CASE("no-final-newline", "one\ntwo\nthree", "one", "two", "three", NULL);
    CASE("crlf", "one\r\ntwo\r\nthree\r\n", "one", "two", "three", NULL);
    CASE("crlf-no-final-newline", "one\r\ntwo\r\nthree", "one", "two", "three", NULL);
    CASE("mixed-endings", "one\r\ntwo\nthree\r\n", "one", "two", "three", NULL);
    CASE("trailing-blank-line", "last\n\n", "last", "", NULL);
    CASE("whitespace", "  two spaces\ttab \n", "  two spaces\ttab ", NULL);
    CASE("backslashes", "C:\\temp\\new\n", "C:\\temp\\new", NULL);
    CASE("non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", "naïve café", "日本語のテキスト",
         "🙂🙂🙂🙂🙂🙂", NULL);

    size_t n = (size_t)1 << 20;
    char *data = malloc(n + 5);
    char *line = malloc(n + 1);
    assert(data != NULL && line != NULL);
    memset(data, 'x', n);
    memcpy(data + n, "\nend\n", 5);
    memcpy(line, data, n);
    line[n] = '\0';
    check("long-line", data, n + 5, (const char *const[]){line, "end", NULL});
    free(line);
    free(data);

    remove(path);
    if (failures > 0) return 1;
    puts("all cases pass");
    return 0;
}
