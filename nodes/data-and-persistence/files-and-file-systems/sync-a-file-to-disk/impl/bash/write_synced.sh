#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sync-a-file-to-disk/.

# write_synced PATH TEXT
# Writes TEXT to PATH with no newline at the end, and syncs the file and its
# directory. GNU sync syncs the files that it is
# given. The sync of macOS and BSD takes no file, so on those systems the
# function falls back to a sync of all file systems. It returns status 1 when
# the write or the sync fails.
write_synced() {
  local path=$1 text=$2 dir
  if [[ -d $path ]]; then
    printf 'write_synced: %s: Is a directory\n' "$path" >&2
    return 1
  fi
  printf '%s' "$text" > "$path" || return 1
  dir=$(dirname -- "$path")
  if sync --version >/dev/null 2>&1; then
    sync -- "$path" && sync -- "$dir"
  else
    sync
  fi
}
