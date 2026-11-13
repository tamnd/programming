#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-size-of-a-file/.

# file_size PATH
# Prints the size in bytes of the regular file at PATH. POSIX has no stat
# utility, so the function uses wc -c, which counts bytes, not characters.
# The input comes from a redirection, so wc prints no file name, and the
# function removes the spaces that some wc versions put before the number.
# The redirection follows a symbolic link. The function returns status 1 for
# a missing file and for a directory.
file_size() {
  if [[ -d $1 ]]; then
    printf 'file_size: %s: is a directory\n' "$1" >&2
    return 1
  fi
  if [[ ! -e $1 ]]; then
    printf 'file_size: %s: no such file\n' "$1" >&2
    return 1
  fi
  local n
  n=$(wc -c <"$1") || return 1
  printf '%s\n' "${n//[[:space:]]/}"
}
