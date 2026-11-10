```go norun reason=fragment group=records id=records file=records.go
package records

import "fmt"

// Record is one [tag][length][value] entry.
type Record struct {
	Tag   byte
	Value []byte
}

// Parse reads records and returns an error for a truncated input.
func Parse(data []byte) ([]Record, error) {
	var records []Record
	for i := 0; i < len(data); {
		if i+2 > len(data) {
			return nil, fmt.Errorf("truncated header at offset %d", i)
		}
		tag, length := data[i], int(data[i+1])
		if i+2+length > len(data) {
			return nil, fmt.Errorf("truncated value at offset %d", i)
		}
		records = append(records, Record{tag, data[i+2 : i+2+length]})
		i += 2 + length
	}
	return records, nil
}

// Encode writes records in the same format.
func Encode(records []Record) []byte {
	var out []byte
	for _, r := range records {
		out = append(out, r.Tag, byte(len(r.Value)))
		out = append(out, r.Value...)
	}
	return out
}
```

```go test group=records id=records-test file=records_test.go
package records

import (
	"bytes"
	"testing"
)

// FuzzParse runs only the seed corpus under plain go test.
// go test -fuzz=FuzzParse would also generate new inputs.
func FuzzParse(f *testing.F) {
	f.Add([]byte{})
	f.Add([]byte("\x01\x02hi"))
	f.Add([]byte("\x01\x02hi\x02\x00"))
	f.Add([]byte("z"))     // a failing input that fuzzing found
	f.Add([]byte("~\xe1")) // another one
	f.Fuzz(func(t *testing.T, data []byte) {
		records, err := Parse(data)
		if err != nil {
			t.Logf("rejected: %v", err)
			return
		}
		if got := Encode(records); !bytes.Equal(got, data) {
			t.Fatalf("Encode(Parse(%x)) = %x", data, got)
		}
		t.Logf("records=%d", len(records))
	})
}
```

```output
=== RUN   FuzzParse
=== RUN   FuzzParse/seed#0
    records_test.go:25: records=0
=== RUN   FuzzParse/seed#1
    records_test.go:25: records=1
=== RUN   FuzzParse/seed#2
    records_test.go:25: records=2
=== RUN   FuzzParse/seed#3
    records_test.go:19: rejected: truncated header at offset 0
=== RUN   FuzzParse/seed#4
    records_test.go:19: rejected: truncated value at offset 0
--- PASS: FuzzParse ($TIME)
    --- PASS: FuzzParse/seed#0 ($TIME)
    --- PASS: FuzzParse/seed#1 ($TIME)
    --- PASS: FuzzParse/seed#2 ($TIME)
    --- PASS: FuzzParse/seed#3 ($TIME)
    --- PASS: FuzzParse/seed#4 ($TIME)
PASS
ok  	example	$TIME
```

The original parser panics on the input of one byte. In a fuzz test, the panic is a failure, and Go writes the input to `testdata/fuzz/FuzzParse/`.

```go fails expect=panic id=no-length-check
package main

import "fmt"

// parse counts the records of the form [tag][length][value].
func parse(data []byte) int {
	count := 0
	for i := 0; i < len(data); {
		length := int(data[i+1]) // no check that the length byte exists
		count++
		i += 2 + length
	}
	return count
}

func main() {
	seeds := [][]byte{{}, []byte("\x01\x02hi"), []byte("\x01\x02hi\x02\x00"), []byte("z"), []byte("~\xe1")}
	for _, seed := range seeds {
		fmt.Printf("records=%d\n", parse(seed))
	}
}
```

- Verdict: native. Fuzzing is part of `go test`. A fuzz test is a function `FuzzXxx(f *testing.F)` in a `_test.go` file. `f.Add` adds seed corpus entries, and `f.Fuzz` takes the target.
- Plain `go test`, as here, runs only the seed corpus: one subtest for each `f.Add`, named `seed#N`, and one for each file in `testdata/fuzz/FuzzParse/`. `go test -fuzz=FuzzParse` generates more inputs with coverage guidance until it fails or is stopped.
- The arguments of the target after `*testing.T` must have the types of the seeds, in the same order. Here there is one `[]byte`.
- Go checks every slice index, so the bug is a panic with the index and the length. A slice expression `data[i+2 : i+2+length]` past the end also panics, unlike a Python slice.
