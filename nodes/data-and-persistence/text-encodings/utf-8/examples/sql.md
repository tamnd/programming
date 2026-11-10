```sql run id=lengths
CREATE TABLE word (label TEXT, value TEXT);
INSERT INTO word VALUES ('composed', 'caf' || char(0xE9));
INSERT INTO word VALUES ('decomposed', 'cafe' || char(0x301));
INSERT INTO word VALUES ('clef', char(0x1D11E));

SELECT label, length(value), octet_length(value), hex(value), unicode(substr(value, -1)) FROM word;
SELECT count(DISTINCT value) FROM word WHERE label <> 'clef';
```

```output for=lengths
composed|4|5|636166C3A9|233
decomposed|5|6|63616665CC81|769
clef|1|4|F09D849E|119070
2
```

- Verdict: native. A SQLite database stores text in UTF-8 by default. The function `length` counts characters, `octet_length` counts bytes, `hex` shows the bytes, and `unicode` and `char` convert between characters and code points.
- SQLite compares text byte by byte by default. The composed and the decomposed forms of "café" look the same, but they are two distinct values. SQLite has no normalization function, so normalize the text before it goes into the database.
- Other databases name the encoding in the definition of the database or the column. In MySQL, the character set `utf8mb4` is UTF-8 with up to four bytes. The older name `utf8` stores at most three bytes, so it cannot hold characters outside the BMP.
