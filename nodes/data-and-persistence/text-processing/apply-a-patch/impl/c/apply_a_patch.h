// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

#ifndef APPLY_A_PATCH_H
#define APPLY_A_PATCH_H

#include <stddef.h>

enum patch_status {
    PATCH_OK,
    PATCH_IO,         // a file cannot be read, and errno tells why
    PATCH_NOMEM,      // there is not enough memory
    PATCH_MALFORMED,  // the patch is not a correct unified diff
    PATCH_MISMATCH,   // a hunk does not match the text
};

// apply_patch applies the unified diff patch to old_text, with no fuzz and no
// offset. Each hunk must match the lines of old_text at the start of its old
// range. On PATCH_OK, *out is a new string that the caller frees with free.
// On PATCH_MALFORMED and PATCH_MISMATCH, *line is the number of the line of
// the patch where the error is.
enum patch_status apply_patch(const char *old_text, const char *patch, char **out, size_t *line);

// apply_patch_file is apply_patch of the two files at path and patch_path.
// On PATCH_IO, errno is the error of fopen, or EIO for a read error.
enum patch_status apply_patch_file(const char *path, const char *patch_path, char **out, size_t *line);

#endif
