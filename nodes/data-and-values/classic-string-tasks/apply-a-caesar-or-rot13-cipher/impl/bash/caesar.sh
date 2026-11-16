#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

# caesar text shift: print text with each letter A to Z and a to z moved
# shift places along the alphabet, from z back to a. Other characters do not
# change. A negative shift decodes, and caesar text 13 is ROT13. tr maps the
# alphabet to the rotated alphabet. LC_ALL=C makes tr work on bytes, so the
# bytes of UTF-8 characters outside ASCII stay as they are.
caesar() {
  local upper=ABCDEFGHIJKLMNOPQRSTUVWXYZ lower=abcdefghijklmnopqrstuvwxyz
  local k=$(((($2 % 26) + 26) % 26))
  printf '%s\n' "$1" | LC_ALL=C tr 'A-Za-z' "${upper:k}${upper:0:k}${lower:k}${lower:0:k}"
}
