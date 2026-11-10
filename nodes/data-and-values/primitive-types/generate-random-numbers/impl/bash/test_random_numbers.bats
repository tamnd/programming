#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/generate-random-numbers/.

setup() {
    source "$BATS_TEST_DIRNAME/random_numbers.sh"
}

# fake_word: a word source that gives the words in FAKE in order.
fake_word() {
    ((next < ${#FAKE[@]})) || return 1
    REPLY=${FAKE[next]}
    next=$((next + 1))
}

# ints_in_range "WORDS" LO HI COUNT: print COUNT results, and fail if a word is
# left.
ints_in_range() {
    local -a out=()
    local k
    read -ra FAKE <<<"$1"
    next=0
    for ((k = 0; k < $4; k++)); do
        int_in_range fake_word "$2" "$3" || return 1
        out+=("$REPLY")
    done
    ((next == ${#FAKE[@]})) || return 1
    echo "${out[*]}"
}

# shuffle_items "WORDS" ITEM...: print the shuffled items, one on each line.
shuffle_items() {
    read -ra FAKE <<<"$1"
    shift
    next=0
    local -a items=("$@")
    shuffle_array fake_word items || return 1
    ((next == ${#FAKE[@]})) || return 1
    if ((${#items[@]} > 0)); then
        printf '%s\n' "${items[@]}"
    fi
}

@test "coin" { run ints_in_range "5 2 7" 0 1 3; [ "$status" -eq 0 ]; [ "$output" = "1 0 1" ]; }
@test "no-results" { run ints_in_range "" 1 6 0; [ "$status" -eq 0 ]; [ "$output" = "" ]; }
@test "die" { run ints_in_range "7 12 1000000 4294967295" 1 6 4; [ "$status" -eq 0 ]; [ "$output" = "2 1 5 4" ]; }
@test "negative-range" { run ints_in_range "100 2000000000 4294967294" -10 10 3; [ "$status" -eq 0 ]; [ "$output" = "6 -5 -8" ]; }
@test "one-value" { run ints_in_range "0 99" 5 5 2; [ "$status" -eq 0 ]; [ "$output" = "5 5" ]; }
@test "reject-low-words" { run ints_in_range "0 3 4" 1 6 1; [ "$status" -eq 0 ]; [ "$output" = "5" ]; }
@test "accept-least" { run ints_in_range "3 4 9" 0 5 2; [ "$status" -eq 0 ]; [ "$output" = "4 3" ]; }
@test "power-of-two" { run ints_in_range "0 4294967295" 0 7 2; [ "$status" -eq 0 ]; [ "$output" = "0 7" ]; }
@test "worst-case" { run ints_in_range "2147483646 2147483647" 0 2147483648 1; [ "$status" -eq 0 ]; [ "$output" = "2147483647" ]; }
@test "full-unsigned-32" { run ints_in_range "0 4294967295 123456789" 0 4294967295 3; [ "$status" -eq 0 ]; [ "$output" = "0 4294967295 123456789" ]; }
@test "full-signed-32" { run ints_in_range "0 2147483648 4294967295" -2147483648 2147483647 3; [ "$status" -eq 0 ]; [ "$output" = "-2147483648 0 2147483647" ]; }
@test "near-int64-max" { run ints_in_range "4294967295 6" 9223372036854775798 9223372036854775807 2; [ "$status" -eq 0 ]; [ "$output" = "9223372036854775803 9223372036854775804" ]; }
@test "near-int64-min" { run ints_in_range "4294967295 9" -9223372036854775808 -9223372036854775799 2; [ "$status" -eq 0 ]; [ "$output" = "-9223372036854775803 -9223372036854775799" ]; }
@test "wide-int64" { run ints_in_range "4294967295 0" 9223372032559808512 9223372036854775807 2; [ "$status" -eq 0 ]; [ "$output" = "9223372036854775807 9223372032559808512" ]; }

@test "shuffle empty" { run shuffle_items ""; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n')" ]; }
@test "shuffle one-item" { run shuffle_items "" "a"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "a")" ]; }
@test "shuffle two-items-swap" { run shuffle_items "0" "a" "b"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "b" "a")" ]; }
@test "shuffle two-items-keep" { run shuffle_items "1" "a" "b"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "a" "b")" ]; }
@test "shuffle five-items" { run shuffle_items "7 3 4 1" "a" "b" "c" "d" "e"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "a" "e" "b" "d" "c")" ]; }
@test "shuffle reject-in-shuffle" { run shuffle_items "0 2 0" "a" "b" "c"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "b" "a" "c")" ]; }
@test "shuffle duplicates" { run shuffle_items "1 1" "x" "x" "y"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "x" "y" "x")" ]; }
@test "shuffle non-ascii" { run shuffle_items "4294967295 4294967294 0" "é" "日本" "🙂" "a"; [ "$status" -eq 0 ]; [ "$output" = "$(printf '%s\n' "日本" "é" "🙂" "a")" ]; }

@test "bad ranges" {
    run int_in_range secure_word 1 0
    [ "$status" -eq 1 ]
    run int_in_range secure_word 0 4294967296
    [ "$status" -eq 1 ]
    run int_in_range secure_word -9223372036854775808 9223372036854775807
    [ "$status" -eq 1 ]
}

@test "the same seed gives the same words" {
    local a b c k
    seed_words 42
    for ((k = 0; k < 10; k++)); do seeded_word; a+=" $REPLY"; done
    seed_words 42
    for ((k = 0; k < 10; k++)); do seeded_word; b+=" $REPLY"; done
    seed_words 43
    for ((k = 0; k < 10; k++)); do seeded_word; c+=" $REPLY"; done
    [ "$a" = "$b" ]
    [ "$a" != "$c" ]
    for w in $a; do ((w >= 0 && w <= 4294967295)); done
}

@test "secure words give values in range" {
    local k
    for ((k = 0; k < 20; k++)); do
        int_in_range secure_word 1 6
        ((REPLY >= 1 && REPLY <= 6))
    done
}

@test "token_hex gives 2n lowercase hexadecimal digits" {
    for n in 0 1 16 32; do
        run token_hex "$n"
        [ "$status" -eq 0 ]
        [[ "$output" =~ ^[0-9a-f]*$ ]]
        [ "${#output}" -eq $((2 * n)) ]
    done
}

@test "tokens differ" {
    [ "$(token_hex 16)" != "$(token_hex 16)" ]
}
