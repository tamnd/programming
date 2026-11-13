// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

/// Returns each choice of k items, in the lexicographic order of the positions.
pub fn combinations(items: &[i32], k: usize) -> Vec<Vec<i32>> {
    let mut out = Vec::new();
    if k <= items.len() {
        let mut path = Vec::with_capacity(k);
        visit(items, k, 0, &mut path, &mut out);
    }
    out
}

fn visit(items: &[i32], k: usize, start: usize, path: &mut Vec<i32>, out: &mut Vec<Vec<i32>>) {
    if path.len() == k {
        // path changes later, so add a copy.
        out.push(path.clone());
        return;
    }
    // Stop when too few items are left to fill path.
    for i in start..=items.len() - (k - path.len()) {
        path.push(items[i]);
        visit(items, k, i + 1, path, out);
        path.pop();
    }
}
