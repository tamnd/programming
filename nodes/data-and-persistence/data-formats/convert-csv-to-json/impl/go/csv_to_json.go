// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-csv-to-json/.

// Package csvtojson converts CSV text with a header line to a JSON array of objects.
package csvtojson

import (
	"bytes"
	"encoding/csv"
	"encoding/json"
	"errors"
	"strings"
)

// CSVError reports a text that is not valid CSV. Kind is "field-count" or "bad-quote".
type CSVError struct {
	Kind string
	Err  error
}

func (e *CSVError) Error() string { return e.Kind + ": " + e.Err.Error() }
func (e *CSVError) Unwrap() error { return e.Err }

// CSVToJSON returns a JSON array with one object for each record after the header.
func CSVToJSON(text string) (string, error) {
	// The reader skips empty lines, changes CRLF to LF, also inside quoted fields,
	// and with FieldsPerRecord at 0 it wants the number of fields of the header in each record.
	records, err := csv.NewReader(strings.NewReader(text)).ReadAll()
	if errors.Is(err, csv.ErrFieldCount) {
		return "", &CSVError{"field-count", err}
	}
	if err != nil {
		return "", &CSVError{"bad-quote", err}
	}
	var out bytes.Buffer
	// json.Marshal of a map sorts the names, so the code writes each object itself.
	// SetEscapeHTML(false) writes <, > and & as themselves.
	enc := json.NewEncoder(&out)
	enc.SetEscapeHTML(false)
	out.WriteByte('[')
	for n, record := range records {
		if n == 0 {
			continue // the header
		}
		if n > 1 {
			out.WriteByte(',')
		}
		out.WriteByte('{')
		for i, field := range record {
			if i > 0 {
				out.WriteByte(',')
			}
			writeString(enc, &out, records[0][i])
			out.WriteByte(':')
			writeString(enc, &out, field)
		}
		out.WriteByte('}')
	}
	out.WriteByte(']')
	return out.String(), nil
}

// writeString writes s as a JSON string. Encode adds a newline, and the code removes it.
func writeString(enc *json.Encoder, out *bytes.Buffer, s string) {
	_ = enc.Encode(s) // Encode cannot fail for a string on a bytes.Buffer.
	out.Truncate(out.Len() - 1)
}
