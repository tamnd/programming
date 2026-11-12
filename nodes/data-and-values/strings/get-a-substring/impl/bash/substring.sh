# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-substring/.

# substring S START END: prints the code points of S from index START up to,
# not including, END. ${s:offset:length} counts characters of the locale, so
# the expansion runs in a subshell with the locale C.UTF-8, where each code
# point is one character.
substring() {
  (
    LC_ALL=C.UTF-8
    printf '%s' "${1:$2:$(($3 - $2))}"
  )
}
