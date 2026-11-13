#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-symbolic-link/.

# make_symlink TARGET LINK
# Makes a symbolic link at LINK that holds TARGET, and prints the target that
# the link holds, read back with readlink. TARGET is stored as it is: it need
# not exist, and a relative target is read from the directory of LINK. When
# LINK names a directory, ln -s makes the link inside that directory, so the
# function checks LINK first. -e follows a link, so -L also checks for a
# dangling link. Another process can make LINK between the check and ln; then
# ln fails, or with a directory it makes the link inside it. The function
# returns status 1 when LINK exists or when ln fails.
make_symlink() {
  local target=$1 link=$2
  if [[ -e $link || -L $link ]]; then
    printf 'make_symlink: %s: File exists\n' "$link" >&2
    return 1
  fi
  ln -s -- "$target" "$link" || return 1
  readlink -- "$link"
}
