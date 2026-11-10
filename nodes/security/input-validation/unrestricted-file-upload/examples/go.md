```go run id=detect-content-type
package main

import (
	"bytes"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"io"
	"mime/multipart"
	"net/http"
	"net/http/httptest"
	"os"
	"path/filepath"
	"strings"
)

const maxBytes = 1 << 20

// allowed maps a detected type to the extension that the server gives the file.
var allowed = map[string]string{"image/png": ".png", "image/jpeg": ".jpg", "image/gif": ".gif"}

func uploadHandler(store string) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		r.Body = http.MaxBytesReader(w, r.Body, maxBytes)
		file, _, err := r.FormFile("file")
		if err != nil {
			http.Error(w, "no file or too large", http.StatusBadRequest)
			return
		}
		defer file.Close()
		data, err := io.ReadAll(file)
		if err != nil {
			http.Error(w, "read error", http.StatusBadRequest)
			return
		}
		// The type comes from the bytes, not from the client's name or header.
		detected := http.DetectContentType(data)
		ext, ok := allowed[detected]
		if !ok {
			http.Error(w, "type "+detected+" is not allowed", http.StatusUnsupportedMediaType)
			return
		}
		sum := sha256.Sum256(data)
		name := hex.EncodeToString(sum[:8]) + ext
		if err := os.WriteFile(filepath.Join(store, name), data, 0o600); err != nil {
			http.Error(w, "cannot save", http.StatusInternalServerError)
			return
		}
		fmt.Fprintf(w, "saved as %s", name)
	}
}

func upload(h http.Handler, filename string, data []byte) {
	var body bytes.Buffer
	mw := multipart.NewWriter(&body)
	part, _ := mw.CreateFormFile("file", filename)
	part.Write(data)
	mw.Close()
	req := httptest.NewRequest("POST", "/upload", &body)
	req.Header.Set("Content-Type", mw.FormDataContentType())
	rec := httptest.NewRecorder()
	h.ServeHTTP(rec, req)
	fmt.Printf("%-14s %d %s\n", filename, rec.Code, strings.TrimSpace(rec.Body.String()))
}

func main() {
	// The store is a private directory outside the web root.
	store, err := os.MkdirTemp("", "uploads")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(store)
	h := uploadHandler(store)

	png := append([]byte("\x89PNG\r\n\x1a\n"), make([]byte, 16)...)
	upload(h, "notes.html", []byte("<h1>hello</h1>"))
	upload(h, "notes.png", []byte("<h1>hello</h1>"))
	upload(h, "photo.php.png", png)
	upload(h, "big.png", append(png, make([]byte, maxBytes)...))
}
```

```output for=detect-content-type
notes.html     415 type text/html; charset=utf-8 is not allowed
notes.png      415 type text/html; charset=utf-8 is not allowed
photo.php.png  200 saved as f731c2528f4184b3.png
big.png        400 no file or too large
```

The handler ignores the name and the Content-Type that the client sends. `http.DetectContentType` reads the first bytes, and the allow-list maps the detected type to the extension that the server gives the file. `http.MaxBytesReader` stops the body at 1 MiB.

- Verdict: stdlib. `net/http` parses multipart uploads with `Request.FormFile`, and `DetectContentType` follows the WHATWG MIME Sniffing Standard.
- `DetectContentType` returns `application/octet-stream` when it cannot find a more specific type, so an allow-list rejects unknown files.
- The test requests use `httptest` and run in memory, with no network.
- Serve stored files with `w.Header().Set("X-Content-Type-Options", "nosniff")` and a `Content-Type` from the server's own record.
