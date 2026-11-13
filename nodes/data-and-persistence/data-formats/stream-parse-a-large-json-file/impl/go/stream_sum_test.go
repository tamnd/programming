// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

package streamsum

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func write(t *testing.T, data string) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "input.json")
	if err := os.WriteFile(path, []byte(data), 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

// 2500 records. The second space in the first record puts the digits of one amount across the offset 65536, so they come in two chunks.
func manyRecords() string {
	records := make([]string, 2500)
	for i := range records {
		space := " "
		if i == 0 {
			space = "  "
		}
		records[i] = fmt.Sprintf(`{"id": %d,%s"amount": %d}`, i, space, i*37%1000-400)
	}
	return "[" + strings.Join(records, ",\n") + "]\n"
}

func TestCases(t *testing.T) {
	cases := []struct {
		name, data string
		want       [2]int64
	}{
		{"three-records", `[{"id": 1, "amount": 10}, {"id": 2, "amount": 25}, {"id": 3, "amount": -5}]` + "\n", [2]int64{3, 30}},
		{"pretty", "[\n  {\n    \"id\": 1,\n    \"amount\": 100\n  },\n  {\n    \"id\": 2,\n    \"amount\": 250\n  }\n]\n", [2]int64{2, 350}},
		{"empty-array", `[]` + "\n", [2]int64{0, 0}},
		{"one-record", `[{"amount": 7}]`, [2]int64{1, 7}},
		{"extra-members", `[{"id": 1, "tags": ["a", "b"], "meta": {"amount": 999, "x": [1, [2]]}, "amount": 5}, {"amount": 6, "ok": true, "none": null}]` + "\n", [2]int64{2, 11}},
		{"strings-with-brackets", `[{"note": "a ] b , c } \" [ {", "amount": 1}, {"note": "\\", "amount": 2}, {"note": "]", "amount": 3}]` + "\n", [2]int64{3, 6}},
		{"non-ascii", `[{"name": "Zürich 😀", "amount": 3}, {"name": "日本語", "amount": 4}]` + "\n", [2]int64{2, 7}},
		{"crlf", "[\r\n{\"amount\": 1},\r\n{\"amount\": 2}\r\n]\r\n", [2]int64{2, 3}},
		{"bom", "\uFEFF[{\"amount\": 4}]\n", [2]int64{1, 4}},
		{"big-total", `[{"amount": 4503599627370496}, {"amount": 4503599627370495}, {"amount": 0}]` + "\n", [2]int64{3, 9007199254740991}},
		{"negative-total", `[{"amount": -9007199254740991}, {"amount": 0}]` + "\n", [2]int64{2, -9007199254740991}},
		{"whitespace", " \n\t[ {\"amount\" : 1} , {\"amount\":2} ]  \n\n", [2]int64{2, 3}},
		// A string of 70000 characters is longer than one chunk.
		{"long-string", `[{"note": "` + strings.Repeat("x", 70000) + `", "amount": 8}, {"amount": 9}]` + "\n", [2]int64{2, 17}},
		{"many-records", manyRecords(), [2]int64{2500, 249750}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := StreamSum(write(t, c.data))
			if err != nil || got != c.want {
				t.Errorf("StreamSum = %v, %v; want %v, nil", got, err, c.want)
			}
		})
	}
}

func TestInvalid(t *testing.T) {
	cases := []struct {
		name, data string
		badRecord  bool
	}{
		{"not-array", `{"amount": 1}` + "\n", false},
		{"trailing-comma", `[{"amount": 1},]` + "\n", false},
		{"missing-comma", `[{"amount": 1} {"amount": 2}]` + "\n", false},
		{"unclosed", `[{"amount": 1},` + "\n", false},
		{"unclosed-element", `[{"amount": 1}, {"amount": 2` + "\n", false},
		{"data-after", `[{"amount": 1}] x` + "\n", false},
		{"two-arrays", `[] []` + "\n", false},
		{"empty-file", "", false},
		{"bad-element", `[{"amount": 1}, {"amount": }]` + "\n", false},
		{"nan", `[{"amount": NaN}]` + "\n", false},
		{"not-object", `[1, 2]` + "\n", true},
		{"missing-amount", `[{"amount": 1}, {"id": 2}]` + "\n", true},
		{"string-amount", `[{"amount": "5"}]` + "\n", true},
		{"null-amount", `[{"amount": null}]` + "\n", true},
		{"bool-amount", `[{"amount": true}]` + "\n", true},
		{"nested-array", `[[{"amount": 1}]]` + "\n", true},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			_, err := StreamSum(write(t, c.data))
			if err == nil || errors.Is(err, ErrBadRecord) != c.badRecord {
				t.Errorf("err = %v, want an error with ErrBadRecord %v", err, c.badRecord)
			}
		})
	}
}

func TestMissing(t *testing.T) {
	if _, err := StreamSum(filepath.Join(t.TempDir(), "missing.json")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("err = %v, want fs.ErrNotExist", err)
	}
}
