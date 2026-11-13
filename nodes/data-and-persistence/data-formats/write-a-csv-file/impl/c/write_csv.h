// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

#ifndef WRITE_CSV_H
#define WRITE_CSV_H

#include <stddef.h>

enum csv_status {
    CSV_OK = 0,
    CSV_ERR_IO,          // the file cannot be written; errno tells why
    CSV_ERR_FIELD_COUNT, // a record has more or fewer fields than the first record (the header)
    CSV_ERR_MEMORY,
};

// nrecords records of ncols[r] fields each. records[r][c] is a NUL-terminated UTF-8 string.
// On success, *out is the CSV text, which the caller frees with free.
enum csv_status format_csv(const char *const *const *records, const size_t *ncols, size_t nrecords, char **out);

// Writes the CSV text of the records to the file at path, with no byte order mark.
enum csv_status write_csv(const char *path, const char *const *const *records, const size_t *ncols, size_t nrecords);

#endif
