// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

// Package sha256hash computes the SHA-256 of a string or of a file.
package sha256hash

import (
	"crypto/sha256"
	"encoding/hex"
	"io"
	"os"
)

// Sha256Text returns the SHA-256 of the UTF-8 bytes of text, as lowercase hex.
// A Go string holds bytes, so []byte(text) is the UTF-8 encoding of valid text.
func Sha256Text(text string) string {
	sum := sha256.Sum256([]byte(text))
	return hex.EncodeToString(sum[:])
}

// Sha256File returns the SHA-256 of the bytes of the file at path, as
// lowercase hex. io.Copy reads the file in chunks, so a large file does not
// fill the memory.
func Sha256File(path string) (string, error) {
	f, err := os.Open(path)
	if err != nil {
		return "", err
	}
	defer f.Close()
	h := sha256.New()
	if _, err := io.Copy(h, f); err != nil {
		return "", err
	}
	return hex.EncodeToString(h.Sum(nil)), nil
}
