# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/measure-elapsed-time/.

# monotonic_ns prints a reading of the monotonic clock in nanoseconds. Bash has
# BASH_MONOSECONDS, which counts whole seconds, so the reading is a multiple
# of 10^9.
monotonic_ns() {
  printf '%s\n' "$((BASH_MONOSECONDS * 1000000000))"
}

# measure CLOCK COMMAND [ARG...] runs the command once and prints the time it
# took, in nanoseconds of CLOCK. CLOCK is a command that prints a reading,
# such as monotonic_ns. The output of the command goes to stderr, and its exit
# status is the status of measure.
measure() {
  local clock=$1 start end status
  shift
  start=$("$clock") || return
  "$@" >&2
  status=$?
  end=$("$clock") || return
  printf '%s\n' "$((end - start))"
  return "$status"
}

# elapsed_ms START_NS END_NS prints END_NS - START_NS as "M.UUU ms", with the
# rest under one microsecond dropped.
elapsed_ms() {
  # Bash arithmetic has integers only, so / drops the fraction.
  local ns=$(($2 - $1))
  printf '%d.%03d ms\n' "$((ns / 1000000))" "$((ns / 1000 % 1000))"
}
