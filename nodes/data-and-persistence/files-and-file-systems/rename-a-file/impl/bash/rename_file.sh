#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rename-a-file/.

# rename_file SRC DST
# Gives the file at SRC the name DST, and replaces a file at DST. mv is a
# rename call when the two paths are on one file system, but mv copies the
# file and deletes SRC when they are not, and a copy is not atomic. So the
# function compares the file systems of the two directories, with df -P, and
# returns status 1 if they are not the same. mv also moves SRC into DST when
# DST is a directory, so the function returns status 1 for a directory at
# DST. -f stops a prompt for a DST that is read-only. -- stops options, so a
# name that starts with - is a file name. If SRC and DST name the same file,
# rename does nothing, but mv gives an error, so the function returns 0.
rename_file() {
  local _fs
  [[ $1 -ef $2 && ! -L $1 && ! -L $2 ]] && return 0
  if [[ -d $2 && ! -L $2 ]]; then
    printf 'rename_file: %s: is a directory\n' "$2" >&2
    return 1
  fi
  if _fs=$(df -P -- "$(dirname -- "$1")" "$(dirname -- "$2")" 2>/dev/null); then
    if [[ $(awk 'NR == 2 { a = $NF } NR == 3 { b = $NF } END { print (a == b) }' <<<"$_fs") != 1 ]]; then
      printf 'rename_file: %s and %s are on different file systems\n' "$1" "$2" >&2
      return 1
    fi
  fi
  mv -f -- "$1" "$2"
}
