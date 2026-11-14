#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-word-count/.

# word_count PATH
# Prints "LINES WORDS BYTES" for the UTF-8 file at PATH. LINES is the number of
# LF bytes, and a word is a run of one or more code points that are not
# White_Space. Returns a nonzero status, and prints nothing on stdout, if the
# file cannot be read or its bytes are not valid UTF-8.
word_count() {
  local lines words bytes
  # iconv stops with a nonzero status on bytes that are not valid UTF-8.
  iconv -f UTF-8 -t UTF-8 <"$1" >/dev/null || return
  lines=$(LC_ALL=C wc -l <"$1") || return
  bytes=$(LC_ALL=C wc -c <"$1") || return
  # wc -w splits at the white space of the locale, and locales do not agree on
  # it. So sed changes the White_Space code points above U+007F to a space, and
  # wc counts in the C locale, where white space is tab, LF, VT, FF, CR and
  # space. Each pattern is the UTF-8 bytes of the code points:
  # U+0085 U+00A0, U+1680, U+2000..U+200A U+2028 U+2029 U+202F, U+205F, U+3000.
  words=$(LC_ALL=C sed \
    -e $'s/\xc2[\x85\xa0]/ /g' \
    -e $'s/\xe1\x9a\x80/ /g' \
    -e $'s/\xe2\x80[\x80-\x8a\xa8\xa9\xaf]/ /g' \
    -e $'s/\xe2\x81\x9f/ /g' \
    -e $'s/\xe3\x80\x80/ /g' <"$1" | LC_ALL=C wc -w) || return
  # $((...)) also accepts the blanks that some wc programs print.
  printf '%d %d %d\n' "$((lines))" "$((words))" "$((bytes))"
}
