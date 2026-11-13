// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/validate-data-against-a-schema/.

package schema

import (
	"slices"
	"testing"
)

func TestCases(t *testing.T) {
	cases := []struct {
		name, schema, instance string
		want                   []Failure
	}{
		{"valid-object", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\", \"age\": 36}", []Failure{}},
		{"missing-required", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\"}", []Failure{{"", "/required"}}},
		{"wrong-member-type", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\", \"age\": \"36\"}", []Failure{{"/age", "/properties/age/type"}}},
		{"below-minimum", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"Ada\", \"age\": -1}", []Failure{{"/age", "/properties/age/minimum"}}},
		{"two-errors", "{\"type\": \"object\", \"required\": [\"name\", \"age\"], \"properties\": {\"name\": {\"type\": \"string\", \"minLength\": 1}, \"age\": {\"type\": \"integer\", \"minimum\": 0}}}", "{\"name\": \"\", \"age\": 3.5}", []Failure{{"/age", "/properties/age/type"}, {"/name", "/properties/name/minLength"}}},
		{"array-items", "{\"type\": \"array\", \"items\": {\"type\": \"string\"}}", "[\"a\", 1, \"c\", null]", []Failure{{"/1", "/items/type"}, {"/3", "/items/type"}}},
		{"no-additional", "{\"properties\": {\"id\": {\"type\": \"integer\"}}, \"additionalProperties\": false}", "{\"id\": 1, \"admin\": true}", []Failure{{"/admin", "/additionalProperties"}}},
		{"integer-from-fraction-zero", "{\"type\": \"integer\"}", "1.0", []Failure{}},
		{"integer-from-exponent", "{\"type\": \"integer\"}", "1e2", []Failure{}},
		{"integer-with-fraction", "{\"type\": \"integer\"}", "3.5", []Failure{{"", "/type"}}},
		{"boolean-not-integer", "{\"type\": \"integer\"}", "true", []Failure{{"", "/type"}}},
		{"boolean-not-number", "{\"type\": \"number\"}", "false", []Failure{{"", "/type"}}},
		{"integer-is-number", "{\"type\": \"number\"}", "3", []Failure{}},
		{"null-not-object", "{\"type\": \"object\"}", "null", []Failure{{"", "/type"}}},
		{"array-not-object", "{\"type\": \"object\"}", "[]", []Failure{{"", "/type"}}},
		{"type-list-match", "{\"type\": [\"string\", \"null\"]}", "null", []Failure{}},
		{"type-list-miss", "{\"type\": [\"string\", \"null\"]}", "5", []Failure{{"", "/type"}}},
		{"max-length-astral", "{\"maxLength\": 1}", "\"😀\"", []Failure{}},
		{"min-length-astral", "{\"minLength\": 2}", "\"\\ud83d\\ude00\"", []Failure{{"", "/minLength"}}},
		{"combining-mark", "{\"maxLength\": 1}", "\"e\\u0301\"", []Failure{{"", "/maxLength"}}},
		{"other-type-keywords", "{\"minimum\": 5, \"required\": [\"a\"], \"items\": false}", "\"abc\"", []Failure{}},
		{"other-type-keywords-number", "{\"maxLength\": 1, \"required\": [\"a\"], \"additionalProperties\": false}", "3", []Failure{}},
		{"unknown-keyword", "{\"minimun\": 5}", "1", []Failure{}},
		{"format-is-annotation", "{\"type\": \"string\", \"format\": \"email\"}", "\"not an email\"", []Failure{}},
		{"true-schema", "true", "{\"a\": [1]}", []Failure{}},
		{"false-schema", "false", "{}", []Failure{{"", ""}}},
		{"empty-schema", "{}", "null", []Failure{}},
		{"false-member-schema", "{\"properties\": {\"a\": false}}", "{\"a\": 0, \"b\": 0}", []Failure{{"/a", "/properties/a"}}},
		{"pointer-escape", "{\"properties\": {\"a/b\": {\"type\": \"string\"}, \"m~n\": {\"type\": \"string\"}}}", "{\"a/b\": 1, \"m~n\": 2}", []Failure{{"/a~1b", "/properties/a~1b/type"}, {"/m~0n", "/properties/m~0n/type"}}},
		{"pointer-escape-order", "{\"properties\": {\"~1\": false}}", "{\"~1\": 0}", []Failure{{"/~01", "/properties/~01"}}},
		{"nested-items", "{\"items\": {\"items\": {\"type\": \"integer\"}}}", "[[1, \"x\"], [2]]", []Failure{{"/0/1", "/items/items/type"}}},
		{"additional-schema", "{\"properties\": {\"id\": {}}, \"additionalProperties\": {\"type\": \"string\"}}", "{\"id\": 1, \"a\": \"x\", \"b\": 2}", []Failure{{"/b", "/additionalProperties/type"}}},
		{"absent-property", "{\"properties\": {\"a\": {\"type\": \"string\"}}}", "{}", []Failure{}},
		{"required-all-missing", "{\"required\": [\"a\", \"b\", \"c\"]}", "{}", []Failure{{"", "/required"}}},
		{"required-null-member", "{\"required\": [\"a\"]}", "{\"a\": null}", []Failure{}},
		{"inclusive-bounds", "{\"items\": {\"minimum\": 0, \"maximum\": 10}}", "[0, 10, 10.5, -0.5]", []Failure{{"/2", "/items/maximum"}, {"/3", "/items/minimum"}}},
		{"negative-zero", "{\"minimum\": 0}", "-0", []Failure{}},
		{"fraction-minimum", "{\"minimum\": 0.5}", "0.4", []Failure{{"", "/minimum"}}},
		{"index-order", "{\"items\": {\"type\": \"string\"}}", "[0, \"\", 0, \"\", \"\", \"\", \"\", \"\", \"\", \"\", 0]", []Failure{{"/0", "/items/type"}, {"/10", "/items/type"}, {"/2", "/items/type"}}},
		{"keyword-order", "{\"type\": \"string\", \"minimum\": 5}", "3", []Failure{{"", "/minimum"}, {"", "/type"}}},
		{"inherited-name", "{\"required\": [\"toString\", \"constructor\"]}", "{}", []Failure{{"", "/required"}}},
		{"proto-member", "{\"properties\": {\"__proto__\": {\"type\": \"string\"}}}", "{\"__proto__\": 1}", []Failure{{"/__proto__", "/properties/__proto__/type"}}},
		{"non-ascii-name", "{\"required\": [\"é\"], \"properties\": {\"日本\": {\"type\": \"integer\"}}}", "{\"é\": 1, \"日本\": \"x\"}", []Failure{{"/日本", "/properties/日本/type"}}},
		{"deep-path", "{\"properties\": {\"a\": {\"items\": {\"properties\": {\"b\": {\"required\": [\"c\"]}}}}}}", "{\"a\": [{\"b\": {}}, {\"b\": {\"c\": 1}}, {\"b\": {}}]}", []Failure{{"/a/0/b", "/properties/a/items/properties/b/required"}, {"/a/2/b", "/properties/a/items/properties/b/required"}}},
	}
	for _, c := range cases {
		got, err := Validate(c.schema, c.instance)
		if err != nil || !slices.Equal(got, c.want) {
			t.Errorf("%s: got %q, %v; want %q", c.name, got, err, c.want)
		}
	}
}

func TestInvalidJSON(t *testing.T) {
	// One of the two texts is not JSON.
	cases := []struct{ name, schema, instance string }{
		{"trailing-comma", "{\"type\": \"array\"}", "[1, 2,]"},
		{"nan", "{\"type\": \"number\"}", "NaN"},
		{"empty-instance", "{}", ""},
		{"bad-schema", "{\"type\": ", "1"},
		{"single-quotes", "{}", "{'a': 1}"},
	}
	for _, c := range cases {
		if _, err := Validate(c.schema, c.instance); err == nil {
			t.Errorf("%s: no error", c.name)
		}
	}
}
