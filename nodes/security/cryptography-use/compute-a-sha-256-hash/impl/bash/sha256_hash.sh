#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

# Bash has no SHA-256 of its own. These functions call sha256sum from GNU
# coreutils. It prints the digest, then the name of the input. The functions
# keep the text before the first space.

# sha256_text TEXT prints the SHA-256 of the bytes of TEXT, as they are. For
# the contract, TEXT holds UTF-8 bytes.
sha256_text() {
  local out
  # printf '%s' writes the bytes with no newline at the end. echo adds one.
  out=$(printf '%s' "$1" | sha256sum) || return
  printf '%s\n' "${out%% *}"
}

# sha256_file PATH prints the SHA-256 of the bytes of the file at PATH.
# sha256sum reads the file from the standard input, so the name in its output
# is "-" and does not depend on PATH.
sha256_file() {
  local out
  out=$(sha256sum <"$1") || return
  printf '%s\n' "${out%% *}"
}
