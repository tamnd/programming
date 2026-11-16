#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-conditional/.

# grade SCORE
# Prints the letter grade for a score from 0 to 100.
grade() {
  local score=$1
  if ((score >= 90)); then
    printf '%s\n' A
  elif ((score >= 80)); then
    printf '%s\n' B
  elif ((score >= 70)); then
    printf '%s\n' C
  elif ((score >= 60)); then
    printf '%s\n' D
  else
    printf '%s\n' F
  fi
}
