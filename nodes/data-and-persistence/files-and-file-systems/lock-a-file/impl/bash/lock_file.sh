#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/lock-a-file/.

# The flock command of util-linux takes flock locks from a shell. Linux has
# it; macOS and the BSDs do not have it by default.

# try_lock_file FD MODE
# Asks for a shared or an exclusive lock on the file open as descriptor FD,
# without a wait. It returns status 0 with the lock, and status 1 when another
# open file has a lock that conflicts. Close FD, or call unlock_file FD, to
# release the lock.
try_lock_file() {
  local fd=$1 mode=$2
  case $mode in
    shared) flock --nonblock --shared "$fd" ;;
    exclusive) flock --nonblock --exclusive "$fd" ;;
    *) printf 'try_lock_file: bad mode: %s\n' "$mode" >&2; return 2 ;;
  esac
}

# unlock_file FD releases the lock on descriptor FD.
unlock_file() {
  flock --unlock "$1"
}

# try_lock PATH HELD WANT
# The entry of the cases: takes HELD with one open of PATH, then asks for WANT
# with a second open. It prints true or false. It returns status 1 when PATH
# does not exist; the function does not make the file.
try_lock() {
  local path=$1 held=$2 want=$3 first second result=false
  if [[ ! -f $path ]]; then
    printf 'try_lock: %s: No such file\n' "$path" >&2
    return 1
  fi
  # {name}< opens the file read-only on a new descriptor; < does not make it.
  exec {first}<"$path"
  if [[ $held != none ]]; then try_lock_file "$first" "$held" || { exec {first}<&-; return 1; }; fi
  exec {second}<"$path"
  if try_lock_file "$second" "$want"; then result=true; fi
  exec {second}<&- {first}<&-
  printf '%s\n' "$result"
}
