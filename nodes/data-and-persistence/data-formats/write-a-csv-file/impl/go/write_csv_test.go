// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

package writecsv

import (
	"errors"
	"os"
	"path/filepath"
	"testing"
)

type rows = [][]string

var cases = []struct {
	name    string
	records rows
	want    string
}{
	{"simple", rows{{"name", "age"}, {"Ada", "36"}, {"Alan", "41"}}, "name,age\r\nAda,36\r\nAlan,41\r\n"},
	{"comma", rows{{"name", "born"}, {"Lovelace, Ada", "1815"}}, "name,born\r\n\"Lovelace, Ada\",1815\r\n"},
	{"double-quote", rows{{"id", "quote"}, {"1", "She said \"hi\"."}}, "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n"},
	{"only-a-quote", rows{{"q"}, {"\""}}, "q\r\n\"\"\"\"\r\n"},
	{"header-only", rows{{"name", "age"}}, "name,age\r\n"},
	{"newline-in-field", rows{{"id", "address"}, {"1", "12 High St\nLeeds"}}, "id,address\r\n1,\"12 High St\nLeeds\"\r\n"},
	{"crlf-in-field", rows{{"id", "address"}, {"1", "12 High St\r\nLeeds"}}, "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n"},
	{"cr-in-field", rows{{"id", "text"}, {"1", "a\rb"}}, "id,text\r\n1,\"a\rb\"\r\n"},
	{"empty-fields", rows{{"a", "b", "c"}, {"", "", ""}}, "a,b,c\r\n,,\r\n"},
	{"one-empty-field", rows{{"note"}, {""}, {"x"}}, "note\r\n\"\"\r\nx\r\n"},
	{"spaces", rows{{"a", "b"}, {" x ", " y"}}, "a,b\r\n x , y\r\n"},
	{"no-records", rows{}, ""},
	{"non-ascii", rows{{"city"}, {"Zürich"}, {"東京"}}, "city\r\nZürich\r\n東京\r\n"},
}

func TestFormatAndWrite(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := FormatCSV(c.records)
			if err != nil || got != c.want {
				t.Fatalf("FormatCSV = %q, %v; want %q", got, err, c.want)
			}
			path := filepath.Join(t.TempDir(), "out.csv")
			if err := WriteCSV(path, c.records); err != nil {
				t.Fatal(err)
			}
			data, err := os.ReadFile(path)
			if err != nil || string(data) != c.want {
				t.Fatalf("file = %q, %v; want %q", data, err, c.want)
			}
		})
	}
}

func TestFieldCount(t *testing.T) {
	for name, records := range map[string]rows{
		"too-many-fields": {{"a", "b"}, {"1", "2", "3"}},
		"too-few-fields":  {{"a", "b"}, {"1"}},
	} {
		t.Run(name, func(t *testing.T) {
			var fe *FieldCountError
			if _, err := FormatCSV(records); !errors.As(err, &fe) {
				t.Fatalf("FormatCSV error = %v; want a *FieldCountError", err)
			}
			path := filepath.Join(t.TempDir(), "out.csv")
			if err := WriteCSV(path, records); !errors.As(err, &fe) {
				t.Fatalf("WriteCSV error = %v; want a *FieldCountError", err)
			}
			if _, err := os.Stat(path); !errors.Is(err, os.ErrNotExist) {
				t.Fatalf("the file exists after an error: %v", err)
			}
		})
	}
}
