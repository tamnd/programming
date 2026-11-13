// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-csv-to-json/.

package csvtojson

import (
	"bufio"
	"encoding/json"
	"errors"
	"os"
	"path/filepath"
	"reflect"
	"testing"
)

type testCase struct {
	Name string `json:"name"`
	Args struct {
		Text string `json:"text"`
	} `json:"args"`
	Expect string `json:"expect"`
	Error  string `json:"error"`
}

func load(t *testing.T, name string) []testCase {
	t.Helper()
	f, err := os.Open(filepath.Join("..", "..", "cases", name))
	if err != nil {
		t.Fatal(err)
	}
	defer f.Close()
	var cases []testCase
	s := bufio.NewScanner(f)
	for s.Scan() {
		if len(s.Bytes()) == 0 {
			continue
		}
		var c testCase
		if err := json.Unmarshal(s.Bytes(), &c); err != nil {
			t.Fatal(err)
		}
		cases = append(cases, c)
	}
	return cases
}

func TestCases(t *testing.T) {
	for _, c := range append(load(t, "basic.jsonl"), load(t, "edge.jsonl")...) {
		t.Run(c.Name, func(t *testing.T) {
			got, err := CSVToJSON(c.Args.Text)
			if err != nil || got != c.Expect {
				t.Fatalf("got %q, %v; want %q", got, err, c.Expect)
			}
		})
	}
}

func TestErrors(t *testing.T) {
	for _, c := range load(t, "errors.jsonl") {
		t.Run(c.Name, func(t *testing.T) {
			_, err := CSVToJSON(c.Args.Text)
			var e *CSVError
			if !errors.As(err, &e) || e.Kind != c.Error {
				t.Fatalf("got %v, want a %s error", err, c.Error)
			}
		})
	}
}

func TestOutputIsJSON(t *testing.T) {
	got, err := CSVToJSON("id,note\n1,\"She said \"\"hi\"\"\nand left\"\n")
	if err != nil {
		t.Fatal(err)
	}
	var v []map[string]string
	if err := json.Unmarshal([]byte(got), &v); err != nil {
		t.Fatal(err)
	}
	want := []map[string]string{{"id": "1", "note": "She said \"hi\"\nand left"}}
	if !reflect.DeepEqual(v, want) {
		t.Fatalf("got %v, want %v", v, want)
	}
}
