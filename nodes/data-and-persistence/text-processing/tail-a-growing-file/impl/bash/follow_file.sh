#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/tail-a-growing-file/.

# A follower reads the lines that are added to a file, one poll at a time, as
# tail -F does. It keeps the file open on file descriptor 9, so it can read
# the rest of the file after a rename or a delete. The state is in global
# variables, so a shell has one follower, and follow_poll must run in the
# shell itself: call it as follow_poll > out, not as $(follow_poll) or in a
# pipeline. Bash cannot get the inode number of an open file, so the follower
# takes it from the path just after the open. A NUL byte in the file is lost.

_follow_path= _follow_open=0 _follow_inode= _follow_offset=0 _follow_pending=
follow_count=0 # the number of lines of the last poll

# Prints the inode number of the file at $1, or fails if no file is there.
_follow_inode_of() {
  local out
  out=$(ls -di -- "$1" 2>/dev/null) || return 1
  set -- $out
  printf '%s\n' "$1"
}

# follow_open PATH: opens the file at PATH and starts at its end. Fails if no
# file is at PATH.
follow_open() {
  local LC_ALL=C
  follow_close
  { exec 9<"$1"; } 2>/dev/null || {
    printf 'follow_open: %s: cannot open\n' "$1" >&2
    return 1
  }
  _follow_path=$1 _follow_open=1 _follow_pending=
  _follow_inode=$(_follow_inode_of "$1") || { follow_close; return 1; }
  # Read to the end of the file, and count the bytes.
  _follow_offset=$(cat <&9 | wc -c) || { follow_close; return 1; }
  _follow_offset=$((_follow_offset))
}

# follow_poll: prints the complete lines that were added since the last poll,
# each with an LF, and puts their number in follow_count.
follow_poll() {
  local LC_ALL=C now size
  follow_count=0
  if [ "$_follow_open" = 1 ]; then
    # Look at the path before the read. Then the read gets each byte that was
    # written before a rename or a delete.
    now=$(_follow_inode_of "$_follow_path") || now=
    if [ "$now" = "$_follow_inode" ]; then
      size=$(wc -c < "$_follow_path") || return 1
      if [ $((size)) -lt "$_follow_offset" ]; then
        # The file is shorter than the offset, so it was truncated. Bash
        # cannot seek, so the follower opens the file again.
        _follow_flush
        exec 9<"$_follow_path" || return 1
        _follow_offset=0
      fi
    fi
    _follow_read
    if [ "$now" != "$_follow_inode" ]; then
      # The file was deleted or renamed. Its rest is read.
      _follow_flush
      follow_close
    fi
  fi
  if [ "$_follow_open" = 0 ] && [ -n "$_follow_path" ]; then
    { exec 9<"$_follow_path"; } 2>/dev/null || return 0
    _follow_open=1 _follow_offset=0
    _follow_inode=$(_follow_inode_of "$_follow_path") || _follow_inode=
    _follow_read
  fi
}

# Prints each complete line from fd 9. read gets the bytes after the last LF
# as a line with no LF, and they wait in _follow_pending for the next poll.
_follow_read() {
  local line
  while IFS= read -r -u 9 line; do
    printf '%s%s\n' "$_follow_pending" "$line"
    _follow_offset=$((_follow_offset + ${#line} + 1))
    _follow_pending=
    follow_count=$((follow_count + 1))
  done
  _follow_offset=$((_follow_offset + ${#line}))
  _follow_pending=$_follow_pending$line
}

_follow_flush() {
  if [ -n "$_follow_pending" ]; then
    printf '%s\n' "$_follow_pending"
    _follow_pending=
    follow_count=$((follow_count + 1))
  fi
}

# follow_close: closes the file.
follow_close() {
  if [ "$_follow_open" = 1 ]; then
    exec 9<&-
    _follow_open=0
  fi
}

# follow COUNT [SECONDS]: polls the follower each SECONDS (1 if not given)
# and prints the new lines. Returns after COUNT lines or more.
follow() {
  local left=$1
  while :; do
    follow_poll || return 1
    left=$((left - follow_count))
    [ "$left" -gt 0 ] || return 0
    sleep "${2:-1}"
  done
}

# follow_file PATH INITIAL STEP...: writes INITIAL to PATH, opens a follower
# and does the steps. Prints N:LINE for each line of poll N.
follow_file() {
  local path=$1 old=$1.1 step text n=0 tmp rc=0
  printf '%s' "$2" > "$path" || return 1
  follow_open "$path" || return 1
  tmp=$(mktemp) || { follow_close; return 1; }
  shift 2
  for step; do
    case $step in
      *:*) text=${step#*:} ;;
      *) text= ;;
    esac
    case ${step%%:*} in
      poll) n=$((n + 1)) && follow_poll > "$tmp" && LC_ALL=C sed "s/^/$n:/" "$tmp" ;;
      append) printf '%s' "$text" >> "$path" ;;
      append-old) printf '%s' "$text" >> "$old" ;;
      truncate) printf '%s' "$text" > "$path" ;;
      rotate) mv -f -- "$path" "$old" && (set -C; : > "$path") ;;
      delete) rm -f -- "$path" ;;
      create) (set -C; printf '%s' "$text" > "$path") ;;
      *) printf 'follow_file: bad step: %s\n' "$step" >&2; false ;;
    esac || { rc=1; break; }
  done
  rm -f -- "$tmp"
  follow_close
  return "$rc"
}
