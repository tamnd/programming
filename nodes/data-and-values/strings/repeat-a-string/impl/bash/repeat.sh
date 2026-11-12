# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/repeat-a-string/.

# repeat S N: prints N copies of S. A brace expansion such as {1..$n} does not
# expand the variable, so the function uses a loop.
repeat() {
  local out='' i
  for ((i = 0; i < $2; i++)); do
    out+=$1
  done
  printf '%s' "$out"
}
