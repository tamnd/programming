```go run id=read-write
package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"strings"
)

func main() {
	text := "id,name,note\r\n7,\"Smith, Ann\",\"said \"\"hi\"\"\r\nand left\"\r\n\r\n8,Bo,\r\n"
	records, err := csv.NewReader(strings.NewReader(text)).ReadAll()
	fmt.Println(err)
	for _, record := range records {
		fmt.Printf("%q\n", record)
	}

	bom, _ := csv.NewReader(strings.NewReader("\ufeffzip,city\n02134,Boston\n")).Read()
	fmt.Printf("%q\n", bom)

	writer := csv.NewWriter(os.Stdout)
	writer.Write([]string{"7", "Smith, Ann", "said \"hi\""})
	writer.Write([]string{"8", " Bo", ""})
	writer.Flush()

	var crlf strings.Builder
	w := csv.NewWriter(&crlf)
	w.UseCRLF = true
	w.Write([]string{"a", "b"})
	w.Flush()
	fmt.Printf("%q\n", crlf.String())
}
```

```output for=read-write
<nil>
["id" "name" "note"]
["7" "Smith, Ann" "said \"hi\"\nand left"]
["8" "Bo" ""]
["\ufeffzip" "city"]
7,"Smith, Ann","said ""hi"""
8," Bo",
"a,b\r\n"
```

```go run id=errors
package main

import (
	"encoding/csv"
	"errors"
	"fmt"
	"strings"
)

func main() {
	ragged := csv.NewReader(strings.NewReader("a,b\n1,2,3\n"))
	_, err := ragged.ReadAll()
	fmt.Println(err, errors.Is(err, csv.ErrFieldCount))

	loose := csv.NewReader(strings.NewReader("a,b\n1,2,3\n"))
	loose.FieldsPerRecord = -1
	all, _ := loose.ReadAll()
	fmt.Println(all)

	for _, input := range []string{"7,Sm\"ith\n", "7,\"Smith\" Ann\n"} {
		_, err := csv.NewReader(strings.NewReader(input)).Read()
		fmt.Println(err)
		lazy := csv.NewReader(strings.NewReader(input))
		lazy.LazyQuotes = true
		record, err := lazy.Read()
		fmt.Printf("%q %v\n", record, err)
	}
}
```

```output for=errors
record on line 2: wrong number of fields true
[[a b] [1 2 3]]
parse error on line 1, column 5: bare " in non-quoted-field
["7" "Sm\"ith"] <nil>
parse error on line 1, column 9: extraneous or missing " in quoted-field
["7" "Smith\" Ann\n"] <nil>
```

- Verdict: stdlib. The package `encoding/csv` has `Reader` and `Writer`. It follows RFC 4180, but `Writer` ends lines with LF unless `UseCRLF` is true.
- The reader removes a CR before a newline, also inside a quoted field. It skips blank lines. It does not remove a BOM.
- The reader rejects a bare quote and a ragged row by default. `FieldsPerRecord = -1` turns off the field count check.
- `LazyQuotes` accepts bad quotes, but the result can be wrong. In the second input, the field takes the rest of the line, with the newline.
