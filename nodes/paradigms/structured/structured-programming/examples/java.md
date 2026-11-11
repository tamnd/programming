```java run id=labels
public class Main {
    public static void main(String[] args) {
        int[][] grid = {{3, 1}, {4, -1}, {-5, 9}};
        int foundRow = -1;
        int foundCol = -1;
        search:
        for (int r = 0; r < grid.length; r++) {
            for (int c = 0; c < grid[r].length; c++) {
                if (grid[r][c] < 0) {
                    foundRow = r;
                    foundCol = c;
                    break search;
                }
            }
        }
        System.out.println(foundRow + "," + foundCol);
    }
}
```

```output for=labels
1,1
```

- Verdict: native. `goto` is a reserved word in Java, but it has no meaning. A labeled `break` or `continue` leaves enclosing loops, and `try`/`finally` runs cleanup on every exit.
