# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/split-a-string/.

# split_string S SEP [LIMIT]: splits S on the literal separator SEP, which must not
# be empty, and keeps the empty fields. With LIMIT n > 0 there are at most n
# fields, and the last field holds the rest of S. The fields go into the
# global array FIELDS.
split_string() {
  local s=$1 sep=$2 limit=${3:-0}
  FIELDS=()
  # The quotes around $sep make it a literal string, not a pattern.
  while [[ $s == *"$sep"* ]] && ((limit == 0 || ${#FIELDS[@]} + 1 < limit)); do
    FIELDS+=("${s%%"$sep"*}")
    s=${s#*"$sep"}
  done
  FIELDS+=("$s")
}

# split_whitespace S: splits S on runs of space, tab and LF. There are no
# empty fields. The fields go into the global array FIELDS.
split_whitespace() {
  FIELDS=()
  # read returns 1 at the end of the input, but it still fills the array.
  IFS=$' \t\n' read -r -d '' -a FIELDS <<<"$1" || true
}

# split_regex S PATTERN: splits S on each match of the POSIX extended regular
# expression PATTERN, which must not match the empty string. The fields go
# into the global array FIELDS.
split_regex() {
  local s=$1 re=$2 match
  FIELDS=()
  while [[ $s =~ $re ]] && [[ -n ${BASH_REMATCH[0]} ]]; do
    match=${BASH_REMATCH[0]}
    # The first copy of the matched text is the leftmost match.
    FIELDS+=("${s%%"$match"*}")
    s=${s#*"$match"}
  done
  FIELDS+=("$s")
}
