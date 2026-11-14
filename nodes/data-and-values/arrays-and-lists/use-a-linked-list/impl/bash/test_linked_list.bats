#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-linked-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/linked_list.sh"
}

@test "deque" { got=$(run_ops "push_back 1" "push_back 2" "push_front 0" "pop_back" "push_back 3") || return 1; [ "$got" = "0 1 3" ]; }
@test "remove-middle" { got=$(run_ops "push_back 1" "push_back 2" "push_back 3" "remove 2") || return 1; [ "$got" = "1 3" ]; }
@test "empty" { got=$(run_ops ) || return 1; [ "$got" = "" ]; }
@test "push-pop" { got=$(run_ops "push_back 1" "pop_front") || return 1; [ "$got" = "" ]; }
@test "single" { got=$(run_ops "push_front 7") || return 1; [ "$got" = "7" ]; }
@test "remove-only" { got=$(run_ops "push_back 5" "remove 5" "push_back 6") || return 1; [ "$got" = "6" ]; }
@test "remove-head" { got=$(run_ops "push_back 1" "push_back 2" "remove 1" "push_front 0") || return 1; [ "$got" = "0 2" ]; }
@test "remove-tail" { got=$(run_ops "push_back 1" "push_back 2" "remove 2" "push_back 3") || return 1; [ "$got" = "1 3" ]; }
@test "pop-both-ends" { got=$(run_ops "push_back 1" "push_back 2" "pop_front" "pop_back" "push_front 3") || return 1; [ "$got" = "3" ]; }
@test "remove-absent" { got=$(run_ops "push_back 1" "remove 9") || return 1; [ "$got" = "1" ]; }
@test "duplicates" { got=$(run_ops "push_back 4" "push_back 5" "push_back 4" "remove 4") || return 1; [ "$got" = "5 4" ]; }
@test "push-front-order" { got=$(run_ops "push_front 1" "push_front 2" "push_front 3") || return 1; [ "$got" = "3 2 1" ]; }
@test "negative" { got=$(run_ops "push_front -1" "push_back -2" "remove -1") || return 1; [ "$got" = "-2" ]; }
@test "limit" { got=$(run_ops "push_back 2147483647" "push_front -2147483648" "remove 2147483647" "push_back 2147483647") || return 1; [ "$got" = "-2147483648 2147483647" ]; }
@test "bad-operation" { if run_ops "insert 1" 2>/dev/null; then return 1; fi; }
@test "many" { local -a ops=(); local k; for k in $(seq 300); do ops+=("push_back $k"); done; got=$(run_ops "${ops[@]}") || return 1; [ "$(wc -w <<<"$got")" -eq 300 ]; [ "${got%% *}" = 1 ]; }
