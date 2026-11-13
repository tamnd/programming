#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/copy-a-file/.

# copy_file SRC DST
# Copies the bytes and the permission bits of the file at SRC to DST, and
# replaces a file at DST. cp copies into a directory at DST, so the function
# returns status 1 for a directory at SRC or DST. It also returns status 1
# when SRC and DST name the same file. cp -p gives DST the permission bits of
# SRC also when DST exists; with no -p, cp keeps the bits of the old DST. -p
# also copies the times. -- stops options, so a name that starts with - is a
# file name.
copy_file() {
  if [[ -d $1 ]]; then
    printf 'copy_file: %s: is a directory\n' "$1" >&2
    return 1
  fi
  if [[ -d $2 ]]; then
    printf 'copy_file: %s: is a directory\n' "$2" >&2
    return 1
  fi
  if [[ -e $2 && $1 -ef $2 ]]; then
    printf 'copy_file: %s and %s are the same file\n' "$1" "$2" >&2
    return 1
  fi
  cp -p -- "$1" "$2" </dev/null
}
