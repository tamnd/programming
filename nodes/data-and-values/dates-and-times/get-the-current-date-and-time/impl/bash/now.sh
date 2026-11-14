# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-current-date-and-time/.

# system_ns prints the system clock in nanoseconds since 1970-01-01T00:00:00Z.
# EPOCHREALTIME has microseconds, and its decimal point comes from the locale,
# so the code removes every character that is not a digit.
system_ns() {
  local us=${EPOCHREALTIME//[!0-9]/}
  printf '%s\n' "$((10#$us * 1000))"
}

# utc_timestamp NS prints the UTC date and time of NS nanoseconds since the
# epoch as YYYY-MM-DDTHH:MM:SS.mmmZ. It needs GNU date for -d @SECONDS.
utc_timestamp() {
  local ns=$1 ms secs date
  # The / of Bash rounds toward zero. Step back for a negative rest, so the
  # result goes toward the past.
  ms=$((ns / 1000000))
  if ((ns % 1000000 < 0)); then ((ms -= 1)); fi
  secs=$((ms / 1000))
  if ((ms % 1000 < 0)); then ((secs -= 1)); fi
  date=$(date -u -d "@$secs" +%Y-%m-%dT%H:%M:%S) || return
  printf '%s.%03dZ\n' "$date" "$((ms - secs * 1000))"
}

# now_utc [CLOCK] reads CLOCK once and prints the UTC date and time of the
# reading. CLOCK is a command that prints nanoseconds; system_ns is the real one.
now_utc() {
  local ns
  ns=$("${1:-system_ns}") || return
  utc_timestamp "$ns"
}
