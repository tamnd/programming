```sql run id=export
CREATE TABLE payment (id INTEGER, name TEXT, note TEXT);
INSERT INTO payment VALUES (7, 'Smith, Ann', 'said "hi"'), (8, 'Bo', NULL), (9, 'Cy', '');
.headers on
.mode csv
SELECT * FROM payment;
```

```output for=export
id,name,note
7,"Smith, Ann","said ""hi"""
8,Bo,
9,Cy,""
```

- Verdict: idiom. The SQL standard has no CSV functions. The SQLite shell has the dot command `.mode csv` for output and `.import --csv` for input, and PostgreSQL has `COPY ... (FORMAT csv)`.
- SQLite writes the CSV of RFC 4180, with CRLF line ends and quotes only where they are needed.
- The NULL in row 8 becomes an empty field, and the empty string in row 9 becomes `""`. CSV has no other way to tell the two values apart.
