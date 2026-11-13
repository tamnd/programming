#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

# file_mtime PATH
# Prints the modification time of the file at PATH, in whole seconds since
# 1970-01-01T00:00:00Z. POSIX has no utility that prints this time in
# seconds. date -r FILE does it in GNU date and in the date of macOS and the
# BSDs, but it is not POSIX. The date of macOS reads a number as a count of
# seconds when no file has that name, so the function returns status 1 for a
# missing file first. date -r follows a symbolic link.
file_mtime() {
  if [[ ! -e $1 ]]; then
    printf 'file_mtime: %s: no such file\n' "$1" >&2
    return 1
  fi
  date -r "$1" +%s
}
