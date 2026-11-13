```rust run id=nested-arrays-and-vec
use std::mem::size_of;

fn main() {
    // [[i32; 3]; 2] is an array of arrays in one block: 2 * 3 * 4 bytes.
    let mut a = [[0i32; 3]; 2];
    a[0][1] = 5;
    println!("{:?} {} {}", a, a.len(), size_of::<[[i32; 3]; 2]>());

    // The rows are next to each other, so flatten gives the items in row-major order.
    let m = [[1, 2, 3], [4, 5, 6]];
    println!("{:?}", m.iter().flatten().collect::<Vec<_>>());

    // Vec<Vec<T>> has one heap block per row, and the rows can differ in length.
    let mut jagged: Vec<Vec<i32>> = (1..=3).map(|n| vec![0; n]).collect();
    jagged[2][2] = 7;
    println!("{:?} {:?}", jagged, jagged.get(5).and_then(|row| row.first()));

    // vec![row; n] clones the row, so the rows are independent.
    let mut grid = vec![vec![0; 3]; 2];
    grid[0][1] = 5;
    println!("{:?}", grid);

    // A flat Vec with an index formula is a dense matrix. Column-major order swaps the roles.
    let (rows, cols) = (2, 3);
    let flat: Vec<i32> = (0..rows * cols).map(|i| (10 * (i / cols) + i % cols) as i32).collect();
    let col_major: Vec<i32> = (0..rows * cols).map(|i| flat[(i % rows) * cols + i / rows]).collect();
    println!("{:?} {:?} {}", flat, col_major, flat[1 * cols + 2]);
}
```

```output
[[0, 5, 0], [0, 0, 0]] 2 24
[1, 2, 3, 4, 5, 6]
[[0], [0, 0], [0, 0, 7]] None
[[0, 5, 0], [0, 0, 0]]
[0, 1, 2, 10, 11, 12] [0, 10, 1, 11, 2, 12] 12
```

- Verdict: native. [[i32; 3]; 2] is one block of 24 bytes, and flatten gives its items in row-major order. Vec<Vec<i32>> has one heap block per row, and the rows can differ in length. vec![row; n] clones the row, so the rows are independent. A flat Vec gives row-major order, and a second formula reads it in column-major order.
