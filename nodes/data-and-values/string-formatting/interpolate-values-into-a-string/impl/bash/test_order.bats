#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

setup() {
  source "$BATS_TEST_DIRNAME/order.sh"
}

@test "three-items" {
  run describe_order 'Ada' 3 1.5
  [ "$output" = 'Order {Ada}: 3 items, total 4.50' ]
}

@test "one-item" {
  run describe_order 'Grace' 1 19.99
  [ "$output" = 'Order {Grace}: 1 item, total 19.99' ]
}

@test "many-items" {
  run describe_order 'Linus' 12 0.25
  [ "$output" = 'Order {Linus}: 12 items, total 3.00' ]
}

@test "zero-items" {
  run describe_order 'Ada' 0 9.99
  [ "$output" = 'Order {Ada}: 0 items, total 0.00' ]
}

@test "one-item-plural" {
  run describe_order 'Bob' 1 0.5
  [ "$output" = 'Order {Bob}: 1 item, total 0.50' ]
}

@test "empty-name" {
  run describe_order '' 2 1
  [ "$output" = 'Order {}: 2 items, total 2.00' ]
}

@test "braces-in-name" {
  run describe_order '{x}' 2 1.25
  [ "$output" = 'Order {{x}}: 2 items, total 2.50' ]
}

@test "percent-in-name" {
  run describe_order '50% off' 4 2.5
  [ "$output" = 'Order {50% off}: 4 items, total 10.00' ]
}

@test "dollar-in-name" {
  run describe_order '${HOME}' 1 3
  [ "$output" = 'Order {${HOME}}: 1 item, total 3.00' ]
}

@test "product-rounds" {
  run describe_order 'Cy' 3 19.99
  [ "$output" = 'Order {Cy}: 3 items, total 59.97' ]
}

@test "large-total" {
  run describe_order 'Dee' 1000 1234.56
  [ "$output" = 'Order {Dee}: 1000 items, total 1234560.00' ]
}

@test "negative-price" {
  run describe_order 'Refund' 2 -4.5
  [ "$output" = 'Order {Refund}: 2 items, total -9.00' ]
}
