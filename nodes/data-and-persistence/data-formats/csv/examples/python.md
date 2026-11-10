```python run id=dict-reader
import csv

data = b"\xef\xbb\xbfzip,city,amount\r\n02134,Boston,12.50\r\n10001,New York\r\n"
with open("people.csv", "wb") as f:
    f.write(data)

with open("people.csv", encoding="utf-8", newline="") as f:
    print(csv.DictReader(f).fieldnames)

with open("people.csv", encoding="utf-8-sig", newline="") as f:
    for row in csv.DictReader(f):
        print(row)
```

```output for=dict-reader
['\ufeffzip', 'city', 'amount']
{'zip': '02134', 'city': 'Boston', 'amount': '12.50'}
{'zip': '10001', 'city': 'New York', 'amount': None}
```

```python run id=strict
import csv
import io

for text in ['7,"Smith, Ann"x\r\n', '7,Sm"ith\r\n']:
    print(next(csv.reader(io.StringIO(text, newline=""))))
    try:
        print(next(csv.reader(io.StringIO(text, newline=""), strict=True)))
    except csv.Error as error:
        print("strict:", error)
```

```output for=strict
['7', 'Smith, Annx']
strict: ',' expected after '"'
['7', 'Sm"ith']
['7', 'Sm"ith']
```

```python run id=dialects
import csv
import io

text = "name;price\r\nTea;3,50\r\n"
dialect = csv.Sniffer().sniff(text)
print(repr(dialect.delimiter))
print(list(csv.reader(io.StringIO(text, newline=""), dialect)))

out = io.StringIO(newline="")
csv.writer(out, delimiter=";").writerow(["Tea", "3,50", "a;b"])
print(repr(out.getvalue()))
```

```output for=dialects
';'
[['name', 'price'], ['Tea', '3,50']]
'Tea;3,50;"a;b"\r\n'
```

```python run id=injection
import csv
import io

TRIGGERS = ("=", "+", "-", "@", "\t", "\r", "\n")


def safe(cell):
    text = str(cell)
    return "'" + text if text.startswith(TRIGGERS) else text


rows = [["name", "comment"], ["Ann", "=HYPERLINK(\"http://example.com\")"], ["Bo", "-5 points"]]
out = io.StringIO(newline="")
writer = csv.writer(out, quoting=csv.QUOTE_ALL)
for row in rows:
    writer.writerow([safe(cell) for cell in row])
print(out.getvalue(), end="")
```

```output for=injection
"name","comment"
"Ann","'=HYPERLINK(""http://example.com"")"
"Bo","'-5 points"
```

- Verdict: stdlib. The `csv` module has `reader` and `writer` for lists, and `DictReader` and `DictWriter` for rows keyed by the header.
- Open files with `newline=""`. The reader then sees each line break inside a quoted field as it is.
- `DictReader` gives `None` for a missing field. It does not raise an error for a short row.
- The reader is lenient by default. `strict=True` rejects text after a closing quote, but it still accepts a bare quote inside an unquoted field.
- The `safe` function follows the OWASP advice. It adds a single quote before a cell that starts with a formula character, and `QUOTE_ALL` puts every cell in quotes.
