// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

#ifndef READ_CSV_H
#define READ_CSV_H

#include <stddef.h>

enum csv_status {
    CSV_OK = 0,
    CSV_ERR_IO,          // the file cannot be read; errno tells why
    CSV_ERR_FIELD_COUNT, // a record has more or fewer fields than the header
    CSV_ERR_BAD_QUOTE,   // a quoted field has no closing quote, or has text after it
    CSV_ERR_MEMORY,
};

// A CSV file with a header. records[r][c] is the field of record r in the column header[c].
struct csv_table {
    size_t ncols;
    char **header;
    size_t nrecords;
    char ***records;
};

// Reads the CSV file at path into *out. On an error, *out is empty and needs no free.
enum csv_status read_csv(const char *path, struct csv_table *out);

// Returns the field of record r in the column with the given name, or NULL if no column has that name.
const char *csv_field(const struct csv_table *t, size_t r, const char *name);

void csv_table_free(struct csv_table *t);

#endif
