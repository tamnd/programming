#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/delete-a-file/.

# delete_file NAME PATH
# Deletes the file at PATH and stores true in the variable NAME if a file was
# there, false if not. rm with no -r does not delete a directory, and it
# deletes a symbolic link, not its target. Standard input comes from
# /dev/null, because rm asks before it deletes a read-only file when standard
# input is a terminal. -f is not used, because it hides a missing file. If rm
# fails and the entry is still there, the function writes the error of rm and
# returns status 1. If no entry is there, it looks at the nearest parent that
# is there: if that parent is a directory that the shell cannot search, the
# function returns status 1, because it cannot tell whether a file is there.
delete_file() {
  local _err _parent
  if _err=$(rm -- "$2" 2>&1 </dev/null); then
    printf -v "$1" '%s' true
    return 0
  fi
  if [[ -e $2 || -L $2 ]]; then
    printf '%s\n' "$_err" >&2
    return 1
  fi
  _parent=$2
  while [[ $_parent != / && $_parent != . ]]; do
    _parent=$(dirname -- "$_parent")
    [[ -e $_parent || -L $_parent ]] && break
  done
  if [[ -d $_parent && ! -x $_parent ]]; then
    printf 'delete_file: %s: cannot search the directory %s\n' "$2" "$_parent" >&2
    return 1
  fi
  printf -v "$1" '%s' false
}
