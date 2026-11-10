#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/generate-random-numbers/.
#
# Random integers in a range, a shuffle, a seeded generator and tokens.
# A word source is a function that sets REPLY to an integer from 0 to
# 2^32 - 1. It runs in the current shell, not in a subshell, so that it can
# keep its state in variables.

# int_in_range SOURCE LO HI: set REPLY to an integer from LO to HI, each with
# equal probability. A word below 2^32 % n is rejected, so that each remainder
# comes from the same number of words. Return 1 if HI < LO or the range has
# more than 2^32 values.
int_in_range() {
    local source=$1 lo=$2 hi=$3 n least
    # hi - lo can overflow, so compare hi with lo + 2^32 - 1 only where that
    # sum fits; above that point, no range can have more than 2^32 values.
    if ((hi < lo)) || { ((lo <= 9223372036854775807 - 4294967295)) && ((hi > lo + 4294967295)); }; then
        echo "int_in_range: need lo <= hi and at most 2^32 values" >&2
        return 1
    fi
    n=$((hi - lo + 1))
    least=$((4294967296 % n))
    while :; do
        "$source" || return 1
        if ((REPLY >= least)); then
            REPLY=$((lo + REPLY % n))
            return 0
        fi
    done
}

# shuffle_array SOURCE NAME: shuffle the indexed array NAME in place with the
# Fisher-Yates method. NAME must not be one of the local names of this
# function, because a name reference to a local of the same name is circular.
shuffle_array() {
    local source=$1 i j tmp
    local -n shuffle_array_items=$2
    for ((i = ${#shuffle_array_items[@]} - 1; i > 0; i--)); do
        int_in_range "$source" 0 "$i" || return 1
        j=$REPLY
        tmp=${shuffle_array_items[i]}
        shuffle_array_items[i]=${shuffle_array_items[j]}
        shuffle_array_items[j]=$tmp
    done
}

# seed_words SEED: seed RANDOM, so that seeded_word gives the same words for
# the same seed.
seed_words() {
    RANDOM=$1
}

# seeded_word: a word source. RANDOM gives 15 bits, so a word takes the bits
# of three values: 15 + 15 + 2.
seeded_word() {
    REPLY=$(((RANDOM << 17) | (RANDOM << 2) | (RANDOM & 3)))
}

# secure_word: a word source that reads 4 bytes from /dev/urandom.
secure_word() {
    local w
    w=$(od -An -N4 -tu4 /dev/urandom) || return 1
    REPLY=$((w))
}

# token_hex NBYTES: print NBYTES bytes from /dev/urandom as 2 * NBYTES
# lowercase hexadecimal digits.
token_hex() {
    local out
    if (($1 > 0)); then
        out=$(od -An -v -N"$1" -tx1 /dev/urandom) || return 1
    fi
    out=${out//[[:space:]]/}
    printf '%s\n' "$out"
}
