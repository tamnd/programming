// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

use std::cmp::Reverse;
use std::collections::BinaryHeap;

/// Returns the request numbers in serve order: the largest priority first,
/// and for equal priorities, the smaller request number first.
pub fn serve_order(priorities: &[i32]) -> Vec<usize> {
    // BinaryHeap is a max-heap. Tuples compare item by item, so the largest
    // priority comes first. Reverse(i) makes the smaller request number the
    // larger value, so equal priorities keep the arrival order.
    let mut heap: BinaryHeap<(i32, Reverse<usize>)> = BinaryHeap::new();
    for (i, &p) in priorities.iter().enumerate() {
        heap.push((p, Reverse(i)));
    }
    let mut served = Vec::with_capacity(heap.len());
    while let Some((_, Reverse(i))) = heap.pop() {
        served.push(i);
    }
    served
}
