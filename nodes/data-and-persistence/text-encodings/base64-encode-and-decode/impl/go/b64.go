// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

// Package b64 encodes bytes as Base64 text and decodes the text strictly.
package b64

import (
	"encoding/base64"
	"errors"
	"fmt"
	"strings"
)

// Alphabet selects one of the two alphabets of RFC 4648.
type Alphabet int

const (
	Standard Alphabet = iota // section 4, padded with "="
	URL                      // section 5, with no padding
)

// ErrInvalid is the error for text that is not valid Base64.
var ErrInvalid = errors.New("invalid base64 text")

func (a Alphabet) encoding() *base64.Encoding {
	if a == URL {
		return base64.RawURLEncoding
	}
	return base64.StdEncoding
}

// Encode returns the Base64 text of data.
func Encode(data []byte, a Alphabet) string {
	return a.encoding().EncodeToString(data)
}

// Decode returns the bytes of text, or an error that wraps ErrInvalid.
func Decode(text string, a Alphabet) ([]byte, error) {
	// Strict rejects non-zero pad bits, but the decoder still skips
	// "\r" and "\n", so check for them first.
	if strings.ContainsAny(text, "\r\n") {
		return nil, fmt.Errorf("%w: line break in %q", ErrInvalid, text)
	}
	data, err := a.encoding().Strict().DecodeString(text)
	if err != nil {
		return nil, fmt.Errorf("%w: %w", ErrInvalid, err)
	}
	return data, nil
}
