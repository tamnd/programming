```go run id=claim-report
package main

import (
	"bytes"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"strconv"
	"strings"
)

const threshold = 1024 // payloads larger than this go to the store

var store = map[string][]byte{} // stands for a blob store or a database
var channel []string

func hash(b []byte) string {
	sum := sha256.Sum256(b)
	return "sha256:" + hex.EncodeToString(sum[:])
}

func send(kind string, payload []byte) {
	var message string
	extra := ""
	if len(payload) <= threshold {
		message = kind + ";inline;" + string(payload)
	} else {
		key := hash(payload)
		store[key] = payload // store first, then publish the claim check
		message = fmt.Sprintf("%s;claim;%s;%d", kind, key, len(payload))
		extra = fmt.Sprintf(" for %d bytes in the store", len(payload))
	}
	channel = append(channel, message)
	fmt.Printf("sent %s: %d bytes on the channel%s\n", kind, len(message), extra)
}

func receive(message string) string {
	parts := strings.SplitN(message, ";", 3)
	kind, mode, rest := parts[0], parts[1], parts[2]
	if mode == "inline" {
		return "received " + kind + ": " + rest
	}
	key, sizeText, _ := strings.Cut(rest, ";")
	size, _ := strconv.Atoi(sizeText)
	payload, ok := store[key]
	if !ok {
		return fmt.Sprintf("received %s: claim %s... not found, to the dead letter channel", kind, key[:19])
	}
	if hash(payload) != key || len(payload) != size {
		return "received " + kind + ": payload does not match the claim check"
	}
	return fmt.Sprintf("received %s: %d rows, hash %s ok", kind, bytes.Count(payload, []byte("\n")), key[7:19])
}

func main() {
	var report bytes.Buffer
	for i := 0; i < 12000; i++ {
		fmt.Fprintf(&report, "row %05d;widget;12.50\n", i)
	}
	send("invoice", []byte("INV-17 total 12.50"))
	send("report", report.Bytes())
	for _, m := range channel {
		fmt.Println(receive(m))
	}

	// The lifecycle rule of the store removes the payload; then the broker delivers the claim again.
	clear(store)
	fmt.Println(receive(channel[1]))
}
```

```output
sent invoice: 33 bytes on the channel
sent report: 91 bytes on the channel for 276000 bytes in the store
received invoice: INV-17 total 12.50
received report: 12000 rows, hash 63b98860796d ok
received report: claim sha256:63b98860796d... not found, to the dead letter channel
```

- Verdict: idiom. `sha256.Sum256` and `hex.EncodeToString` make the key, and a map of byte slices stands for the blob store.
- `strings.SplitN(message, ";", 3)` keeps any `;` in an inline payload, and `strings.Cut` splits the key from the size.
- `clear(store)` plays the lifecycle rule that removes the payload.
