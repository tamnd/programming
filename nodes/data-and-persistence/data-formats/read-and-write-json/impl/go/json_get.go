// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-json/.

// Package jsonget reads a value from a JSON text and writes it back as canonical JSON.
package jsonget

import (
	"bytes"
	"encoding/json"
	"errors"
	"io"
	"regexp"
	"strconv"
	"strings"
)

// ErrNotFound is the error when the path names no value.
var ErrNotFound = errors.New("no value at the path")

// An array index: ASCII digits, no sign, no leading zero.
var index = regexp.MustCompile(`^(?:0|[1-9][0-9]*)$`)

// JSONGet returns the value at path in the JSON text, as canonical JSON.
func JSONGet(text string, path []string) (string, error) {
	dec := json.NewDecoder(strings.NewReader(text))
	// Keep each number as its text (json.Number), not as a float64.
	dec.UseNumber()
	var value any
	if err := dec.Decode(&value); err != nil {
		if err == io.EOF {
			return "", io.ErrUnexpectedEOF // the text holds no value
		}
		return "", err
	}
	// Decode stops after one value. Only white space may follow it.
	if _, err := dec.Token(); err != io.EOF {
		return "", errors.New("data after the JSON value")
	}
	for _, step := range path {
		switch v := value.(type) {
		case map[string]any:
			next, ok := v[step]
			if !ok {
				return "", ErrNotFound
			}
			value = next
		case []any:
			i, err := strconv.Atoi(step)
			if !index.MatchString(step) || err != nil || i >= len(v) {
				return "", ErrNotFound
			}
			value = v[i]
		default:
			return "", ErrNotFound
		}
	}
	var out bytes.Buffer
	enc := json.NewEncoder(&out)
	// Without this, the encoder writes <, > and & as \u003c, \u003e and \u0026.
	enc.SetEscapeHTML(false)
	// The encoder sorts the keys of a map by their bytes, which is code point order.
	if err := enc.Encode(normalize(value)); err != nil {
		return "", err
	}
	return strings.TrimSuffix(out.String(), "\n"), nil
}

// normalize writes -0 as 0. A json.Number keeps the text of the number as it is.
func normalize(value any) any {
	switch v := value.(type) {
	case map[string]any:
		for k, e := range v {
			v[k] = normalize(e)
		}
	case []any:
		for i, e := range v {
			v[i] = normalize(e)
		}
	case json.Number:
		if v == "-0" {
			return json.Number("0")
		}
	}
	return value
}
