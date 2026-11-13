// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

// Package writecsv writes records as a CSV file of RFC 4180.
package writecsv

import (
	"fmt"
	"os"
	"strings"
)

// FieldCountError reports a record with more or fewer fields than the header.
type FieldCountError struct {
	Record, Got, Want int
}

func (e *FieldCountError) Error() string {
	return fmt.Sprintf("record %d has %d fields, the header has %d", e.Record, e.Got, e.Want)
}

// FormatCSV returns the CSV text of the records. The first record is the header.
// It does not use csv.Writer from encoding/csv, because that writer writes a
// record with one empty field as an empty line and changes line breaks in fields.
func FormatCSV(records [][]string) (string, error) {
	var b strings.Builder
	for n, record := range records {
		if len(record) != len(records[0]) {
			return "", &FieldCountError{n, len(record), len(records[0])}
		}
		for i, field := range record {
			if i > 0 {
				b.WriteByte(',')
			}
			// A record with one empty field needs quotes, or the line is empty and a reader skips it.
			if strings.ContainsAny(field, ",\"\r\n") || (len(record) == 1 && field == "") {
				b.WriteByte('"')
				b.WriteString(strings.ReplaceAll(field, `"`, `""`))
				b.WriteByte('"')
			} else {
				b.WriteString(field)
			}
		}
		b.WriteString("\r\n")
	}
	return b.String(), nil
}

// WriteCSV writes the records to the file at path as UTF-8, with no byte order mark.
func WriteCSV(path string, records [][]string) error {
	text, err := FormatCSV(records)
	if err != nil {
		return err
	}
	return os.WriteFile(path, []byte(text), 0o644)
}
