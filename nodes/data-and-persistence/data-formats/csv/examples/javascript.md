```javascript run id=parser
// Parse RFC 4180 text into rows. Accepts CRLF or LF row ends.
function parseCsv(text) {
  const rows = [];
  let row = [];
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

function formatField(value) {
  return /[",\r\n]/.test(value) ? `"${value.replaceAll('"', '""')}"` : value;
}

function formatCsv(rows) {
  return rows.map((row) => row.map(formatField).join(",") + "\r\n").join("");
}

const text = 'id,name,note\r\n7,"Smith, Ann","said ""hi""\r\nand left"\r\n8,Bo,\r\n';
const rows = parseCsv(text);
console.log(rows);
console.log(JSON.stringify(formatCsv(rows)));
console.log(formatCsv(rows) === text);
console.log(parseCsv("a,b\n1,2"));
for (const bad of ['7,Sm"ith\n', '7,"Smith"x\n', '7,"Smith']) {
  try {
    parseCsv(bad);
  } catch (error) {
    console.log(error.message);
  }
}
```

```output for=parser
[
  [ 'id', 'name', 'note' ],
  [ '7', 'Smith, Ann', 'said "hi"\r\nand left' ],
  [ '8', 'Bo', '' ]
]
"id,name,note\r\n7,\"Smith, Ann\",\"said \"\"hi\"\"\r\nand left\"\r\n8,Bo,\r\n"
true
[ [ 'a', 'b' ], [ '1', '2' ] ]
bare quote at offset 4
text after closing quote at offset 9
missing closing quote
```

- Verdict: idiom. JavaScript has no CSV parser in the language or in Node.js. The function `parseCsv` is a short state machine for RFC 4180. It accepts CRLF and LF row ends, and it rejects a bare quote, text after a closing quote and a missing closing quote.
- Each field is a string. The program must convert numbers and dates itself.
