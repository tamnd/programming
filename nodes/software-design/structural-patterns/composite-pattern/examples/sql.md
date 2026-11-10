```sql run id=tree
CREATE TABLE entry (id INTEGER PRIMARY KEY, parent INTEGER REFERENCES entry (id), name TEXT, size INTEGER);
INSERT INTO entry VALUES (1, NULL, 'project', NULL), (2, 1, 'readme.md', 200), (3, 1, 'src', NULL),
    (4, 3, 'main.go', 1200), (5, 3, 'util.go', 300);

WITH RECURSIVE
    tree (id, depth, path) AS (
        SELECT id, 0, printf('%05d', id) FROM entry WHERE parent IS NULL
        UNION ALL
        SELECT e.id, t.depth + 1, t.path || '/' || printf('%05d', e.id)
        FROM entry e JOIN tree t ON e.parent = t.id
    ),
    total (id, size) AS (
        SELECT t.id, (SELECT sum(e.size) FROM tree d JOIN entry e ON e.id = d.id
                      WHERE d.path = t.path OR d.path LIKE t.path || '/%')
        FROM tree t
    )
SELECT printf('%s%s%s %d', substr('        ', 1, 2 * t.depth), e.name,
              CASE WHEN e.size IS NULL THEN '/' ELSE '' END, s.size)
FROM tree t JOIN entry e ON e.id = t.id JOIN total s ON s.id = t.id
ORDER BY t.path;
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. A table where each row names its parent stores the tree, and a recursive common table expression walks it. The same query handles a file and a directory: a directory has no size of its own, and its total is the sum over the rows below it.
- The path column orders the rows as a depth-first walk, so the output reads like the other examples.
- A cycle in the parent column makes this query run without end, because each step makes a row with a longer path. A condition such as `t.depth < 100` in the recursive part stops it. The example runs in SQLite.
