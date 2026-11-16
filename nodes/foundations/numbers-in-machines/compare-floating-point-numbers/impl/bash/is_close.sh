#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-floating-point-numbers/.

# is_close A B REL_TOL ABS_TOL
# Prints true if A and B are close, and false otherwise:
# |A - B| <= max(REL_TOL * max(|A|, |B|), ABS_TOL).
# Bash arithmetic has integers only, so the function gives the numbers to
# awk, which computes with floats.
is_close() {
  awk -v a="$1" -v b="$2" -v rel="$3" -v abst="$4" 'BEGIN {
    a += 0; b += 0; rel += 0; abst += 0
    if (a == b) { print "true"; exit }
    d = a - b; if (d < 0) d = -d
    m = (a < 0 ? -a : a); n = (b < 0 ? -b : b); if (n > m) m = n
    tol = rel * m; if (abst > tol) tol = abst
    print (d <= tol ? "true" : "false")
  }'
}
