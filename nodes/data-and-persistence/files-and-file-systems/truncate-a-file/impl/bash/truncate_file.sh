#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/truncate-a-file/.

# truncate_file PATH LENGTH
# Sets the size of the existing file at PATH to LENGTH bytes. POSIX has no
# truncate utility, so the function uses dd: with an empty input, seek= and
# no conv=notrunc, dd sets the size of the output file to the offset after
# the seek. That cuts the bytes after LENGTH, or adds zero bytes up to
# LENGTH. dd makes a missing file, so the function returns status 1 first
# for a missing file, for a directory and for a LENGTH that is not a
# whole number.
truncate_file() {
  if [[ ! $2 =~ ^[0-9]+$ ]]; then
    printf 'truncate_file: %s: invalid length\n' "$2" >&2
    return 1
  fi
  if [[ -d $1 ]]; then
    printf 'truncate_file: %s: is a directory\n' "$1" >&2
    return 1
  fi
  if [[ ! -e $1 ]]; then
    printf 'truncate_file: %s: no such file\n' "$1" >&2
    return 1
  fi
  dd if=/dev/null of="$1" bs=1 seek="$2" 2>/dev/null
}
