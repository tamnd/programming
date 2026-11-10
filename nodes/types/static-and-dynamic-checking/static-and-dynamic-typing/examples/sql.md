```sql run id=types
CREATE TABLE item (qty INTEGER);
INSERT INTO item VALUES (2), ('3'), ('abc'), (2.5), (NULL);
SELECT qty, typeof(qty) FROM item;
SELECT typeof(1), typeof(1.0), typeof('1'), typeof(x'01');
```

```output
2|integer
3|integer
abc|text
2.5|real
|null
integer|real|text|blob
```

- Verdict: native. This example uses SQLite. In SQLite, the datatype goes with each value, not with the column. `typeof` gives the storage class of a value.
- A column type gives a type affinity. In an `INTEGER` column, SQLite converts the text `'3'` to an integer and keeps `'abc'` as text.
- A `STRICT` table rejects a value that does not fit the column type (see Pitfalls on the concept page).
