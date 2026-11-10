```typescript run id=typed-rows
type Row = string[];

function parseCsv(text: string): Row[] {
  const rows: Row[] = [];
  let row: Row = [];
  let field = "";
  let quoted = false;
  let closed = false;
  for (let i = 0; i < text.length; i++) {
    const c = text[i];
    if (quoted) {
      if (c === '"' && text[i + 1] === '"') {
        field += '"';
        i++;
      } else if (c === '"') {
        quoted = false;
        closed = true;
      } else {
        field += c;
      }
    } else if (c === ",") {
      row.push(field);
      field = "";
      closed = false;
    } else if (c === "\n" || (c === "\r" && text[i + 1] === "\n")) {
      if (c === "\r") i++;
      row.push(field);
      rows.push(row);
      row = [];
      field = "";
      closed = false;
    } else if (closed) {
      throw new Error(`text after closing quote at offset ${i}`);
    } else if (c === '"' && field === "") {
      quoted = true;
    } else if (c === '"') {
      throw new Error(`bare quote at offset ${i}`);
    } else {
      field += c;
    }
  }
  if (quoted) throw new Error("missing closing quote");
  if (field !== "" || closed || row.length > 0) {
    row.push(field);
    rows.push(row);
  }
  return rows;
}

interface Payment {
  zip: string;
  city: string;
  amount: number;
}

function toPayments(rows: Row[]): Payment[] {
  const [header, ...body] = rows;
  return body.map((fields, index) => {
    if (fields.length !== header.length) {
      throw new Error(`row ${index + 2}: ${fields.length} fields, expected ${header.length}`);
    }
    const amount = Number(fields[header.indexOf("amount")]);
    if (Number.isNaN(amount)) throw new Error(`row ${index + 2}: amount is not a number`);
    return { zip: fields[header.indexOf("zip")], city: fields[header.indexOf("city")], amount };
  });
}

const text = 'zip,city,amount\r\n02134,Boston,12.50\r\n10001,"New York, NY",3\r\n';
console.log(toPayments(parseCsv(text)));
for (const bad of ["zip,city,amount\r\n02134,Boston\r\n", "zip,city,amount\r\n02134,Boston,ten\r\n"]) {
  try {
    toPayments(parseCsv(bad));
  } catch (error) {
    console.log((error as Error).message);
  }
}
```

```output for=typed-rows
[
  { zip: '02134', city: 'Boston', amount: 12.5 },
  { zip: '10001', city: 'New York, NY', amount: 3 }
]
row 2: 2 fields, expected 3
row 2: amount is not a number
```

- Verdict: idiom. TypeScript has no CSV parser, and neither does JavaScript. The function `parseCsv` is the same state machine as in the JavaScript example, with types.
- The types of TypeScript do not check the data at run time. The function `toPayments` checks the field count and converts `amount` by hand. The ZIP code stays a string, so its leading zero stays.
