// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pretty-print-json/.

// Package prettyjson writes a JSON text indented, with the names of each object sorted.
package prettyjson

import (
	"bytes"
	"encoding/json"
	"errors"
	"io"
	"strings"
)

// PrettyJSON returns the JSON text indented by two spaces, with the names of each object sorted.
func PrettyJSON(text string) (string, error) {
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
	var out bytes.Buffer
	enc := json.NewEncoder(&out)
	// Without this, the encoder writes <, > and & as \u003c, \u003e and \u0026.
	enc.SetEscapeHTML(false)
	enc.SetIndent("", "  ")
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
