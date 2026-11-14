// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

#ifndef REMOVE_LINES_H
#define REMOVE_LINES_H

// Removes count lines from the file at path, from line start, and returns how
// many it removed. Line 1 is the first line. A line ends at LF, and the LF
// goes with the line. The result is less than count if the file ends first.
// If no line goes, the file is not written. Otherwise the rest of the file
// goes to a new file in the same directory, and rename puts it in the place
// of the old one. On an error, the function returns -1 and sets errno:
// EINVAL if start is less than 1 or count is less than 0, ENOENT if the file
// does not exist.
int remove_lines(const char *path, int start, int count);

#endif
