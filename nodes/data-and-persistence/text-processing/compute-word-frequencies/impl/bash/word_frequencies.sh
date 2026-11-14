#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-word-frequencies/.

# word_frequencies PATH N
# Prints the N most frequent words of the file at PATH, one "word count" line
# each, sorted by count from high to low, then by word. A word is a run of
# ASCII letters, changed to lower case. Returns a nonzero status, and prints
# nothing on stdout, if the file cannot be read.
word_frequencies() {
  [[ -r $1 && ! -d $1 ]] || { printf 'word_frequencies: cannot read %s\n' "$1" >&2; return 1; }
  # LC_ALL=C makes each command work on bytes, so A-Za-z is the 52 ASCII
  # letters, and sort compares bytes. tr -cs changes each run of bytes that
  # are not letters to one LF, so each word is on its own line. If the file starts with a byte
  # that is not a letter, the first line is empty, and grep removes it.
  # sort -k1,1nr -k2,2 sorts by count, then by word, so ties have one order.
  # awk keeps the first N lines and puts the word before the count.
  LC_ALL=C tr -cs 'A-Za-z' '\n' <"$1" |
    LC_ALL=C tr 'A-Z' 'a-z' |
    LC_ALL=C grep -v '^$' |
    LC_ALL=C sort |
    LC_ALL=C uniq -c |
    LC_ALL=C sort -k1,1nr -k2,2 |
    awk -v n="$2" 'NR > n { exit } { print $2, $1 }'
}
