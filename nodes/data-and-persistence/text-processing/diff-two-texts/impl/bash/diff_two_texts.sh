#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/diff-two-texts/.

# diff_files OLD_PATH NEW_PATH
# Prints the edit script that changes the lines of the file OLD_PATH into the
# lines of the file NEW_PATH. Each line of the script is a space and a line
# that stays, a - and a line of OLD_PATH that goes, or a + and a line of
# NEW_PATH that comes. The script has the smallest number of - and + lines.
# When two scripts are equally short, a - comes before a +. Returns a nonzero
# status, and prints nothing on stdout, if a file cannot be read.
diff_files() {
  local path
  for path in "$1" "$2"; do
    [[ -r $path && ! -d $path ]] || { printf 'diff_files: cannot read %s\n' "$path" >&2; return 1; }
  done
  # LC_ALL=C compares the bytes of the lines.
  local LC_ALL=C
  local -a a b lcs out
  # mapfile -t removes the LF of each line and keeps a CR before it.
  mapfile -t a <"$1"
  mapfile -t b <"$2"
  local n=${#a[@]} m=${#b[@]}
  local w=$((m + 1)) i j down right
  # lcs[i*w+j] is the length of a longest common subsequence of the lines
  # from a[i] and from b[j]. An unset element of row n or column m is 0.
  for ((i = n - 1; i >= 0; i--)); do
    for ((j = m - 1; j >= 0; j--)); do
      if [[ ${a[i]} == "${b[j]}" ]]; then
        lcs[i * w + j]=$((${lcs[(i + 1) * w + j + 1]:-0} + 1))
      else
        down=${lcs[(i + 1) * w + j]:-0} right=${lcs[i * w + j + 1]:-0}
        lcs[i * w + j]=$((down >= right ? down : right))
      fi
    done
  done
  i=0 j=0
  while ((i < n && j < m)); do
    if [[ ${a[i]} == "${b[j]}" ]]; then
      out+=(" ${a[i]}")
      ((i += 1, j += 1))
    elif ((${lcs[(i + 1) * w + j]:-0} >= ${lcs[i * w + j + 1]:-0})); then
      out+=("-${a[i]}")
      ((i += 1))
    else
      out+=("+${b[j]}")
      ((j += 1))
    fi
  done
  for ((; i < n; i++)); do out+=("-${a[i]}"); done
  for ((; j < m; j++)); do out+=("+${b[j]}"); done
  ((${#out[@]} == 0)) || printf '%s\n' "${out[@]}"
}
