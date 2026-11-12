```go run id=string-header-and-shared-bytes
package main

import (
	"fmt"
	"unsafe"
)

func main() {
	// A string value is a pointer to bytes and a length: 16 bytes on a 64-bit system.
	s := "hello, world"
	fmt.Println(unsafe.Sizeof(s), len(s))

	// A substring shares the bytes of the original string. No copy is made.
	sub := s[7:]
	same := unsafe.StringData(sub) == (*byte)(unsafe.Add(unsafe.Pointer(unsafe.StringData(s)), 7))
	fmt.Println(sub, same)

	// A conversion to []byte gives bytes that can change, so the bytes are a copy.
	b := []byte(s)
	b[0] = 'H'
	fmt.Println(string(b), "|", s, unsafe.SliceData(b) == unsafe.StringData(s))

	// The bytes are UTF-8, so non-ASCII text takes more bytes than characters.
	for _, t := range []string{"café", "日本", "😀"} {
		fmt.Printf("%s: bytes=%d % x\n", t, len(t), t)
	}
}
```

```output for=string-header-and-shared-bytes
16 12
world true
Hello, world | hello, world false
café: bytes=5 63 61 66 c3 a9
日本: bytes=6 e6 97 a5 e6 9c ac
😀: bytes=4 f0 9f 98 80
```

- Verdict: native. A string value is 16 bytes on a 64-bit system: a pointer and a length. The substring `s[7:]` points 7 bytes into the same memory, so it costs no copy. A `[]byte` that the program changes is a copy, so `s` stays as it was. The bytes are UTF-8: 5 for `café`, 6 for `日本` and 4 for the emoji.
