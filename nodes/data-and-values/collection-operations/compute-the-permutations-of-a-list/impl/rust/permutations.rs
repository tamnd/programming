// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

/// Returns each order of the items, in the lexicographic order of the positions.
pub fn permutations(items: &[i32]) -> Vec<Vec<i32>> {
    let mut out = Vec::new();
    let mut used = vec![false; items.len()];
    let mut path = Vec::with_capacity(items.len());
    visit(items, &mut used, &mut path, &mut out);
    out
}

fn visit(items: &[i32], used: &mut [bool], path: &mut Vec<i32>, out: &mut Vec<Vec<i32>>) {
    if path.len() == items.len() {
        // path changes later, so add a copy.
        out.push(path.clone());
        return;
    }
    for (i, &x) in items.iter().enumerate() {
        if used[i] {
            continue;
        }
        used[i] = true;
        path.push(x);
        visit(items, used, path, out);
        path.pop();
        used[i] = false;
    }
}
