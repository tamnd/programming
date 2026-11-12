# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-a-substring/.

# replace S OLD NEW: prints S with each match of OLD replaced by NEW. The
# quotes around "$2" make OLD a literal string, not a pattern. Bash 5.2 replaces
# an unquoted & in NEW with the match when the option patsub_replacement is on,
# and Bash 3.2 keeps quotes in NEW as text. So the function turns the option
# off in a subshell and leaves NEW unquoted, which works in both.
replace() {
  (
    shopt -u patsub_replacement 2>/dev/null
    local new=$3
    printf '%s' "${1//"$2"/$new}"
  )
}

# replace_first S OLD NEW: prints S with the first match of OLD replaced by NEW.
replace_first() {
  (
    shopt -u patsub_replacement 2>/dev/null
    local new=$3
    printf '%s' "${1/"$2"/$new}"
  )
}
