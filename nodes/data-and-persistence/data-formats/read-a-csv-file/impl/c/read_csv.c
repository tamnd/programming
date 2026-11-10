// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

// The C standard library has no CSV reader, so this file has a small parser of RFC 4180.

#include "read_csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A growing array of bytes or of pointers.
struct bytes {
    char *p;
    size_t n, cap;
};

struct row {
    char **fields;
    size_t n, cap;
};

struct rows {
    struct row *p;
    size_t n, cap;
};

static bool push_byte(struct bytes *b, char c) {
    if (b->n + 1 >= b->cap) {
        size_t cap = b->cap ? 2 * b->cap : 64;
        char *p = realloc(b->p, cap);
        if (p == nullptr) {
            return false;
        }
        b->p = p;
        b->cap = cap;
    }
    b->p[b->n++] = c;
    b->p[b->n] = '\0';
    return true;
}

static bool push_field(struct row *r, char *field) {
    if (r->n == r->cap) {
        size_t cap = r->cap ? 2 * r->cap : 8;
        char **p = realloc(r->fields, cap * sizeof *p);
        if (p == nullptr) {
            return false;
        }
        r->fields = p;
        r->cap = cap;
    }
    r->fields[r->n++] = field;
    return true;
}

static bool push_row(struct rows *rs, struct row r) {
    if (rs->n == rs->cap) {
        size_t cap = rs->cap ? 2 * rs->cap : 16;
        struct row *p = realloc(rs->p, cap * sizeof *p);
        if (p == nullptr) {
            return false;
        }
        rs->p = p;
        rs->cap = cap;
    }
    rs->p[rs->n++] = r;
    return true;
}

static void free_row(struct row *r) {
    for (size_t i = 0; i < r->n; i++) {
        free(r->fields[i]);
    }
    free(r->fields);
}

static void free_rows(struct rows *rs) {
    for (size_t i = 0; i < rs->n; i++) {
        free_row(&rs->p[i]);
    }
    free(rs->p);
}

// Returns the length of the line break at i: 1 for LF, 2 for CRLF, 0 for none.
static size_t line_break(const char *s, size_t len, size_t i) {
    if (i < len && s[i] == '\n') {
        return 1;
    }
    return i + 1 < len && s[i] == '\r' && s[i + 1] == '\n' ? 2 : 0;
}

// Reads one field at *i into a new string in *out.
static enum csv_status parse_field(const char *s, size_t len, size_t *i, char **out) {
    struct bytes b = {0};
    if (!push_byte(&b, '\0')) { // an empty field is an empty string, not a null pointer
        return CSV_ERR_MEMORY;
    }
    b.n = 0;
    size_t j = *i;
    if (j < len && s[j] == '"') {
        j++;
        for (;;) {
            if (j >= len) {
                free(b.p);
                return CSV_ERR_BAD_QUOTE;
            }
            char c = s[j];
            size_t step = 1;
            if (c == '"') {
                if (j + 1 >= len || s[j + 1] != '"') {
                    break;
                }
                step = 2;
            } else if (line_break(s, len, j) == 2) {
                c = '\n'; // CRLF inside quotes becomes LF.
                step = 2;
            }
            if (!push_byte(&b, c)) {
                free(b.p);
                return CSV_ERR_MEMORY;
            }
            j += step;
        }
        j++; // the closing quote
        if (j < len && s[j] != ',' && line_break(s, len, j) == 0) {
            free(b.p);
            return CSV_ERR_BAD_QUOTE;
        }
    } else {
        while (j < len && s[j] != ',' && line_break(s, len, j) == 0) {
            if (!push_byte(&b, s[j++])) {
                free(b.p);
                return CSV_ERR_MEMORY;
            }
        }
    }
    *i = j;
    *out = b.p;
    return CSV_OK;
}

