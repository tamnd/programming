#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-record-type/.

# birthday NAME PERSON...
# Each PERSON is the text "name|age". Prints one line "name|age" for each
# person, in the same order. Each person whose name is equal to NAME is one
# year older.
birthday() {
  # Bash has no record type, so a record is one string with a separator.
  # The | must not be in a name.
  local who=$1 p name age
  shift
  for p in "$@"; do
    name=${p%|*}
    age=${p##*|}
    if [[ $name == "$who" ]]; then
      age=$((age + 1))
    fi
    printf '%s|%s\n' "$name" "$age"
  done
}
