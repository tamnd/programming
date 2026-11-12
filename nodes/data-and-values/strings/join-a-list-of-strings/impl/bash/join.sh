# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/join-a-list-of-strings/.

# join SEP ITEM...: prints the items with SEP between each two. "${*}" uses
# only the first character of IFS, so the function loops for a separator of
# any length.
join() {
  local sep=$1 out="" item
  shift
  if (($# > 0)); then
    out=$1
    shift
  fi
  for item in "$@"; do
    out+="$sep$item"
  done
  printf '%s' "$out"
}

# join_numbers SEP N...: prints the decimal form of each integer N, with SEP
# between each two. printf %d checks that each item is an integer.
join_numbers() {
  local sep=$1 n
  shift
  local -a parts=()
  for n in "$@"; do
    parts+=("$(printf '%d' "$n")")
  done
  join "$sep" ${parts[@]+"${parts[@]}"}
}
