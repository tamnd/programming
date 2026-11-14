#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-set/.

# toggle WORD...
# Adds each word that is not in the set, and removes each word that is in it.
# Prints the words of the set, one on each line, in ascending order by bytes.
toggle() {
  # Bash has no set type. The keys of an associative array are the set.
  local -A seen=()
  local word
  for word in "$@"; do
    if [[ -v seen[$word] ]]; then
      # The quotes stop the shell from reading seen[...] as a glob pattern.
      unset 'seen[$word]'
    else
      seen[$word]=1
    fi
  done
  if ((${#seen[@]} > 0)); then
    printf '%s\n' "${!seen[@]}" | LC_ALL=C sort
  fi
}
