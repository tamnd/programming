// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

#ifndef DELETE_FILE_H
#define DELETE_FILE_H

// delete_file deletes the file at path with unlink. It returns 1 if a file was
// there, 0 if not (ENOENT, or ENOTDIR for a path through a regular file), and
// -1 with errno from unlink on each other error, for example EACCES. unlink
// does not delete a directory, and it deletes a symbolic link, not its target.
int delete_file(const char *path);

#endif
