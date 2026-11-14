# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sleep-for-a-duration/.

# sleep_ns NS: one call of sleep(1) for NS nanoseconds, written as seconds
# with nine decimal places. GNU sleep takes a number that is not an integer.
sleep_ns() {
  local ns=$1
  sleep "$(printf '%d.%09d' "$((ns / 1000000000))" "$((ns % 1000000000))")"
}

# sleep_requests DURATION_NS START_NS READING...: runs the loop of the
# contract on a fake clock that gives START_NS and then each READING, and
# prints the durations that the loop asks for, with one space between them.
sleep_requests() {
  local now=$2 deadline=$(($2 + $1)) requests=()
  shift 2
  while ((now < deadline)); do
    requests+=("$((deadline - now))")
    now=$1
    shift
  done
  echo "${requests[*]}"
}

# sleep_for_ns NS: sleeps for at least NS nanoseconds. Bash has no monotonic
# clock, so the function makes one call of sleep. A negative or zero NS
# returns at once.
sleep_for_ns() {
  local ns=$1
  if ((ns > 0)); then
    sleep_ns "$ns"
  fi
}
