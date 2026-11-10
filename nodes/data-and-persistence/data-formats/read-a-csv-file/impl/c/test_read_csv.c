// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

#define _POSIX_C_SOURCE 200809L

#include "read_csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 4

// Each record is written as "name=field" pairs joined by "|".
struct test_case {
    const char *name;
    const char *data;
    const char *want[MAX];
};

static const struct test_case cases[] = {
    {"simple", "name,age\nAda,36\nAlan,41\n", {"name=Ada|age=36", "name=Alan|age=41"}},
    {"crlf", "name,age\r\nAda,36\r\nAlan,41\r\n", {"name=Ada|age=36", "name=Alan|age=41"}},
    {"no-final-newline", "name,age\nAda,36\nAlan,41", {"name=Ada|age=36", "name=Alan|age=41"}},
    {"quoted-comma", "name,born\n\"Lovelace, Ada\",1815\n", {"name=Lovelace, Ada|born=1815"}},
    {"escaped-quote", "id,quote\n1,\"She said \"\"hi\"\".\"\n", {"id=1|quote=She said \"hi\"."}},
    {"quoted-header", "\"first name\",\"last name\"\nAda,Lovelace\n", {"first name=Ada|last name=Lovelace"}},
    {"newline-in-field", "id,address\n1,\"12 High St\nLeeds\"\n", {"id=1|address=12 High St\nLeeds"}},
    {"crlf-in-field", "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n", {"id=1|address=12 High St\nLeeds"}},
    {"blank-line-in-field", "id,note\n1,\"one\n\ntwo\"\n", {"id=1|note=one\n\ntwo"}},
    {"empty-last-line", "a,b\n1,2\n\n", {"a=1|b=2"}},
    {"blank-lines", "a,b\n\n1,2\n\n\n3,4\n", {"a=1|b=2", "a=3|b=4"}},
    {"empty-fields", "a,b,c\n,,\n", {"a=|b=|c="}},
    {"quoted-empty", "name\n\"\"\n", {"name="}},
    {"spaces", "a,b\n x , y \n", {"a= x |b= y "}},
    {"header-only", "a,b\n", {nullptr}},
    {"empty-file", "", {nullptr}},
    {"bom", "\xEF\xBB\xBFname,age\nAda,36\n", {"name=Ada|age=36"}},
    {"non-ascii", "city,country\nZürich,Schweiz\n東京,日本\n", {"city=Zürich|country=Schweiz", "city=東京|country=日本"}},
};

struct invalid_case {
    const char *name;
    const char *data;
    enum csv_status want;
};

static const struct invalid_case invalid[] = {
    {"too-few-fields", "a,b\n1\n", CSV_ERR_FIELD_COUNT},
    {"too-many-fields", "a,b\n1,2,3\n", CSV_ERR_FIELD_COUNT},
    {"unclosed-quote", "a,b\n1,\"two\n", CSV_ERR_BAD_QUOTE},
    {"text-after-quote", "a,b\n1,\"two\"x\n", CSV_ERR_BAD_QUOTE},
};

// Writes data to a new temporary file and stores its path in path.
static int write_temp(char path[static 32], const char *data) {
    strcpy(path, "/tmp/read-csv-XXXXXX");
    int fd = mkstemp(path);
    if (fd < 0) {
        perror("mkstemp");
        return -1;
    }
    size_t len = strlen(data);
    bool ok = write(fd, data, len) == (ssize_t)len;
    close(fd);
    return ok ? 0 : -1;
}

// Joins the fields of record r as "name=field|...".
static void join(const struct csv_table *t, size_t r, char *out, size_t size) {
    out[0] = '\0';
    for (size_t c = 0; c < t->ncols; c++) {
        size_t used = strlen(out);
        snprintf(out + used, size - used, "%s%s=%s", c ? "|" : "", t->header[c], t->records[r][c]);
    }
}

int main(void) {
    int failed = 0;
    char path[32];
    char got[256];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *tc = &cases[i];
        if (write_temp(path, tc->data) != 0) {
            return 1;
        }
        struct csv_table t;
        enum csv_status st = read_csv(path, &t);
        unlink(path);
        size_t nwant = 0;
        while (nwant < MAX && tc->want[nwant] != nullptr) {
            nwant++;
        }
        bool ok = st == CSV_OK && t.nrecords == nwant;
        for (size_t r = 0; ok && r < nwant; r++) {
            join(&t, r, got, sizeof got);
            ok = strcmp(got, tc->want[r]) == 0;
        }
        if (!ok) {
            fprintf(stderr, "FAIL %s: status %d, %zu records\n", tc->name, (int)st, t.nrecords);
            failed++;
        }
        csv_table_free(&t);
    }
    for (size_t i = 0; i < sizeof invalid / sizeof invalid[0]; i++) {
        if (write_temp(path, invalid[i].data) != 0) {
            return 1;
        }
        struct csv_table t;
        enum csv_status st = read_csv(path, &t);
        unlink(path);
        if (st != invalid[i].want || t.nrecords != 0) {
            fprintf(stderr, "FAIL %s: status %d\n", invalid[i].name, (int)st);
            failed++;
        }
    }
    struct csv_table t;
    if (read_csv("/nonexistent/read-csv/missing.csv", &t) != CSV_ERR_IO) {
        fprintf(stderr, "FAIL missing\n");
        failed++;
    }
    if (write_temp(path, "name,age\nAda,36\n") != 0 || read_csv(path, &t) != CSV_OK) {
        return 1;
    }
    unlink(path);
    const char *age = csv_field(&t, 0, "age");
    if (age == nullptr || strcmp(age, "36") != 0 || csv_field(&t, 0, "town") != nullptr) {
        fprintf(stderr, "FAIL csv_field\n");
        failed++;
    }
    csv_table_free(&t);
    if (failed > 0) {
        return 1;
    }
    printf("ok\n");
    return 0;
}
