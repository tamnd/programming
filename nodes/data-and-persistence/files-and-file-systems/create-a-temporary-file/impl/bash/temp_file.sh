#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-temporary-file/.

# make_temp_file DIR PREFIX
# Makes a new, empty file in DIR whose name is PREFIX followed by six random
# characters, and prints its path. mktemp makes the file with the mode 0600,
# and only when no file has the name. The function returns status 2 when
# PREFIX has a slash, and status 1 when mktemp fails, for example when DIR
# does not exist.
make_temp_file() {
  local dir=$1 prefix=$2
  if [[ $prefix == */* ]]; then
    printf 'make_temp_file: invalid prefix: %s\n' "$prefix" >&2
    return 2
  fi
  mktemp "$dir/${prefix}XXXXXX"
}

# use_temp_file DIR PREFIX DATA
# Writes DATA to a new temporary file, prints the file back, and removes it.
# A trap is not used, because a trap on RETURN would stay set in the shell
# that sources this file; the function removes the file on each path.
use_temp_file() {
  local dir=$1 prefix=$2 data=$3 path status=0
  path=$(make_temp_file "$dir" "$prefix") || return
  printf '%s' "$data" >"$path" && cat -- "$path" || status=1
  rm -f -- "$path"
  return "$status"
}
