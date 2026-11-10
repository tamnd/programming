#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/split-a-string/.

setup() {
  source "$BATS_TEST_DIRNAME/split.sh"
}

# expect FIELD...: checks that FIELDS holds exactly the given fields.
expect() {
  local want=("$@")
  if [[ ${#FIELDS[@]} -ne ${#want[@]} ]]; then
    echo "got ${#FIELDS[@]} fields: $(declare -p FIELDS)" >&2
    return 1
  fi
  local i
  for i in "${!want[@]}"; do
    if [[ ${FIELDS[i]} != "${want[i]}" ]]; then
      echo "field $i: got '${FIELDS[i]}', want '${want[i]}'" >&2
      return 1
    fi
  done
}

@test "comma" { split_string 'a,b,c' ',' 0; expect a b c; }
@test "csv-line" { split_string 'Ada,1815,London' ',' 0; expect Ada 1815 London; }
@test "multi-char-separator" { split_string 'a::b::c' '::' 0; expect a b c; }
@test "space-separator" { split_string 'one two  three' ' ' 0; expect one two '' three; }
@test "limit-two" { split_string 'key=value=more' '=' 2; expect key value=more; }
@test "empty" { split_string '' ',' 0; expect ''; }
@test "no-separator" { split_string 'abc' ',' 0; expect abc; }
@test "leading-separator" { split_string ',a,b' ',' 0; expect '' a b; }
@test "trailing-separators" { split_string 'a,b,,' ',' 0; expect a b '' ''; }
@test "adjacent-separators" { split_string 'a,,b' ',' 0; expect a '' b; }
@test "only-separator" { split_string ',' ',' 0; expect '' ''; }
@test "overlapping-match" { split_string 'aaa' 'aa' 0; expect '' a; }
@test "dot-separator" { split_string '192.168.0.1' '.' 0; expect 192 168 0 1; }
@test "pipe-separator" { split_string 'a|b|c' '|' 0; expect a b c; }
@test "non-ascii" { split_string 'día→naïve→日本→🙂' '→' 0; expect día naïve 日本 🙂; }
@test "limit-one" { split_string 'a,b,c' ',' 1; expect a,b,c; }
@test "limit-above-count" { split_string 'a,b' ',' 5; expect a b; }
@test "limit-keeps-trailing-empty" { split_string 'a,b,' ',' 3; expect a b ''; }
@test "glob characters are literal" { split_string 'a*b?c' '*' 0; expect a 'b?c'; }

@test "whitespace single-spaces" { split_whitespace 'a b c'; expect a b c; }
@test "whitespace runs-and-ends" { split_whitespace '  lead  and trail  '; expect lead and trail; }
@test "whitespace tabs-and-newlines" { split_whitespace $'a\t\tb\nc\n'; expect a b c; }
@test "whitespace one-word" { split_whitespace 'word'; expect word; }
@test "whitespace empty" { split_whitespace ''; expect; }
@test "whitespace only-white-space" { split_whitespace $' \t\n '; expect; }

@test "regex comma-or-semicolon" { split_regex 'a, b;c ,d' '[ ]*[,;][ ]*'; expect a b c d; }
@test "regex digit-runs" { split_regex 'a1b22c333' '[0-9]+'; expect a b c ''; }
@test "regex adjacent-matches" { split_regex 'a,;b' '[,;]'; expect a '' b; }
@test "regex leading-and-trailing" { split_regex ';a;' ';'; expect '' a ''; }
@test "regex empty" { split_regex '' '[,;]'; expect ''; }
@test "regex no-match" { split_regex 'abc' '[0-9]+'; expect abc; }
