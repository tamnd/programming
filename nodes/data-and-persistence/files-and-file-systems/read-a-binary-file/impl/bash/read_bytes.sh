#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-binary-file/.

# A Bash variable cannot hold a 0 byte, so these functions give the bytes as
# lowercase hexadecimal, two digits for each byte, made by od.

# read_bytes NAME PATH
# Stores the bytes of the file PATH in the variable NAME, as hexadecimal.
# Returns status 1, and does not set NAME, if PATH is missing or a directory.
read_bytes() {
  local _read_bytes
  if [[ -d $2 ]]; then
    printf 'read_bytes: %s: is a directory\n' "$2" >&2
    return 1
  fi
  # -An: no offsets; -v: no * for lines that repeat; -tx1: one byte at a time.
  _read_bytes=$(set -o pipefail; od -An -v -tx1 -- "$2" | tr -d ' \n') || return 1
  printf -v "$1" '%s' "$_read_bytes"
}

# read_exact NAME PATH N
# Stores the first N bytes of the file PATH in the variable NAME, as
# hexadecimal. Returns status 1 if PATH is missing or a directory, and
# status 2 if the file has fewer than N bytes. NAME is not set on an error.
read_exact() {
  local _read_exact=''
  if [[ -d $2 ]]; then
    printf 'read_exact: %s: is a directory\n' "$2" >&2
    return 1
  fi
  if (($3 > 0)); then
    # -N stops od after N bytes; a shorter file gives fewer digits.
    _read_exact=$(set -o pipefail; od -An -v -tx1 -N "$3" -- "$2" | tr -d ' \n') || return 1
  elif [[ ! -r $2 ]]; then
    printf 'read_exact: %s: cannot read the file\n' "$2" >&2
    return 1
  fi
  if ((${#_read_exact} < 2 * $3)); then
    printf 'read_exact: %s: %d of %d bytes before the end of the file\n' \
      "$2" $((${#_read_exact} / 2)) "$3" >&2
    return 2
  fi
  printf -v "$1" '%s' "$_read_exact"
}
