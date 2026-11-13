```java run id=arrays-of-arrays
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        // new int[2][3] makes an array of 2 row arrays with 3 ints each.
        int[][] grid = new int[2][3];
        grid[0][1] = 5;
        System.out.println(Arrays.deepToString(grid) + " " + grid.length + " " + grid[0].length);

        // Each row is its own object, so rows can have different lengths and can be replaced.
        int[][] jagged = new int[3][];
        for (int i = 0; i < jagged.length; i++) jagged[i] = new int[i + 1];
        jagged[2][2] = 7;
        grid[1] = new int[] {1, 2, 3, 4};
        System.out.println(Arrays.deepToString(jagged) + " " + Arrays.deepToString(grid));

        // clone copies only the outer array. The rows are shared.
        int[][] copy = grid.clone();
        copy[0][0] = 9;
        System.out.println((copy == grid) + " " + (copy[0] == grid[0]) + " " + grid[0][0]);

        // A flat array with an index formula is one block of memory in row-major order.
        int rows = 2, cols = 3;
        int[] flat = new int[rows * cols];
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) flat[r * cols + c] = 10 * r + c;
        System.out.println(Arrays.toString(flat) + " " + flat[1 * cols + 2]);
    }
}
```

```output
[[0, 5, 0], [0, 0, 0]] 2 3
[[0], [0, 0], [0, 0, 7]] [[0, 5, 0], [1, 2, 3, 4]]
false true 9
[0, 1, 2, 10, 11, 12] 12
```

- Verdict: native. new int[2][3] makes an array of 2 row arrays. A jagged int[3][] gets rows of different lengths, and the program can replace a row with a longer one. clone copies only the outer array, so a write through the copy changes the original. A flat int[] with an index formula is one block in row-major order.
