# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

# is_empty S: exits with status 0 if S is the empty string. The [[ keyword does
# not split words, so the test is correct also when S has spaces.
is_empty() {
  [[ -z $1 ]]
}

# is_blank S: exits with status 0 if each character of S is ASCII white space.
# The expansion deletes the six characters, and the test checks what is left.
is_blank() {
  [[ -z ${1//[$' \t\n\v\f\r']/} ]]
}
