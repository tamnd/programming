#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/watch-a-file-for-changes/.
# Watch a file for changes: poll its size and modification time.

# file_signature PATH: prints "SIZE MTIME" (the mtime in whole seconds), or
# nothing when the file does not exist. A directory is an error.
file_signature() {
  local path=$1
  if [ -d "$path" ]; then
    printf 'watch: %s: Is a directory\n' "$path" >&2
    return 1
  fi
  [ -e "$path" ] || return 0
  # GNU stat takes -c; the stat of macOS and BSD takes -f.
  stat -c '%s %Y' -- "$path" 2>/dev/null || stat -f '%z %m' -- "$path"
}

# watch_init PATH: takes the first signature into WATCH_LAST.
watch_init() {
  WATCH_PATH=$1
  WATCH_LAST=$(file_signature "$WATCH_PATH") || return 1
}

# watch_poll: sets WATCH_CHANGE to created, deleted, modified or none, for the
# time since the last poll. Do not call it in $( ): the new signature must stay
# in WATCH_LAST.
watch_poll() {
  local now
  now=$(file_signature "$WATCH_PATH") || return 1
  if [ -z "$WATCH_LAST" ] && [ -z "$now" ]; then
    WATCH_CHANGE=none
  elif [ -z "$WATCH_LAST" ]; then
    WATCH_CHANGE=created
  elif [ -z "$now" ]; then
    WATCH_CHANGE=deleted
  elif [ "$now" != "$WATCH_LAST" ]; then
    WATCH_CHANGE=modified
  else
    WATCH_CHANGE=none
  fi
  WATCH_LAST=$now
}

# watch_file PATH COMMAND [ARG...]: runs COMMAND once for each burst of
# changes, after the file is quiet for QUIET polls. INTERVAL is in seconds.
watch_file() {
  local path=$1 quiet=${QUIET:-2} interval=${INTERVAL:-1} pending=-1
  shift
  watch_init "$path" || return 1
  while :; do
    watch_poll || return 1
    if [ "$WATCH_CHANGE" != none ]; then
      pending=0
    elif [ "$pending" -ge 0 ]; then
      pending=$((pending + 1))
      if [ "$pending" -ge "$quiet" ]; then
        pending=-1
        "$@" "$path"
      fi
    fi
    sleep "$interval"
  done
}