// Splits RFC 4180 text into records of fields. An empty line gives no record.
static enum csv_status parse_csv(const char *s, size_t len, struct rows *out) {
    struct rows rs = {0};
    size_t i = 0;
    while (i < len) {
        size_t empty = line_break(s, len, i);
        if (empty > 0) {
            i += empty;
            continue;
        }
        struct row r = {0};
        for (;;) {
            char *field = nullptr;
            enum csv_status st = parse_field(s, len, &i, &field);
            if (st == CSV_OK && !push_field(&r, field)) {
                free(field);
                st = CSV_ERR_MEMORY;
            }
            if (st != CSV_OK) {
                free_row(&r);
                free_rows(&rs);
                return st;
            }
            if (i >= len || s[i] != ',') {
                break;
            }
            i++;
        }
        i += line_break(s, len, i);
        if (!push_row(&rs, r)) {
            free_row(&r);
            free_rows(&rs);
            return CSV_ERR_MEMORY;
        }
    }
    *out = rs;
    return CSV_OK;
}

// Reads the whole file into a new buffer.
static enum csv_status read_file(const char *path, char **data, size_t *len) {
    FILE *f = fopen(path, "rb");
    if (f == nullptr) {
        return CSV_ERR_IO;
    }
    struct bytes b = {0};
    char chunk[4096];
    size_t got;
    while ((got = fread(chunk, 1, sizeof chunk, f)) > 0) {
        for (size_t k = 0; k < got; k++) {
            if (!push_byte(&b, chunk[k])) {
                free(b.p);
                fclose(f);
                return CSV_ERR_MEMORY;
            }
        }
    }
    bool failed = ferror(f);
    fclose(f);
    if (failed) {
        free(b.p);
        return CSV_ERR_IO;
    }
    *data = b.p; // null for an empty file
    *len = b.n;
    return CSV_OK;
}

enum csv_status read_csv(const char *path, struct csv_table *out) {
    *out = (struct csv_table){0};
    char *data = nullptr;
    size_t len = 0;
    enum csv_status st = read_file(path, &data, &len);
    if (st != CSV_OK) {
        return st;
    }
    const char *s = data;
    if (len >= 3 && memcmp(s, "\xEF\xBB\xBF", 3) == 0) { // the UTF-8 byte order mark
        s += 3;
        len -= 3;
    }
    struct rows rs = {0};
    st = parse_csv(s, len, &rs);
    free(data);
    if (st != CSV_OK) {
        return st;
    }
    if (rs.n == 0) {
        free(rs.p);
        return CSV_OK;
    }
    for (size_t r = 1; r < rs.n; r++) {
        if (rs.p[r].n != rs.p[0].n) {
            free_rows(&rs);
            return CSV_ERR_FIELD_COUNT;
        }
    }
    char ***records = rs.n > 1 ? malloc((rs.n - 1) * sizeof *records) : nullptr;
    if (rs.n > 1 && records == nullptr) {
        free_rows(&rs);
        return CSV_ERR_MEMORY;
    }
    for (size_t r = 1; r < rs.n; r++) {
        records[r - 1] = rs.p[r].fields;
    }
    *out = (struct csv_table){
        .ncols = rs.p[0].n, .header = rs.p[0].fields, .nrecords = rs.n - 1, .records = records};
    free(rs.p);
    return CSV_OK;
}

const char *csv_field(const struct csv_table *t, size_t r, const char *name) {
    for (size_t c = 0; c < t->ncols; c++) {
        if (strcmp(t->header[c], name) == 0) {
            return t->records[r][c];
        }
    }
    return nullptr;
}

void csv_table_free(struct csv_table *t) {
    for (size_t r = 0; r < t->nrecords; r++) {
        for (size_t c = 0; c < t->ncols; c++) {
            free(t->records[r][c]);
        }
        free(t->records[r]);
    }
    for (size_t c = 0; c < t->ncols; c++) {
        free(t->header[c]);
    }
    free(t->header);
    free(t->records);
    *t = (struct csv_table){0};
}
