// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

use std::collections::HashSet;

/// An amount in cents and an upper-case currency code.
/// The derived PartialEq, Eq and Hash use the same two fields, so they agree.
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct Money {
    cents: i64,
    currency: String,
}

impl Money {
    /// "eur" and "EUR" are the same currency.
    pub fn new(cents: i64, currency: &str) -> Self {
        Money { cents, currency: currency.to_ascii_uppercase() }
    }

    pub fn cents(&self) -> i64 {
        self.cents
    }

    pub fn currency(&self) -> &str {
        &self.currency
    }
}

/// Returns the number of different Money values.
pub fn count_distinct(amounts: &[i64], currencies: &[&str]) -> usize {
    amounts
        .iter()
        .zip(currencies)
        .map(|(&amount, currency)| Money::new(amount, currency))
        .collect::<HashSet<_>>()
        .len()
}
