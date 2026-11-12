#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-whole-file/.

# read_text NAME PATH
# Stores all of the text of the UTF-8 file PATH in the variable NAME, with no change.
# Returns a nonzero status, and does not set NAME, if PATH is missing, a directory
# or not valid UTF-8. A Bash variable cannot hold a 0 byte, so the text must have none.
read_text() {
  local _read_text
  if [[ -d $2 ]]; then
    printf 'read_text: %s: is a directory\n' "$2" >&2
    return 1
  fi
  # iconv stops with an error on a bad byte sequence or a missing file.
  # $(...) drops all final newlines, so add an x after the text, then remove it.
  _read_text=$(iconv -f UTF-8 -t UTF-8 -- "$2" && printf x) || return
  printf -v "$1" '%s' "${_read_text%x}"
}
