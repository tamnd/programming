#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

# A shell script has no build step, so the text of lines.txt is in the
# script itself, as a here-document. The quoted delimiter stops parameter
# expansion and command substitution in the text, so a $ or a ` in the file
# stays as it is.
embedded_text() {
  cat <<'END_OF_LINES'
# SPDX-License-Identifier: MIT
alpha
café

gamma
END_OF_LINES
}

# embedded_line N
# Prints line N of the embedded text (1 is the line after the license
# header), with no line end.
# Returns status 1, and prints nothing, when there is no such line.
embedded_line() {
  local n=$1 i=-1 line
  [[ $n =~ ^-?[0-9]+$ ]] || return 1
  ((n >= 1)) || return 1
  while IFS= read -r line; do
    i=$((i + 1))
    if ((i == n)); then
      printf '%s' "$line"
      return 0
    fi
  done < <(embedded_text)
  return 1
}
