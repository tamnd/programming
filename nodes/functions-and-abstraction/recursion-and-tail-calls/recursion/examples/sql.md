```sql run id=chain
CREATE TABLE employee (name TEXT PRIMARY KEY, boss TEXT);
INSERT INTO employee VALUES
    ('Ada', NULL), ('Brian', 'Ada'), ('Chen', 'Ada'), ('Dana', 'Brian'), ('Eve', 'Dana');

WITH RECURSIVE chain(name, level) AS (
    SELECT name, 0 FROM employee WHERE boss IS NULL
    UNION ALL
    SELECT employee.name, chain.level + 1
    FROM employee JOIN chain ON employee.boss = chain.name
)
SELECT level, name FROM chain ORDER BY level, name;

WITH RECURSIVE factorial(n, value) AS (
    SELECT 1, 1
    UNION ALL
    SELECT n + 1, value * (n + 1) FROM factorial WHERE n < 10
)
SELECT value FROM factorial WHERE n = 10;
```

```output for=chain
0|Ada
1|Brian
1|Chen
2|Dana
3|Eve
3628800
```

- Verdict: native. `WITH RECURSIVE` defines a table in terms of itself. The example runs in SQLite.
- A recursive common table expression is a compound select. The first part gives the starting rows, and the recursive part refers to the table once and computes more rows from them.
- SQLite does not use a call stack for this. It puts rows in a queue, takes one row at a time, and runs the recursive select on that row. The recursion stops when the queue is empty.
- A recursive part with no stop condition runs until it fills memory. A `WHERE` clause in the recursive part or a `LIMIT` on the query stops it.
