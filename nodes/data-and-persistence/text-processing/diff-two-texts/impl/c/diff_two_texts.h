// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

#ifndef DIFF_TWO_TEXTS_H
#define DIFF_TWO_TEXTS_H

// diff_lines returns the edit script that changes the lines of old_text into
// the lines of new_text, as a new string. Each line of the result is a space
// and a line that stays, a - and a line of old_text that goes, or a + and a
// line of new_text that comes, with an LF at the end. The script has the
// smallest number of - and + lines. When two scripts are equally short, a -
// comes before a +. A line ends at LF, and a final LF does not start a new
// line. Free the result with free. On an error it returns NULL and sets errno
// to ENOMEM.
char *diff_lines(const char *old_text, const char *new_text);

// diff_files returns diff_lines of the two files at old_path and new_path. On
// an error it returns NULL and sets errno: the error of fopen, EIO for a read
// error, or ENOMEM.
char *diff_files(const char *old_path, const char *new_path);

#endif
