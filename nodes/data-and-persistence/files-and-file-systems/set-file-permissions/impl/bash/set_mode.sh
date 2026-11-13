#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/set-file-permissions/.

# set_mode PATH MODE
# Sets the mode of the file at PATH to MODE, three or four octal digits, and
# prints the new permission bits as octal digits, such as 640. The chmod
# command also takes symbolic modes such as u+x, so the function checks MODE
# first, with a pattern that must match the whole string. POSIX has no stat
# utility, and the options of stat differ between GNU and BSD, so the function
# tries the GNU form and then the BSD form. chmod follows a symbolic link. The
# function returns status 2 for an invalid mode and 1 for a missing file.
set_mode() {
  local path=$1 mode=$2 bits
  if [[ ! $mode =~ ^[0-7]{3,4}$ ]]; then
    printf 'set_mode: invalid mode: %q\n' "$mode" >&2
    return 2
  fi
  chmod -- "$mode" "$path" || return 1
  bits=$(stat -L -c '%a' -- "$path" 2>/dev/null || stat -L -f '%Mp%Lp' -- "$path") || return 1
  printf '%04o\n' "$((8#$bits))" | sed 's/^0\([0-7]\{3\}\)$/\1/'
}
