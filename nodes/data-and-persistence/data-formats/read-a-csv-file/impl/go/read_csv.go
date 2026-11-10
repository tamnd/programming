// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

// Package readcsv reads a CSV file of RFC 4180 with a header line.
package readcsv

import (
	"bufio"
	"bytes"
	"encoding/csv"
	"errors"
	"io"
	"os"
)

// ReadCSV returns one map for each record after the header, from each name to
// its field. A bad quote gives a *csv.ParseError that wraps csv.ErrQuote, and a
// record with the wrong number of fields one that wraps csv.ErrFieldCount.
func ReadCSV(path string) ([]map[string]string, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	in := bufio.NewReader(f)
	// encoding/csv does not remove a byte order mark.
	if b, err := in.Peek(3); err == nil && bytes.Equal(b, []byte("\xEF\xBB\xBF")) {
		in.Discard(3)
	}
	r := csv.NewReader(in)
	// FieldsPerRecord 0, the default, takes the count from the first record.
	r.FieldsPerRecord = 0
	header, err := r.Read()
	if errors.Is(err, io.EOF) {
		return []map[string]string{}, nil
	}
	if err != nil {
		return nil, err
	}
	records := []map[string]string{}
	for {
		row, err := r.Read()
		if errors.Is(err, io.EOF) {
			return records, nil
		}
		if err != nil {
			return nil, err
		}
		record := make(map[string]string, len(header))
		for i, name := range header {
			record[name] = row[i]
		}
		records = append(records, record)
	}
}
