#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-file/.

# write_text PATH TEXT MODE writes TEXT to PATH with no change to its bytes.
# MODE is truncate, append or atomic.
write_text() {
  local path=$1 text=$2 mode=$3 tmp
  case $mode in
    # printf '%s' writes the bytes as they are: no newline at the end, and
    # no change to a backslash, a % or a leading -n.
    truncate) printf '%s' "$text" >"$path" ;;
    append) printf '%s' "$text" >>"$path" ;;
    atomic)
      # mv onto a directory moves the file into it, so refuse a directory.
      if [ -d "$path" ]; then
        printf 'write_text: %s: is a directory\n' "$path" >&2
        return 1
      fi
      # The temporary file is in the same directory, so mv is a rename.
      tmp=$(mktemp "$path.XXXXXX") || return
      if printf '%s' "$text" >"$tmp" && mv -f "$tmp" "$path"; then
        return 0
      fi
      rm -f "$tmp"
      return 1
      ;;
    *)
      printf 'write_text: unknown mode %s\n' "$mode" >&2
      return 2
      ;;
  esac
}
