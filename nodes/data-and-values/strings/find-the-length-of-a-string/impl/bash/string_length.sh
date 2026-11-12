# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-length-of-a-string/.

# lengths S: prints three lengths of the valid UTF-8 string S on one line: the
# number of bytes, the number of UTF-16 code units and the number of code
# points. ${#s} counts characters in the current locale, so the function sets
# the locale for each count.
lengths() {
  local s=$1 bytes code_points four_byte
  # In the C locale each byte is one character.
  bytes=$(LC_ALL=C; printf '%s' "${#s}")
  # In a UTF-8 locale each code point is one character.
  code_points=$(LC_ALL=C.UTF-8; printf '%s' "${#s}")
  # A first byte F0 to F4 starts a code point above U+FFFF, and UTF-16 needs
  # two code units for it. Keep only those bytes and count them.
  four_byte=$(LC_ALL=C; t=${s//[!$'\xf0'-$'\xf4']/}; printf '%s' "${#t}")
  printf '%d %d %d\n' "$bytes" $((code_points + four_byte)) "$code_points"
}
