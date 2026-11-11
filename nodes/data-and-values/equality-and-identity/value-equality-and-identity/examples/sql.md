```sql run id=nulls
CREATE TABLE users (name TEXT, email TEXT);
INSERT INTO users VALUES ('ann', 'ann@example.com'), ('bob', NULL), ('cy', NULL);

SELECT 'NULL = NULL is null', (NULL = NULL) IS NULL;
SELECT 'rows where email = NULL', count(*) FROM users WHERE email = NULL;
SELECT 'rows where email IS NULL', count(*) FROM users WHERE email IS NULL;
SELECT 'NULL IS NOT DISTINCT FROM NULL', NULL IS NOT DISTINCT FROM NULL;
SELECT 'distinct emails', count(*) FROM (SELECT DISTINCT email FROM users);
```

```output for=nulls
NULL = NULL is null|1
rows where email = NULL|0
rows where email IS NULL|2
NULL IS NOT DISTINCT FROM NULL|1
distinct emails|2
```

- Verdict: native. `=` compares values. When either side is `NULL`, the result is `NULL`, not true or false, so a `WHERE` clause with `= NULL` selects no rows.
- `IS` and `IS NOT DISTINCT FROM` treat two `NULL` values as equal. `IS` is a short form that SQLite adds. Other databases need `IS NOT DISTINCT FROM`.
- `DISTINCT` and `GROUP BY` treat two `NULL` values as the same, so the three rows have two distinct emails: one address and one `NULL`.
- A row has no identity apart from its key. Equality of rows is equality of their columns.
