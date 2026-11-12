# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-for-a-substring/.

# find S SUB: prints the code point index of the first match of SUB in S, or
# -1. ${s%%"$sub"*} removes the longest suffix that starts with SUB, so the rest
# is the text before the first match. The quotes make SUB a literal string,
# not a pattern. The length runs in the locale C.UTF-8, where each code point
# is one character.
find() {
  (
    LC_ALL=C.UTF-8
    if [[ $1 != *"$2"* ]]; then
      echo -1
      return
    fi
    local before=${1%%"$2"*}
    echo "${#before}"
  )
}

# find_last S SUB: prints the code point index of the last match of SUB in S,
# or -1. ${s%"$sub"*} removes the shortest suffix that starts with SUB.
find_last() {
  (
    LC_ALL=C.UTF-8
    if [[ $1 != *"$2"* ]]; then
      echo -1
      return
    fi
    local before=${1%"$2"*}
    echo "${#before}"
  )
}
