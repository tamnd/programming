// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

use std::sync::atomic::{AtomicU64, Ordering};
use std::sync::OnceLock;

/// Hands out consecutive ids. Get the one instance with `IdGenerator::instance()`.
pub struct IdGenerator {
    next: AtomicU64,
}

static INSTANCE: OnceLock<IdGenerator> = OnceLock::new();
static CONSTRUCTIONS: AtomicU64 = AtomicU64::new(0);

impl IdGenerator {
    fn new() -> Self {
        CONSTRUCTIONS.fetch_add(1, Ordering::SeqCst);
        IdGenerator { next: AtomicU64::new(1) }
    }

    /// Returns the one instance. `OnceLock` runs `new` once, even when
    /// several threads call this at the same time.
    pub fn instance() -> &'static IdGenerator {
        INSTANCE.get_or_init(IdGenerator::new)
    }

    pub fn next_id(&self) -> u64 {
        self.next.fetch_add(1, Ordering::Relaxed)
    }

    pub fn constructions() -> u64 {
        CONSTRUCTIONS.load(Ordering::SeqCst)
    }
}

pub fn singleton_report(calls: u32) -> Vec<u64> {
    let mut seen: Vec<*const IdGenerator> = Vec::new();
    let (mut first, mut last) = (0, 0);
    for i in 0..calls {
        let g = IdGenerator::instance();
        if !seen.contains(&(g as *const IdGenerator)) {
            seen.push(g);
        }
        last = g.next_id();
        if i == 0 {
            first = last;
        }
    }
    vec![last - first, IdGenerator::constructions(), seen.len() as u64]
}
