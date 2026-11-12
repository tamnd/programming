# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

# characters S: sets the array CHARS to the code points of S. In the C locale,
# ${s:i:1} is one byte, so the loop finds the length of each UTF-8 sequence
# from its first byte. local LC_ALL=C keeps the change of locale inside the
# function.
characters() {
  local LC_ALL=C
  local s=$1 i=0 n b
  CHARS=()
  while ((i < ${#s})); do
    b=${s:i:1}
    n=1
    if [[ $b > $'\xef' ]]; then
      n=4
    elif [[ $b > $'\xdf' ]]; then
      n=3
    elif [[ $b > $'\x7f' ]]; then
      n=2
    fi
    CHARS+=("${s:i:n}")
    ((i += n))
  done
}
