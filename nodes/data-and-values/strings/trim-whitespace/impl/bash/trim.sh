# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/trim-whitespace/.

# trim S: prints S with the white space at the start and at the end removed.
# In the C locale, the class [:space:] is the 6 ASCII white space characters.
# The first expansion removes the longest prefix of white space, and the second
# removes the longest suffix.
trim() {
  (
    LC_ALL=C
    s=$1
    s=${s#"${s%%[![:space:]]*}"}
    s=${s%"${s##*[![:space:]]}"}
    printf '%s' "$s"
  )
}

# trim_start S: prints S with the white space at the start removed.
trim_start() {
  (
    LC_ALL=C
    s=$1
    s=${s#"${s%%[![:space:]]*}"}
    printf '%s' "$s"
  )
}
