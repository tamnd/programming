#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

# unique
# Reads items from stdin, one per line. Prints each item the first time it
# comes, so the first copies keep their order. No item can hold a newline.
unique() {
  # seen[$0] is 0 the first time, so !seen[$0]++ is true only then.
  # A pattern with no action prints the line.
  awk '!seen[$0]++'
}

# unique_by_email
# Reads users from stdin, one per line: NAME and EMAIL with a tab between them.
# Prints the first user for each email. Case does not matter in the emails.
unique_by_email() {
  # LC_ALL=C: tolower changes only A to Z, on every machine.
  LC_ALL=C awk -F '\t' '!seen[tolower($2)]++'
}
