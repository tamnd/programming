# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

# starts_with S PREFIX: exits with status 0 if S starts with PREFIX. The quotes
# around "$2" make PREFIX a literal string, and the * outside them matches the
# rest of S.
starts_with() {
  [[ $1 == "$2"* ]]
}

# ends_with S SUFFIX: exits with status 0 if S ends with SUFFIX.
ends_with() {
  [[ $1 == *"$2" ]]
}
