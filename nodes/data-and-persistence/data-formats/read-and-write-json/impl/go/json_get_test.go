// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-json/.

package jsonget

import (
	"errors"
	"testing"
)

type errorCase struct {
	name, text string
	path       []string
}

func TestCases(t *testing.T) {
	cases := []struct {
		name, text string
		path       []string
		want       string
	}{
		{"member", "{\"name\": \"Ada\", \"born\": 1815}", []string{"name"}, "\"Ada\""},
		{"whole-document", "{\"b\": [1, 2], \"a\": true}", nil, "{\"a\":true,\"b\":[1,2]}"},
		{"nested-path", "{\"user\": {\"tags\": [\"x\", \"y\"]}}", []string{"user", "tags", "1"}, "\"y\""},
		{"big-int", "{\"id\": 9007199254740993}", []string{"id"}, "9007199254740993"},
		{"beyond-int64", "[18446744073709551617, -18446744073709551617]", nil, "[18446744073709551617,-18446744073709551617]"},
		{"int64-bounds", "[-9223372036854775808, 9223372036854775807]", nil, "[-9223372036854775808,9223372036854775807]"},
		{"negative-zero", "[-0, 0]", nil, "[0,0]"},
		{"duplicate-name", "{\"a\": 1, \"a\": 2}", []string{"a"}, "2"},
		{"duplicate-name-whole", "{\"a\": 1, \"b\": 0, \"a\": {\"c\": 3}}", nil, "{\"a\":{\"c\":3},\"b\":0}"},
		{"array-index-10", "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, \"ten\"]", []string{"10"}, "\"ten\""},
		{"numeric-name", "{\"0\": \"zero\", \"1\": \"one\"}", []string{"0"}, "\"zero\""},
		{"key-order-ascii", "{\"b\": 1, \"a\": 2, \"B\": 3, \"10\": 4, \"9\": 5}", nil, "{\"10\":4,\"9\":5,\"B\":3,\"a\":2,\"b\":1}"},
		{"key-order-prefix", "{\"ab\": 1, \"a\": 2, \"\": 3}", nil, "{\"\":3,\"a\":2,\"ab\":1}"},
		{"key-order-astral", "{\"😀\": 1, \"ﬁ\": 2, \"é\": 3}", nil, "{\"é\":3,\"ﬁ\":2,\"😀\":1}"},
		{"non-ascii", "[\"Zürich\", \"日本語\", \"😀\"]", nil, "[\"Zürich\",\"日本語\",\"😀\"]"},
		{"escaped-non-ascii", "[\"\\u00e9\\ud83d\\ude00\"]", []string{"0"}, "\"é😀\""},
		{"escapes", "[\"q\\\" b\\\\ s\\/ t\\t n\\n r\\r\"]", nil, "[\"q\\\" b\\\\ s/ t\\t n\\n r\\r\"]"},
		{"control-chars", "[\"\\u0000\\u001f\\b\\f\\u007f\"]", nil, "[\"\\u0000\\u001f\\b\\f\x7f\"]"},
		{"html-chars", "{\"html\": \"<a href='x'>&amp;</a>\"}", []string{"html"}, "\"<a href='x'>&amp;</a>\""},
		{"empty-containers", "{\"a\": {}, \"b\": [], \"c\": \"\"}", nil, "{\"a\":{},\"b\":[],\"c\":\"\"}"},
		{"scalar-root", "42", nil, "42"},
		{"string-root", "\"text\"", nil, "\"text\""},
		{"whitespace", " \n\ttrue\r\n ", nil, "true"},
		{"whitespace-inside", "\n{ \"a\" :\t[ 1 , 2 ] }\r\n", nil, "{\"a\":[1,2]}"},
		{"deep-path", "{\"a\": [{\"b\": [[1, {\"c\": \"deep\"}]]}]}", []string{"a", "0", "b", "0", "1", "c"}, "\"deep\""},
		{"null-member", "{\"a\": null}", []string{"a"}, "null"},
		{"false-member", "{\"a\": false, \"b\": 0}", []string{"a"}, "false"},
		{"proto-name", "{\"__proto__\": {\"x\": 1}, \"constructor\": 2}", nil, "{\"__proto__\":{\"x\":1},\"constructor\":2}"},
		{"proto-path", "{\"__proto__\": {\"x\": 1}}", []string{"__proto__", "x"}, "1"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := JSONGet(c.text, c.path)
			if err != nil {
				t.Fatal(err)
			}
			if got != c.want {
				t.Errorf("got %q, want %q", got, c.want)
			}
		})
	}
}

func TestInvalidJSON(t *testing.T) {
	cases := []errorCase{
		{"trailing-comma-array", "[1, 2,]", nil},
		{"trailing-comma-object", "{\"a\": 1,}", nil},
		{"single-quotes", "{'a': 1}", nil},
		{"unquoted-name", "{a: 1}", nil},
		{"nan", "[NaN]", nil},
		{"infinity", "[Infinity]", nil},
		{"negative-infinity", "-Infinity", nil},
		{"leading-zero", "[01]", nil},
		{"plus-sign", "+1", nil},
		{"hex-number", "0x10", nil},
		{"empty-text", "", nil},
		{"whitespace-only", " \n", nil},
		{"trailing-data", "[1] x", nil},
		{"two-values", "1 2", nil},
		{"comment", "{\"a\": 1} // note", nil},
		{"byte-order-mark", "\ufeff{\"a\": 1}", nil},
		{"raw-control-char", "[\"a\tb\"]", nil},
		{"bad-escape", "[\"\\x41\"]", nil},
		{"unclosed", "[1, 2", nil},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			_, err := JSONGet(c.text, c.path)
			if err == nil || errors.Is(err, ErrNotFound) {
				t.Errorf("got error %v, want a syntax error", err)
			}
		})
	}
}

func TestNotFound(t *testing.T) {
	cases := []errorCase{
		{"missing-name", "{\"a\": 1}", []string{"b"}},
		{"inherited-name", "{\"a\": 1}", []string{"toString"}},
		{"array-length", "[1, 2]", []string{"length"}},
		{"index-out-of-range", "[1, 2]", []string{"2"}},
		{"index-leading-zero", "[1, 2]", []string{"01"}},
		{"index-negative", "[1, 2]", []string{"-1"}},
		{"index-plus", "[1, 2]", []string{"+1"}},
		{"index-non-ascii-digit", "[1, 2]", []string{"١"}},
		{"through-scalar", "{\"a\": 1}", []string{"a", "b"}},
		{"through-null", "{\"a\": null}", []string{"a", "b"}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if _, err := JSONGet(c.text, c.path); !errors.Is(err, ErrNotFound) {
				t.Errorf("got error %v, want ErrNotFound", err)
			}
		})
	}
}
