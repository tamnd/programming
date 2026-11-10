// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

package readcsv

import (
	"encoding/csv"
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"reflect"
	"testing"
)

type rec = map[string]string

func write(t *testing.T, data string) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "input.csv")
	if err := os.WriteFile(path, []byte(data), 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestCases(t *testing.T) {
	adaAlan := []rec{{"name": "Ada", "age": "36"}, {"name": "Alan", "age": "41"}}
	cases := []struct {
		name, data string
		want       []rec
	}{
		{"simple", "name,age\nAda,36\nAlan,41\n", adaAlan},
		{"crlf", "name,age\r\nAda,36\r\nAlan,41\r\n", adaAlan},
		{"no-final-newline", "name,age\nAda,36\nAlan,41", adaAlan},
		{"quoted-comma", "name,born\n\"Lovelace, Ada\",1815\n", []rec{{"name": "Lovelace, Ada", "born": "1815"}}},
		{"escaped-quote", "id,quote\n1,\"She said \"\"hi\"\".\"\n", []rec{{"id": "1", "quote": `She said "hi".`}}},
		{"quoted-header", "\"first name\",\"last name\"\nAda,Lovelace\n", []rec{{"first name": "Ada", "last name": "Lovelace"}}},
		{"newline-in-field", "id,address\n1,\"12 High St\nLeeds\"\n", []rec{{"id": "1", "address": "12 High St\nLeeds"}}},
		{"crlf-in-field", "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n", []rec{{"id": "1", "address": "12 High St\nLeeds"}}},
		{"blank-line-in-field", "id,note\n1,\"one\n\ntwo\"\n", []rec{{"id": "1", "note": "one\n\ntwo"}}},
		{"empty-last-line", "a,b\n1,2\n\n", []rec{{"a": "1", "b": "2"}}},
		{"blank-lines", "a,b\n\n1,2\n\n\n3,4\n", []rec{{"a": "1", "b": "2"}, {"a": "3", "b": "4"}}},
		{"empty-fields", "a,b,c\n,,\n", []rec{{"a": "", "b": "", "c": ""}}},
		{"quoted-empty", "name\n\"\"\n", []rec{{"name": ""}}},
		{"spaces", "a,b\n x , y \n", []rec{{"a": " x ", "b": " y "}}},
		{"header-only", "a,b\n", []rec{}},
		{"empty-file", "", []rec{}},
		{"bom", "\uFEFFname,age\nAda,36\n", []rec{{"name": "Ada", "age": "36"}}},
		{"non-ascii", "city,country\nZürich,Schweiz\n東京,日本\n", []rec{{"city": "Zürich", "country": "Schweiz"}, {"city": "東京", "country": "日本"}}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ReadCSV(write(t, c.data))
			if err != nil || !reflect.DeepEqual(got, c.want) {
				t.Errorf("ReadCSV = %q, %v; want %q, nil", got, err, c.want)
			}
		})
	}
}

func TestInvalid(t *testing.T) {
	cases := []struct {
		name, data string
		want       error
	}{
		{"too-few-fields", "a,b\n1\n", csv.ErrFieldCount},
		{"too-many-fields", "a,b\n1,2,3\n", csv.ErrFieldCount},
		{"unclosed-quote", "a,b\n1,\"two\n", csv.ErrQuote},
		{"text-after-quote", "a,b\n1,\"two\"x\n", csv.ErrQuote},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if _, err := ReadCSV(write(t, c.data)); !errors.Is(err, c.want) {
				t.Errorf("err = %v, want %v", err, c.want)
			}
		})
	}
}

func TestMissing(t *testing.T) {
	if _, err := ReadCSV(filepath.Join(t.TempDir(), "missing.csv")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("err = %v, want fs.ErrNotExist", err)
	}
}
