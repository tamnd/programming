#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-an-environment-variable/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/get_env.sh"
}

check() { # check NAME DEFAULT WANT [KEY=VALUE]
  local name=$1 default=$2 want=$3
  unset -v "$name"
  if (($# > 3)); then export "$4"; fi
  run get_env "$name" "$default"
  [ "$status" -eq 0 ]
  [ "$output" = "$want" ]
}

@test "present" { check APP_PORT 80 8080 APP_PORT=8080; }
@test "path-value" { check APP_HOME /opt/app /srv/app APP_HOME=/srv/app; }
@test "missing" { check APP_PORT 80 80; }
@test "empty-value" { check APP_MODE production "" APP_MODE=; }
@test "empty-default" { check APP_MODE "" ""; }
@test "spaces" { check APP_TITLE none "  two  spaces  " "APP_TITLE=  two  spaces  "; }
@test "equals" { check APP_QUERY none key=value=more APP_QUERY=key=value=more; }
@test "newline" { check APP_TEXT none $'first\nsecond' $'APP_TEXT=first\nsecond'; }
@test "non-ascii" { check APP_GREETING none "xin chào, 日本語, 🙂" "APP_GREETING=xin chào, 日本語, 🙂"; }
@test "case" { check app_port 80 80 APP_PORT=8080; }
@test "inherited-name" { check toString none none; }

@test "the value comes from the environment of the process" {
  unset -v APP_PORT
  run env APP_PORT=8080 bash -c 'source "$1"; get_env APP_PORT 80' _ "$BATS_TEST_DIRNAME/get_env.sh"
  [ "$status" -eq 0 ]
  [ "$output" = 8080 ]
}
