// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

#define _POSIX_C_SOURCE 200809L

#include "write_csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXR 3
#define MAXC 3

struct test_case {
    const char *name;
    size_t nrecords;
    size_t ncols[MAXR];
    const char *records[MAXR][MAXC];
    const char *want;
};

static const struct test_case cases[] = {
    {"simple", 3, {2, 2, 2}, {{"name", "age"}, {"Ada", "36"}, {"Alan", "41"}}, "name,age\r\nAda,36\r\nAlan,41\r\n"},
    {"comma", 2, {2, 2}, {{"name", "born"}, {"Lovelace, Ada", "1815"}}, "name,born\r\n\"Lovelace, Ada\",1815\r\n"},
    {"double-quote", 2, {2, 2}, {{"id", "quote"}, {"1", "She said \"hi\"."}}, "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n"},
    {"only-a-quote", 2, {1, 1}, {{"q"}, {"\""}}, "q\r\n\"\"\"\"\r\n"},
    {"header-only", 1, {2}, {{"name", "age"}}, "name,age\r\n"},
    {"newline-in-field", 2, {2, 2}, {{"id", "address"}, {"1", "12 High St\nLeeds"}}, "id,address\r\n1,\"12 High St\nLeeds\"\r\n"},
    {"crlf-in-field", 2, {2, 2}, {{"id", "address"}, {"1", "12 High St\r\nLeeds"}}, "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n"},
    {"cr-in-field", 2, {2, 2}, {{"id", "text"}, {"1", "a\rb"}}, "id,text\r\n1,\"a\rb\"\r\n"},
    {"empty-fields", 2, {3, 3}, {{"a", "b", "c"}, {"", "", ""}}, "a,b,c\r\n,,\r\n"},
    {"one-empty-field", 3, {1, 1, 1}, {{"note"}, {""}, {"x"}}, "note\r\n\"\"\r\nx\r\n"},
    {"spaces", 2, {2, 2}, {{"a", "b"}, {" x ", " y"}}, "a,b\r\n x , y\r\n"},
    {"no-records", 0, {0}, {{NULL}}, ""},
    {"non-ascii", 3, {1, 1, 1}, {{"city"}, {"Z\xc3\xbcrich"}, {"\xe6\x9d\xb1\xe4\xba\xac"}}, "city\r\nZ\xc3\xbcrich\r\n\xe6\x9d\xb1\xe4\xba\xac\r\n"},
    {"too-many-fields", 2, {2, 3}, {{"a", "b"}, {"1", "2", "3"}}, NULL},
    {"too-few-fields", 2, {2, 1}, {{"a", "b"}, {"1"}}, NULL},
};

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return NULL;
    }
    char *buf = calloc(1, 4096);
    size_t n = buf ? fread(buf, 1, 4095, f) : 0;
    (void)n;
    fclose(f);
    return buf;
}

int main(void) {
    int failed = 0;
    char path[] = "/tmp/write-csv-XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) {
        perror("mkstemp");
        return 1;
    }
    close(fd);
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *t = &cases[i];
        const char *const *rows[MAXR];
        for (size_t r = 0; r < MAXR; r++) {
            rows[r] = t->records[r];
        }
        char *got = NULL;
        enum csv_status st = format_csv(rows, t->ncols, t->nrecords, &got);
        int ok;
        if (t->want == NULL) {
            ok = st == CSV_ERR_FIELD_COUNT && got == NULL && write_csv(path, rows, t->ncols, t->nrecords) == CSV_ERR_FIELD_COUNT;
        } else {
            ok = st == CSV_OK && strcmp(got, t->want) == 0 && write_csv(path, rows, t->ncols, t->nrecords) == CSV_OK;
            char *data = ok ? read_file(path) : NULL;
            ok = ok && data != NULL && strcmp(data, t->want) == 0;
            free(data);
        }
        free(got);
        if (!ok) {
            printf("FAIL %s\n", t->name);
            failed++;
        }
    }
    unlink(path);
    printf("%s\n", failed ? "FAILED" : "all tests passed");
    return failed != 0;
}
