```python run id=value-in-owner-columns
import sqlite3
from dataclasses import dataclass

db = sqlite3.connect(":memory:")
db.execute("""create table employment (id integer primary key, person text,
              start_date text, end_date text, salary_amount integer, salary_currency text)""")


@dataclass(frozen=True)
class DateRange:
    start: str
    end: str | None  # None: the employment has not ended.

    def __str__(self):
        return f"{self.start} to {self.end or 'open'}"


@dataclass(frozen=True)
class Money:
    amount: int
    currency: str

    def __str__(self):
        return f"{self.amount} {self.currency}"


class Employment:
    def __init__(self, person, period, salary, id=None):
        self.id, self.person, self.period, self.salary = id, person, period, salary


def insert(e):
    sql = "insert into employment values (null, ?, ?, ?, ?, ?)"
    p, s = e.period, e.salary
    e.id = db.execute(sql, (e.person, p.start, p.end, s.amount, s.currency)).lastrowid


def update(e):
    sql = """update employment set person = ?, start_date = ?, end_date = ?,
             salary_amount = ?, salary_currency = ? where id = ?"""
    p, s = e.period, e.salary
    db.execute(sql, (e.person, p.start, p.end, s.amount, s.currency, e.id))


def find(id):
    sql = "select * from employment where id = ?"
    id, person, start, end, amount, currency = db.execute(sql, (id,)).fetchone()
    return Employment(person, DateRange(start, end), Money(amount, currency), id)


def show_rows():
    for row in db.execute("select * from employment order by id"):
        print("row:", " ".join("null" if v is None else str(v) for v in row))


insert(Employment("Ann", DateRange("2024-01-01", "2024-12-31"), Money(5000, "EUR")))
insert(Employment("Bo", DateRange("2025-03-01", None), Money(4000, "EUR")))
show_rows()
ann, bo = find(1), find(2)
for e in (ann, bo):
    print(f"loaded: {e.person} | {e.period} | {e.salary}")
same = "yes" if ann.salary == Money(5000, "EUR") else "no"
print(f"salary of Ann equals 5000 EUR: {same}")

ann.salary = Money(5500, "EUR")  # A new value replaces the old one.
update(ann)
print(f"after raise: Ann {find(1).salary} | Bo {find(2).salary}")

sql = "select person from employment where salary_currency = 'EUR' and salary_amount > 4500"
print("EUR salaries above 4500:", ", ".join(p for (p,) in db.execute(sql)))
```

```output
row: 1 Ann 2024-01-01 2024-12-31 5000 EUR
row: 2 Bo 2025-03-01 null 4000 EUR
loaded: Ann | 2024-01-01 to 2024-12-31 | 5000 EUR
loaded: Bo | 2025-03-01 to open | 4000 EUR
salary of Ann equals 5000 EUR: yes
after raise: Ann 5500 EUR | Bo 4000 EUR
EUR salaries above 4500: Ann
```

- Verdict: idiom. `DateRange` and `Money` are frozen dataclasses, so they compare by their fields and cannot change after they are made. A raise is a new `Money` value.
- `insert` and `update` write the fields of both values to the columns of the `employment` row, and `find` builds new values from these columns. A null `end_date` becomes `None`.
