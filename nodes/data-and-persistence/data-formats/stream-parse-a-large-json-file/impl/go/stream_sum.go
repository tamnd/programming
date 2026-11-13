// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

// Package streamsum reads a large JSON array one element at a time.
package streamsum

import (
	"bufio"
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
)

// ErrBadRecord is the error when an element is not an object with a number member amount.
var ErrBadRecord = errors.New("the element has no number member amount")

// StreamSum returns the number of objects in the JSON array in the file, and the sum of their member amount.
func StreamSum(path string) ([2]int64, error) {
	var result [2]int64
	f, err := os.Open(path)
	if err != nil {
		return result, err
	}
	defer f.Close()

	r := bufio.NewReader(f)
	// RFC 8259 allows a parser to ignore a byte order mark. The decoder does not, so skip it here.
	if bom, _ := r.Peek(3); bytes.Equal(bom, []byte("\xef\xbb\xbf")) {
		r.Discard(3)
	}
	dec := json.NewDecoder(r)
	// Keep each number as its text (json.Number), not as a float64.
	dec.UseNumber()

	// Token reads the [ and stops. The decoder then reads one element at a time.
	if tok, err := dec.Token(); err != nil {
		return result, noEOF(err)
	} else if tok != json.Delim('[') {
		return result, fmt.Errorf("the text starts with %v, not [", tok)
	}
	for dec.More() {
		var record any
		if err := dec.Decode(&record); err != nil {
			return result, noEOF(err)
		}
		obj, _ := record.(map[string]any)
		n, ok := obj["amount"].(json.Number)
		if !ok {
			return result, fmt.Errorf("element %d: %w", result[0], ErrBadRecord)
		}
		amount, err := n.Int64()
		if err != nil {
			return result, err
		}
		result[0]++
		result[1] += amount
	}
	if _, err := dec.Token(); err != nil { // the ]
		return result, noEOF(err)
	}
	// Only white space may follow the array.
	if _, err := dec.Token(); err != io.EOF {
		return result, errors.New("data after the JSON array")
	}
	return result, nil
}

// noEOF turns the end of the input into an error that says the text is not complete.
func noEOF(err error) error {
	if err == io.EOF {
		return io.ErrUnexpectedEOF
	}
	return err
}
