// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/validate-data-against-a-schema/.

// Package schema checks a JSON value against a small part of JSON Schema 2020-12.
package schema

import (
	"cmp"
	"encoding/json"
	"math"
	"slices"
	"strconv"
	"strings"
	"unicode/utf8"
)

// Failure is the instance location and the keyword location of one failed assertion.
type Failure [2]string

// RFC 6901: "~" becomes "~0" first, then "/" becomes "~1".
var escaper = strings.NewReplacer("~", "~0", "/", "~1")

func pointer(name string) string { return "/" + escaper.Replace(name) }

func kinds(value any) []string {
	switch v := value.(type) {
	case nil:
		return []string{"null"}
	case bool:
		return []string{"boolean"}
	case float64:
		if v == math.Trunc(v) {
			return []string{"integer", "number"}
		}
		return []string{"number"}
	case string:
		return []string{"string"}
	case []any:
		return []string{"array"}
	}
	return []string{"object"}
}

func check(schema, value any, here, at string, errors *[]Failure) {
	switch s := schema.(type) {
	case bool:
		if !s {
			*errors = append(*errors, Failure{here, at})
		}
		return
	case map[string]any:
		if t, ok := s["type"]; ok {
			allowed, _ := t.([]any)
			if name, ok := t.(string); ok {
				allowed = []any{name}
			}
			if !slices.ContainsFunc(kinds(value), func(k string) bool { return slices.Contains(allowed, any(k)) }) {
				*errors = append(*errors, Failure{here, at + "/type"})
			}
		}
		switch v := value.(type) {
		case float64:
			if m, ok := s["minimum"].(float64); ok && v < m {
				*errors = append(*errors, Failure{here, at + "/minimum"})
			}
			if m, ok := s["maximum"].(float64); ok && v > m {
				*errors = append(*errors, Failure{here, at + "/maximum"})
			}
		case string:
			// len counts bytes. RuneCountInString counts code points.
			n := float64(utf8.RuneCountInString(v))
			if m, ok := s["minLength"].(float64); ok && n < m {
				*errors = append(*errors, Failure{here, at + "/minLength"})
			}
			if m, ok := s["maxLength"].(float64); ok && n > m {
				*errors = append(*errors, Failure{here, at + "/maxLength"})
			}
		case map[string]any:
			required, _ := s["required"].([]any)
			for _, name := range required {
				if _, ok := v[name.(string)]; !ok {
					*errors = append(*errors, Failure{here, at + "/required"})
					break
				}
			}
			named, _ := s["properties"].(map[string]any)
			extra, hasExtra := s["additionalProperties"]
			for name, member := range v {
				if sub, ok := named[name]; ok {
					check(sub, member, here+pointer(name), at+"/properties"+pointer(name), errors)
				} else if hasExtra {
					check(extra, member, here+pointer(name), at+"/additionalProperties", errors)
				}
			}
		case []any:
			if items, ok := s["items"]; ok {
				for i, item := range v {
					check(items, item, here+"/"+strconv.Itoa(i), at+"/items", errors)
				}
			}
		}
	}
}

// Validate returns the sorted failures of instance against schema. Both are JSON texts.
func Validate(schema, instance string) ([]Failure, error) {
	var s, v any
	if err := json.Unmarshal([]byte(schema), &s); err != nil {
		return nil, err
	}
	if err := json.Unmarshal([]byte(instance), &v); err != nil {
		return nil, err
	}
	errors := []Failure{}
	check(s, v, "", "", &errors)
	// A map has no order, so the sort makes the result the same on each run.
	slices.SortFunc(errors, func(a, b Failure) int { return cmp.Or(cmp.Compare(a[0], b[0]), cmp.Compare(a[1], b[1])) })
	return errors, nil
}
