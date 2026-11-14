#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-lines-from-a-file/.

# remove_lines PATH START COUNT
# Removes COUNT lines from the file at PATH, from line START, and prints how
# many it removed. Line 1 is the first line. A line ends at LF, and the LF goes
# with the line. If no line goes, the file is not written. Otherwise the rest
# of the file goes to a new file in the same directory, and mv puts it in the
# place of the old one. Returns 2 with a message if START is not 1 or more, if
# COUNT is not 0 or more, or if the file cannot be read or written.
remove_lines() {
  local file=$1 start=$2 count=$3 total removed tmp
  if ! [[ $start =~ ^[0-9]+$ && $count =~ ^[0-9]+$ ]] || ((10#$start < 1)); then
    printf 'remove_lines: bad range: start %s, count %s\n' "$start" "$count" >&2
    return 2
  fi
  # 10# reads 08 as eight, not as a bad octal number.
  start=$((10#$start)) count=$((10#$count))
  [[ -f $file && -r $file ]] || { printf 'remove_lines: cannot read %s\n' "$file" >&2; return 2; }
  # NR in the END action is the number of lines, with a last line with no LF.
  total=$(awk 'END { print NR }' <"$file") || return 2
  removed=$((total - start + 1))
  ((removed < 0)) && removed=0
  ((removed > count)) && removed=$count
  if ((removed == 0)); then
    echo 0
    return 0
  fi
  tmp=$(mktemp "$file.XXXXXX") || return 2
  # cp -p gives the new file the permissions of the old one, and > keeps them.
  # head and tail copy the bytes as they are, also a last line with no LF.
  if cp -p -- "$file" "$tmp" &&
    {
      # head -n 0 is not portable, so the code does not run it.
      if ((start > 1)); then head -n "$((start - 1))" -- "$file"; fi &&
        tail -n "+$((start + removed))" -- "$file"
    } >"$tmp" &&
    mv -f -- "$tmp" "$file"; then
    echo "$removed"
  else
    rm -f -- "$tmp"
    return 2
  fi
}
