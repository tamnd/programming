#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-word-frequencies/.

# top_words TEXT N prints the N most frequent words of TEXT, one "word count"
# line each, by count from high to low, then by word in byte order.
top_words() {
  local count word
  # LC_ALL=C makes tr, sort and uniq work on bytes. Each byte of a multibyte
  # UTF-8 sequence is in \200-\377, so it stays in its word, and the byte order
  # of UTF-8 is code point order.
  printf '%s\n' "$1" |
    LC_ALL=C tr -c 'A-Za-z0-9\200-\377' '\n' |
    LC_ALL=C tr 'A-Z' 'a-z' |
    LC_ALL=C grep -v '^$' |
    LC_ALL=C sort |
    LC_ALL=C uniq -c |
    LC_ALL=C sort -k1,1nr -k2,2 |
    head -n "$2" |
    while read -r count word; do
      printf '%s %s\n' "$word" "$count"
    done
}
