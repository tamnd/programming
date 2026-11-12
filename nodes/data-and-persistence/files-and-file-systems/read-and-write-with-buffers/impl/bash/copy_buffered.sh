#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-with-buffers/.

# copy_buffered NAME SRC DST SIZE
# Copies the file SRC to DST with dd, in blocks of SIZE bytes, and stores the
# number of bytes copied in the variable NAME. dd reads one block and writes
# it before it reads the next one. Returns status 2 if SIZE is not a whole
# number of at least 1, and status 1 if dd fails, for example on a missing
# file or a full disk. NAME is not set on an error.
copy_buffered() {
  local _copy_buffered
  if [[ ! $4 =~ ^[0-9]+$ ]] || ((10#$4 < 1)); then
    printf 'copy_buffered: %s: size must be a whole number of at least 1\n' "$4" >&2
    return 2
  fi
  # dd writes its count to stderr; the last line starts with the number of
  # bytes, with GNU dd and with BSD dd.
  if ! _copy_buffered=$(dd if="$2" of="$3" bs="$((10#$4))" 2>&1); then
    printf '%s\n' "$_copy_buffered" >&2
    return 1
  fi
  _copy_buffered=${_copy_buffered##*$'\n'}
  printf -v "$1" '%s' "${_copy_buffered%% *}"
}
