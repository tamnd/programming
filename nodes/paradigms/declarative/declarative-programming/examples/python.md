```python run id=sql-and-regex
import re
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE orders (item TEXT, qty INTEGER, price INTEGER)")
db.executemany("INSERT INTO orders VALUES (?, ?, ?)", [("pen", 3, 2), ("book", 1, 12), ("ink", 2, 5), ("pen", 4, 2)])

query = "SELECT item, SUM(qty * price) FROM orders GROUP BY item ORDER BY 2 DESC"
for row in db.execute(query):
    print(row)

print(re.findall(r"\b(\d{4})-(\d{2})-(\d{2})\b", "from 2024-01-31 to 2025-12-01"))
```

```output for=sql-and-regex
('pen', 14)
('book', 12)
('ink', 10)
[('2024', '01', '31'), ('2025', '12', '01')]
```

```python wrong id=greedy
import re

html = "<b>bold</b> and <i>italic</i>"
print(re.findall(r"<.*>", html))
print(re.findall(r"<.*?>", html))
```

```output for=greedy
['<b>bold</b> and <i>italic</i>']
['<b>', '</b>', '<i>', '</i>']
```

- Verdict: native. The standard library includes `sqlite3`, so a Python program can state a query in SQL and let the SQLite engine choose how to run it. The `re` module compiles a regular expression into a matcher.
- The `?` placeholders pass values to the query as data. Do not build SQL text from values with string formatting.
- In the wrong block, `.*` is greedy, so the first pattern matches from the first `<` to the last `>`. A regular expression states what to match, but its rules for which match wins still need care. The lazy form `.*?` matches each tag.
