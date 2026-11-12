# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/build-a-string-efficiently/.

# number_line N: prints the numbers 1 to N, separated by commas. The loop
# collects the numbers in an array, and "${parts[*]}" joins them with the
# first character of IFS. The IFS change is local to the function.
number_line() {
  local -a parts=()
  local i
  for ((i = 1; i <= $1; i++)); do
    parts+=("$i")
  done
  local IFS=,
  printf '%s' "${parts[*]}"
}
