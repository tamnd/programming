// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pretty-print-json/.

package prettyjson

import "testing"

func TestCases(t *testing.T) {
	cases := []struct {
		name, text, want string
	}{
		{"object", "{\"name\": \"Ada\", \"born\": 1815, \"tags\": [\"math\", \"code\"]}", "{\n  \"born\": 1815,\n  \"name\": \"Ada\",\n  \"tags\": [\n    \"math\",\n    \"code\"\n  ]\n}"},
		{"nested", "{\"user\": {\"id\": 7, \"roles\": [\"admin\"]}, \"ok\": true}", "{\n  \"ok\": true,\n  \"user\": {\n    \"id\": 7,\n    \"roles\": [\n      \"admin\"\n    ]\n  }\n}"},
		{"array-of-objects", "[{\"a\": 1}, {\"b\": null}]", "[\n  {\n    \"a\": 1\n  },\n  {\n    \"b\": null\n  }\n]"},
		{"compact-input", "{\"a\":[1,2,{\"b\":false}]}", "{\n  \"a\": [\n    1,\n    2,\n    {\n      \"b\": false\n    }\n  ]\n}"},
		{"key-order-ascii", "{\"b\": 1, \"a\": 2, \"B\": 3, \"10\": 4, \"9\": 5}", "{\n  \"10\": 4,\n  \"9\": 5,\n  \"B\": 3,\n  \"a\": 2,\n  \"b\": 1\n}"},
		{"key-order-prefix", "{\"ab\": 1, \"a\": 2, \"\": 3}", "{\n  \"\": 3,\n  \"a\": 2,\n  \"ab\": 1\n}"},
		{"key-order-astral", "{\"😀\": 1, \"ﬁ\": 2, \"é\": 3}", "{\n  \"é\": 3,\n  \"ﬁ\": 2,\n  \"😀\": 1\n}"},
		{"key-order-nested", "{\"z\": {\"y\": 1, \"x\": 2}, \"a\": [{\"d\": 1, \"c\": 2}]}", "{\n  \"a\": [\n    {\n      \"c\": 2,\n      \"d\": 1\n    }\n  ],\n  \"z\": {\n    \"x\": 2,\n    \"y\": 1\n  }\n}"},
		{"duplicate-name", "{\"a\": 1, \"b\": 0, \"a\": {\"c\": 3}}", "{\n  \"a\": {\n    \"c\": 3\n  },\n  \"b\": 0\n}"},
		{"big-int", "{\"id\": 9007199254740993}", "{\n  \"id\": 9007199254740993\n}"},
		{"beyond-int64", "[18446744073709551617, -18446744073709551617]", "[\n  18446744073709551617,\n  -18446744073709551617\n]"},
		{"negative-zero", "[-0, 0]", "[\n  0,\n  0\n]"},
		{"non-ascii", "[\"Zürich\", \"日本語\", \"😀\"]", "[\n  \"Zürich\",\n  \"日本語\",\n  \"😀\"\n]"},
		{"escaped-non-ascii", "[\"\\u00e9\\ud83d\\ude00\"]", "[\n  \"é😀\"\n]"},
		{"escapes", "[\"q\\\" b\\\\ s\\/ t\\t n\\n r\\r\"]", "[\n  \"q\\\" b\\\\ s/ t\\t n\\n r\\r\"\n]"},
		{"control-chars", "[\"\\u0000\\u001f\\b\\f\\u007f\"]", "[\n  \"\\u0000\\u001f\\b\\f\u007f\"\n]"},
		{"html-chars", "{\"html\": \"<a href='x'>&amp;</a>\"}", "{\n  \"html\": \"<a href='x'>&amp;</a>\"\n}"},
		{"empty-containers", "{\"a\": {}, \"b\": [], \"c\": \"\"}", "{\n  \"a\": {},\n  \"b\": [],\n  \"c\": \"\"\n}"},
		{"empty-object-root", "{}", "{}"},
		{"empty-array-root", "[ ]", "[]"},
		{"scalar-root", "42", "42"},
		{"string-root", "\"text\"", "\"text\""},
		{"literals", "[true, false, null]", "[\n  true,\n  false,\n  null\n]"},
		{"whitespace-inside", "\n{ \"a\" :\t[ 1 , 2 ] }\r\n", "{\n  \"a\": [\n    1,\n    2\n  ]\n}"},
		{"already-pretty", "{\n  \"a\": [\n    1,\n    2\n  ],\n  \"b\": {}\n}", "{\n  \"a\": [\n    1,\n    2\n  ],\n  \"b\": {}\n}"},
		{"deep", "[[[[1]]]]", "[\n  [\n    [\n      [\n        1\n      ]\n    ]\n  ]\n]"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := PrettyJSON(c.text)
			if err != nil {
				t.Fatal(err)
			}
			if got != c.want {
				t.Errorf("got %q, want %q", got, c.want)
			}
			// Pretty-printing the output again gives the same text.
			if again, _ := PrettyJSON(got); again != got {
				t.Errorf("not idempotent: %q", again)
			}
		})
	}
}

func TestInvalidJSON(t *testing.T) {
	cases := []struct{ name, text string }{
		{"trailing-comma-array", "[1, 2,]"},
		{"trailing-comma-object", "{\"a\": 1,}"},
		{"single-quotes", "{'a': 1}"},
		{"unquoted-name", "{a: 1}"},
		{"nan", "[NaN]"},
		{"infinity", "[Infinity]"},
		{"negative-infinity", "-Infinity"},
		{"leading-zero", "[01]"},
		{"plus-sign", "+1"},
		{"empty-text", ""},
		{"whitespace-only", " \n"},
		{"trailing-data", "[1] x"},
		{"two-values", "1 2"},
		{"comment", "{\"a\": 1} // note"},
		{"byte-order-mark", "\ufeff{\"a\": 1}"},
		{"raw-control-char", "[\"a\tb\"]"},
		{"bad-escape", "[\"\\x41\"]"},
		{"unclosed", "[1, 2"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			if got, err := PrettyJSON(c.text); err == nil {
				t.Errorf("got %q and no error", got)
			}
		})
	}
}
