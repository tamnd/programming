```python run id=account-record-set
import sqlite3


class RecordSet:
    """Rows and named columns in memory, in the shape of the result of a query."""

    def __init__(self, columns, rows):
        self.columns = list(columns)
        self.rows = [list(r) for r in rows]
        self.states = ["unchanged"] * len(self.rows)

    @classmethod
    def from_cursor(cls, cursor):
        return cls([d[0] for d in cursor.description], cursor.fetchall())

    def get(self, i, column):
        return self.rows[i][self.columns.index(column)]

    def set(self, i, column, value):
        self.rows[i][self.columns.index(column)] = value
        if self.states[i] == "unchanged":
            self.states[i] = "modified"

    def add(self, *values):
        self.rows.append(list(values))
        self.states.append("added")

    def where(self, test):
        return [i for i in range(len(self.rows)) if test(lambda c: self.get(i, c))]


def money(cents):
    return f"{cents // 100}.{cents % 100:02d}"


db = sqlite3.connect(":memory:")
db.execute("create table account (id integer, name text, balance integer)")
db.executemany("insert into account values (?, ?, ?)", [(1, "Ann", 12000), (2, "Bob", 1550)])
rs = RecordSet.from_cursor(db.execute("select id, name, balance from account order by id"))
db.close()  # the record set needs no connection from here on

print("columns:", ", ".join(rs.columns))
rs.set(1, "balance", rs.get(1, "balance") + 1000)
rs.add(3, "Cy", 0)
for i in range(len(rs.rows)):
    print(f"{rs.get(i, 'id')} | {rs.get(i, 'name')} | {money(rs.get(i, 'balance'))} | {rs.states[i]}")
rich = rs.where(lambda col: col("balance") > 2000)
print("balance over 20.00:", ", ".join(rs.get(i, "name") for i in rich))
print("rows to save:", sum(1 for s in rs.states if s != "unchanged"))
```

```output
columns: id, name, balance
1 | Ann | 120.00 | unchanged
2 | Bob | 25.50 | modified
3 | Cy | 0.00 | added
balance over 20.00: Ann, Bob
rows to save: 2
```

- Verdict: stdlib. `sqlite3` gives the rows of a query as tuples, and `cursor.description` gives the names of the columns. The `RecordSet` class keeps both in memory, so it works after the connection is closed.
- The class marks each row as unchanged, modified or added, so the code that saves the record set knows which rows to write.
