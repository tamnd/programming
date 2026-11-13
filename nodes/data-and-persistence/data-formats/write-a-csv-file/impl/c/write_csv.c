// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

#include "write_csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A record with one empty field needs quotes, or the line is empty and a reader skips it.
static int needs_quotes(const char *field, int alone) {
    return (alone && field[0] == '\0') || strpbrk(field, ",\"\r\n") != NULL;
}

enum csv_status format_csv(const char *const *const *records, const size_t *ncols, size_t nrecords, char **out) {
    *out = NULL;
    // The first pass checks the records and counts the bytes (one more for each field, for a comma), and the second pass writes them.
    size_t size = 1;
    for (size_t r = 0; r < nrecords; r++) {
        if (ncols[r] != ncols[0]) {
            return CSV_ERR_FIELD_COUNT;
        }
        for (size_t c = 0; c < ncols[r]; c++) {
            const char *f = records[r][c];
            size += strlen(f) + 1;
            if (needs_quotes(f, ncols[r] == 1)) {
                size += 2;
                for (const char *p = f; *p; p++) {
                    size += *p == '"';
                }
            }
        }
        size += 2; // CRLF
    }
    char *buf = malloc(size);
    if (buf == NULL) {
        return CSV_ERR_MEMORY;
    }
    char *w = buf;
    for (size_t r = 0; r < nrecords; r++) {
        for (size_t c = 0; c < ncols[r]; c++) {
            const char *f = records[r][c];
            if (c > 0) {
                *w++ = ',';
            }
            if (needs_quotes(f, ncols[r] == 1)) {
                *w++ = '"';
                for (const char *p = f; *p; p++) {
                    if (*p == '"') {
                        *w++ = '"';
                    }
                    *w++ = *p;
                }
                *w++ = '"';
            } else {
                size_t n = strlen(f);
                memcpy(w, f, n);
                w += n;
            }
        }
        *w++ = '\r';
        *w++ = '\n';
    }
    *w = '\0';
    *out = buf;
    return CSV_OK;
}

enum csv_status write_csv(const char *path, const char *const *const *records, const size_t *ncols, size_t nrecords) {
    char *text;
    enum csv_status st = format_csv(records, ncols, nrecords, &text);
    if (st != CSV_OK) {
        return st;
    }
    // "wb": on Windows, text mode would write each LF as CRLF, also inside a field.
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        free(text);
        return CSV_ERR_IO;
    }
    size_t n = strlen(text);
    int ok = fwrite(text, 1, n, f) == n;
    ok = fclose(f) == 0 && ok;
    free(text);
    return ok ? CSV_OK : CSV_ERR_IO;
}
