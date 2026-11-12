# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/change-the-case-of-a-string/.

# to_upper S: prints S with a to z changed to A to Z. The function uses tr
# with LC_ALL=C and the 26 letters, so it changes only the ASCII bytes, and it
# runs in each version of Bash.
to_upper() {
  printf '%s' "$1" | LC_ALL=C tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
}

# to_lower S: prints S with A to Z changed to a to z.
to_lower() {
  printf '%s' "$1" | LC_ALL=C tr 'ABCDEFGHIJKLMNOPQRSTUVWXYZ' 'abcdefghijklmnopqrstuvwxyz'
}
