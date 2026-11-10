# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

# _date_at TEXT I SEP tells if a date YYYY<SEP>MM<SEP>DD with no digit next to
# it starts at byte I of TEXT. The groups are then in BASH_REMATCH. The quoted
# "$3" in the pattern matches SEP as a literal string. ERE has no lookaround,
# so the code checks the digit before and the digit after.
_date_at() {
  local text=$1 i=$2
  ((i == 0)) || [[ ${text:i-1:1} != [0-9] ]] || return 1
  [[ ${text:i} =~ ^([0-9]{4})"$3"([0-9]{2})"$3"([0-9]{2}) ]] || return 1
  [[ ${text:i+${#BASH_REMATCH[0]}:1} != [0-9] ]]
}

# reformat_dates TEXT FROM_SEP TO_SEP prints TEXT with each date
# YYYY<FROM_SEP>MM<FROM_SEP>DD changed to DD<TO_SEP>MM<TO_SEP>YYYY, with no
# newline at the end.
reformat_dates() {
  # In the C locale, offsets count bytes and [0-9] is ASCII only.
  local LC_ALL=C
  local text=$1 from=$2 to=$3 out="" i=0
  while ((i < ${#text})); do
    if _date_at "$text" "$i" "$from"; then
      out+=${BASH_REMATCH[3]}$to${BASH_REMATCH[2]}$to${BASH_REMATCH[1]}
      ((i += ${#BASH_REMATCH[0]}))
    else
      out+=${text:i:1}
      ((i += 1))
    fi
  done
  printf '%s' "$out"
}

# find_dates TEXT SEP prints "YYYY MM DD" for each date YYYY<SEP>MM<SEP>DD in
# TEXT, one per line.
find_dates() {
  local LC_ALL=C
  local text=$1 sep=$2 i=0
  while ((i < ${#text})); do
    if _date_at "$text" "$i" "$sep"; then
      printf '%s %s %s\n' "${BASH_REMATCH[1]}" "${BASH_REMATCH[2]}" "${BASH_REMATCH[3]}"
      ((i += ${#BASH_REMATCH[0]}))
    else
      ((i += 1))
    fi
  done
}
