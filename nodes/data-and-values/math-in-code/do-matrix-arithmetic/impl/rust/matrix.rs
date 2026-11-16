// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

/// Returns the transpose of m: row i of the result is column i of m.
pub fn transpose<R: AsRef<[i64]>>(m: &[R]) -> Vec<Vec<i64>> {
    let cols = m[0].as_ref().len();
    (0..cols).map(|j| m.iter().map(|row| row.as_ref()[j]).collect()).collect()
}

/// Returns the product a times b, or None if the number of columns of a is
/// not the number of rows of b.
pub fn matrix_multiply<R: AsRef<[i64]>>(a: &[R], b: &[R]) -> Option<Vec<Vec<i64>>> {
    if a[0].as_ref().len() != b.len() {
        return None;
    }
    let p = b[0].as_ref().len();
    let mut c = vec![vec![0i64; p]; a.len()];
    for (i, row) in a.iter().enumerate() {
        for (k, &x) in row.as_ref().iter().enumerate() {
            for (j, &y) in b[k].as_ref().iter().enumerate() {
                c[i][j] += x * y;
            }
        }
    }
    Some(c)
}
