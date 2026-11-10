// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

package sha256hash

import (
	"bytes"
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

var textCases = []struct {
	name, text, want string
}{
	{"abc", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
	{"two-block-56", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"},
	{"fox", "The quick brown fox jumps over the lazy dog", "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"},
	{"empty", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
	{"a-55", strings.Repeat("a", 55), "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318"},
	{"a-56", strings.Repeat("a", 56), "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a"},
	{"a-63", strings.Repeat("a", 63), "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34"},
	{"a-64", strings.Repeat("a", 64), "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb"},
	{"multi-block-112", "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1"},
	{"non-ascii", "naïve café 日本語 \U0001F642", "9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d"},
	{"nfc-cafe", "caf\u00e9", "850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e"},
	{"nfd-cafe", "cafe\u0301", "81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a"},
	{"trailing-newline", "abc\n", "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"},
}

// From cases/files.jsonl.
var fileCases = []struct {
	name string
	data []byte
	want string
}{
	{"abc-file", []byte("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
	{"abc-newline-file", []byte("abc\n"), "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"},
	{"abc-crlf-file", []byte("abc\r\n"), "552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025"},
	{"empty-file", []byte{}, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
	{"binary-file", []byte{0x00, 0x01, 0xff}, "26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092"},
	// One million repetitions of "a": more than one chunk.
	{"million-a-file", bytes.Repeat([]byte("a"), 1_000_000), "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"},
}

func TestSha256Text(t *testing.T) {
	for _, c := range textCases {
		t.Run(c.name, func(t *testing.T) {
			if got := Sha256Text(c.text); got != c.want {
				t.Errorf("got %s, want %s", got, c.want)
			}
		})
	}
}

func TestSha256File(t *testing.T) {
	for _, c := range fileCases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), "in.bin")
			if err := os.WriteFile(path, c.data, 0o644); err != nil {
				t.Fatal(err)
			}
			got, err := Sha256File(path)
			if err != nil {
				t.Fatal(err)
			}
			if got != c.want {
				t.Errorf("got %s, want %s", got, c.want)
			}
		})
	}
}

func TestMissingFile(t *testing.T) {
	_, err := Sha256File(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got error %v, want fs.ErrNotExist", err)
	}
}
