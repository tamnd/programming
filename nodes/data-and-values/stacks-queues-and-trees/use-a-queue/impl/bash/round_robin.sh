#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-queue/.

# finish_order SLICE WORK...
# Prints the job numbers in the order in which round-robin finishes them, on
# one line. The queue is an indexed array with a head index: q+=(x) adds at
# the back, and the head index moves forward to dequeue.
finish_order() {
  local slice=$1 job head=0 run
  shift
  local -a left=("$@") q=() finished=()
  for ((job = 0; job < $#; job++)); do
    q+=("$job")
  done
  while ((head < ${#q[@]})); do
    job=${q[head]}
    head=$((head + 1))
    run=$((left[job] < slice ? left[job] : slice))
    left[job]=$((left[job] - run))
    if ((left[job] == 0)); then
      finished+=("$job")
    else
      q+=("$job")
    fi
  done
  printf '%s\n' "${finished[*]}"
}
