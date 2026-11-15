---
tag: 3CK24T
kind: tool
slug: bats
name: Bats
aliases: [bats-core, Bash Automated Testing System, bats-assert]
area: testing/test-frameworks
level: advanced
summary: >-
  Bats, the Bash Automated Testing System, is a TAP-compliant test framework for Bash. A test is a block of shell commands, and the test passes when every command exits with status 0.
role: [test-framework]
licence: mit
repo: https://github.com/bats-core/bats-core
homepage: https://bats-core.readthedocs.io
see_also: [test-framework, unit-testing, pester, xunit, test-driven-development, pytest, go-test]
sources:
  - url: https://github.com/bats-core/bats-core
    title: bats-core, README
    locator: the introduction; Background
    accessed: 2026-11-15
  - url: https://github.com/bats-core/bats-core/blob/master/LICENSE.md
    title: bats-core, LICENSE.md
    locator: the whole file
    accessed: 2026-11-15
  - url: https://bats-core.readthedocs.io/en/stable/usage.html
    title: bats-core documentation, Usage
    locator: the options; Running tests
    accessed: 2026-11-15
  - url: https://bats-core.readthedocs.io/en/stable/installation.html
    title: bats-core documentation, Installation
    locator: MacOS, Homebrew; Any OS, npm; Installing Bats from source
    accessed: 2026-11-15
  - url: https://bats-core.readthedocs.io/en/stable/writing-tests.html
    title: bats-core documentation, Writing tests
    locator: run; skip; setup and teardown; bats_require_minimum_version; Printing to the terminal; Special variables; Tagging tests
    accessed: 2026-11-15
  - url: https://github.com/bats-core/bats-core/blob/master/docs/CHANGELOG.md
    title: bats-core, CHANGELOG
    locator: 1.14.0; 1.7.0; 1.5.0; 1.0.0
    accessed: 2026-11-15
  - url: https://bats-core.readthedocs.io/en/stable/gotchas.html
    title: bats-core documentation, Gotchas
    locator: the whole page
    accessed: 2026-11-15
  - url: https://github.com/bats-core/bats-assert
    title: bats-assert, README
    locator: Install; Usage; assert_output
    accessed: 2026-11-15
  - url: https://github.com/bats-core/bats-core/releases
    title: bats-core releases on GitHub
    locator: v1.14.0
    accessed: 2026-11-15
---

## What it is

Bats is "a TAP-compliant testing framework for Bash 3.2 or above".[^1] It is most useful to test software in Bash, but it can test any UNIX program.[^1] A Bats test file is a Bash script with a special syntax for test cases, and each test case is a function with a description.[^1] Its licence is the MIT License.[^2]

A test case is a list of ordinary shell commands. Bats runs each test case with the Bash option `errexit` (`set -e`). If every command in the test case exits with status 0, the test passes. So each line is an assertion.[^1] Bats writes its results in the Test Anything Protocol (TAP) when its output is not a terminal, for example in a CI job.[^3]

The project bats-core is a fork of the original Bats by Sam Stephenson. The fork was made on 19 September 2017, because the development of the original project had stalled, and the people who called for new maintainers could not get write access to it. The original repository was archived on 29 April 2021.[^1][^2]

## Install and first use

Bats is in Homebrew as `bats-core` and in npm as `bats`. It can also be installed from a clone of the repository with `install.sh`.[^4]

```text
brew install bats-core
npm install -g bats
```

This script has a function `slugify`, which makes the slug of a web address from a title:

```bash norun reason=fragment group=first file=slug.sh
# slugify prints the slug of a title: lower case, with hyphens between words.
slugify() {
  if (( $# != 1 )); then
    echo "usage: slugify TITLE" >&2
    return 2
  fi
  local s=${1,,}
  s=${s//[^a-z0-9]/-}
  while [[ $s == *--* ]]; do
    s=${s//--/-}
  done
  s=${s#-}
  printf '%s\n' "${s%-}"
}
```

The test file `slug.bats` loads the script in `setup`, which runs before each test. The last test has a wrong expectation, to show a failure:

```bash norun reason=fragment group=first file=slug.bats
bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/slug.sh"
}

@test "joins the words with hyphens" {
  run slugify "Hello, World"
  [ "$status" -eq 0 ]
  [ "$output" = "hello-world" ]
}

@test "removes the hyphens at the ends" {
  run -0 slugify "  Bash: Tips & Tricks!  "
  [ "$output" = "bash-tips-tricks" ]
}

@test "needs one argument" {
  run -2 --separate-stderr slugify
  [ "$output" = "" ]
  [ "$stderr" = "usage: slugify TITLE" ]
}

@test "spells out the plus signs" {
  run slugify "C++ Primer"
  [ "$output" = "c-plus-plus-primer" ]
}
```

The helper `run` runs a command, saves its exit status in `$status` and its output in `$output`, and then returns 0, so the test can check the results.[^5] With `-N` as its first argument, `run` also checks that the exit status is N. With `--separate-stderr`, it puts the standard error in `$stderr`.[^5] These flags came in Bats 1.5.0, and without the line `bats_require_minimum_version 1.5.0`, Bats gives the warning BW02.[^6] The function `bats_require_minimum_version` came in Bats 1.7.0.[^5]

The site runs this script, which calls Bats with TAP output and prints the exit status:

```bash run group=first file=main.sh id=first-use
bats --tap --print-output-on-failure slug.bats
echo "exit status: $?"
```

