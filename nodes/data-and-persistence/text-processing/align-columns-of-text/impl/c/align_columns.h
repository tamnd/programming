// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

#ifndef ALIGN_COLUMNS_H
#define ALIGN_COLUMNS_H

// Returns text with its fields in columns, in a new string that the caller
// frees. text is UTF-8. A line ends at LF. Each $ in a line separates two
// fields, and a $ at the end of a line does not start a new field. An empty
// line has no fields. The width of a column is the largest number of code
// points of a field in it. Each field is padded with spaces to the width of
// its column, on the right ("left"), on the left ("right"), or on the two
// sides ("center"), with the odd space on the right. One space separates two
// columns. The padding on the right of the last field of a line is not
// written. Each line of the result ends with LF. Returns NULL with errno set
// to EINVAL if align is not "left", "right" or "center", or to ENOMEM if
// there is no memory.
char *align_columns(const char *text, const char *align);

#endif
