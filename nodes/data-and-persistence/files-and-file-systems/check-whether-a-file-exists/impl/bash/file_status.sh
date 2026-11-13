#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-file-exists/.

# file_status NAME PATH
# Stores what is at PATH in the variable NAME: file, directory, other,
# missing or broken-link. The tests -f, -d and -e follow symbolic links, and
# -L does not. Each test gives false also when it cannot tell, for example
# when a parent directory cannot be searched. So for a path that seems
# missing, the function looks at the nearest parent that is there. If that
# parent is a directory that the shell cannot search, it returns status 1 and
# does not set NAME.
file_status() {
  local _file_status _parent
  if [[ -f $2 ]]; then
    _file_status=file
  elif [[ -d $2 ]]; then
    _file_status=directory
  elif [[ -e $2 ]]; then
    _file_status=other
  elif [[ -L $2 ]]; then
    _file_status=broken-link
  else
    _parent=$2
    while [[ $_parent != / && $_parent != . ]]; do
      _parent=$(dirname -- "$_parent")
      [[ -e $_parent || -L $_parent ]] && break
    done
    if [[ -d $_parent && ! -x $_parent ]]; then
      printf 'file_status: %s: cannot search the directory %s\n' "$2" "$_parent" >&2
      return 1
    fi
    _file_status=missing
  fi
  printf -v "$1" '%s' "$_file_status"
}
