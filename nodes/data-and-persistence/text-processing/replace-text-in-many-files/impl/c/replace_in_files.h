// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

#ifndef REPLACE_IN_FILES_H
#define REPLACE_IN_FILES_H

#include <stddef.h>

// Replaces each instance of old_text with new_text in each of the n files in
// paths, and puts the number of instances in each file in counts. old_text and
// new_text are plain text, not patterns. Instances do not overlap. A file with
// no instance is not written. A changed file goes to a new file in the same
// directory, and rename puts it in the place of the old one. The files are
// done in order. Returns 0, or -1 with errno set at the first error: the files
// before have the new text, and the files after do not change. An empty
// old_text sets errno to EINVAL.
int replace_in_files(const char *const paths[], size_t n, const char *old_text, const char *new_text,
                     size_t counts[]);

#endif