```output
1..4
ok 1 joins the words with hyphens
ok 2 removes the hyphens at the ends
ok 3 needs one argument
not ok 4 spells out the plus signs
# (in test file slug.bats, line 26)
#   `[ "$output" = "c-plus-plus-primer" ]' failed
# Last output:
# c-primer
exit status: 1
```

A failed test has the line `not ok`, then the file, the line and the command that failed. The option `--print-output-on-failure` also prints the value of `$output` of the failed test.[^3] If any test fails, `bats` exits with status 1.[^3] The outputs on this page come from Bats 1.11.1.

## Concepts of the tool

### Hooks, temporary directories and skips

The functions `setup` and `teardown` run before and after each test case. `setup_file` and `teardown_file` run once for each test file, and `setup_suite` and `teardown_suite` run once for the whole run.[^5] `setup` and `teardown` also run for a skipped test.[^5] `$BATS_TEST_TMPDIR` is a temporary directory for one test, and `$BATS_FILE_TMPDIR` is one for all the tests of a file.[^5]

Output of a test to stdout or stderr is shown only when the test fails. To print text in every run, a test writes it to file descriptor 3. Each line should start with `#`, so that the output stays valid TAP.[^5]

```bash norun reason=fragment group=hooks file=hooks.bats
bats_require_minimum_version 1.5.0

setup_file() {
  echo "# setup_file" >&3
}

setup() {
  echo "# setup: $BATS_TEST_DESCRIPTION" >&3
  cd "$BATS_TEST_TMPDIR" || return 1
  printf 'pear\napple\nfig\n' > fruit.txt
}

teardown() {
  echo "# teardown" >&3
}

@test "sort puts apple first" {
  run sort fruit.txt
  [ "${lines[0]}" = "apple" ]
  [ "${#lines[@]}" -eq 3 ]
}

@test "grep fails when no line matches" {
  run ! grep -q kiwi fruit.txt
}

# bats test_tags=network
@test "downloads the price list" {
  skip "this run has no network"
  false
}
```

The array `lines` holds the lines of the output.[^5] `run !` expects a non-zero exit status. A bare `! command` does not fail the test unless it is the last command, because Bash does not apply `set -e` to a command with `!`.[^5][^7] The comment `# bats test_tags=network` puts a tag on the next test, and the option `--filter-tags` selects tests by tag. A tag with `!` in front excludes the tests with that tag.[^5]

```bash run group=hooks file=main.sh id=hooks
bats --tap hooks.bats
echo "--- without the tag network"
bats --tap --filter-tags '!network' hooks.bats
```

```output
1..3
# setup_file
# setup: sort puts apple first
# teardown
ok 1 sort puts apple first
# setup: grep fails when no line matches
# teardown
ok 2 grep fails when no line matches
# setup: downloads the price list
# teardown
ok 3 downloads the price list # skip this run has no network
--- without the tag network
1..2
# setup_file
# setup: sort puts apple first
# teardown
ok 1 sort puts apple first
# setup: grep fails when no line matches
# teardown
ok 2 grep fails when no line matches
```

### Helper libraries

The command `load` sources a helper file and adds the suffix `.bash` to its name. To load a `.sh` file, a test uses `source`.[^7] The library bats-assert adds assertion functions, such as `assert_success`, `assert_output` and `assert_line`, and it needs the library bats-support.[^8] On a failure, `assert_output` prints the expected output and the actual output:[^8]

```text
@test 'assert_output()' {
  run echo 'have'
  assert_output 'want'
}

-- output differs --
expected : want
actual   : have
--
```

### Common mistakes

The documentation lists some traps:[^7]

- `run` always succeeds. The test must check `$status`, or call the command without `run`.
- `run` runs the command in a subshell, so a variable that the command sets is not visible after `run`.
- In `run command | jq ...`, Bash parses the pipe outside of `run`. The helper `bats_pipe` is for pipes.[^5]
- A `for` loop cannot register a test many times, because Bats turns each `@test` into a function.
- Bats does not pass parameters to a test file. Environment variables are the workaround.

## Common tasks

```text
bats test/                                  run the .bats files in a directory
bats -r test/                               also run the files in subdirectories
bats --filter 'slug' test/                  run only the tests whose names match a regular expression
bats --filter-tags '!network' test/         exclude the tests with a tag
bats --formatter junit test/                write a JUnit XML report
bats --jobs 4 test/                         run tests in parallel (needs GNU parallel or rush)
bats --count test/                          count the tests and do not run them
bats -x test/                               print each command as it runs, like set -x
```

The options come from the usage page.[^3] Without `-r`, Bats runs only the `.bats` files at the top level of a directory.[^3]

## Configuration

The test files control Bats with `bats_require_minimum_version` and the tag comments, and the command line controls the rest.[^5][^3] The option `--report-formatter` writes a report file in addition to the normal output, and `--output` gives its directory.[^3]

## Alternatives

- [[pester]]: the test framework of PowerShell, with blocks and mocks.
- A shell script that runs commands and checks their exit status: this needs no tool, but it gives no TAP report and no isolation between tests.

## History and versions

| Version | Change |
|---|---|
| 1.0.0, 8 June 2018 | compatible with Bats 0.4.0, with better Bash compatibility and a large gain in speed[^6] |
| 1.5.0, 22 October 2021 | `run -N` and `run !`; `--separate-stderr`; `--print-output-on-failure`; `-x`[^6] |
| 1.7.0, 14 May 2022 | `bats_require_minimum_version`; `setup_suite` and `teardown_suite`; the warning BW02 for flags of `run` without the version guard[^6] |
| 1.14.0, 21 July 2026 | `run` honors `set -e` in the functions it calls; an error when no tests are found, with `--allow-empty-suite` for the old behavior[^6] |

The latest version on 12 November 2026 was 1.14.0, from 21 July 2026.[^9]

## References
