```go run id=to-pointer
package main

import (
	"fmt"
	"strconv"
	"strings"
)

// toPointer converts an RFC 9535 Normalized Path, such as $['a'][0], into an RFC 6901 JSON Pointer.
func toPointer(path string) (string, error) {
	if !strings.HasPrefix(path, "$") {
		return "", fmt.Errorf("no root identifier in %q", path)
	}
	var b strings.Builder
	rest := path[1:]
	for rest != "" {
		if !strings.HasPrefix(rest, "[") {
			return "", fmt.Errorf("expected [ at %q", rest)
		}
		rest = rest[1:]
		var token string
		if strings.HasPrefix(rest, "'") {
			name, n, err := unquote(rest)
			if err != nil {
				return "", err
			}
			token, rest = name, rest[n:]
		} else {
			end := strings.IndexByte(rest, ']')
			if end < 0 {
				return "", fmt.Errorf("no ] in %q", rest)
			}
			if n, err := strconv.ParseUint(rest[:end], 10, 64); err != nil || strconv.FormatUint(n, 10) != rest[:end] {
				return "", fmt.Errorf("bad index %q", rest[:end])
			}
			token, rest = rest[:end], rest[end:]
		}
		if !strings.HasPrefix(rest, "]") {
			return "", fmt.Errorf("expected ] at %q", rest)
		}
		rest = rest[1:]
		token = strings.ReplaceAll(strings.ReplaceAll(token, "~", "~0"), "/", "~1")
		b.WriteString("/" + token)
	}
	return b.String(), nil
}

// unquote decodes a 'name' with the escapes of a Normalized Path. It returns the name and the bytes used.
func unquote(s string) (string, int, error) {
	escapes := map[byte]string{'b': "\b", 'f': "\f", 'n': "\n", 'r': "\r", 't': "\t", '\'': "'", '\\': "\\"}
	var b strings.Builder
	for i := 1; i < len(s); i++ {
		switch s[i] {
		case '\'':
			return b.String(), i + 1, nil
		case '\\':
			if i+1 >= len(s) {
				return "", 0, fmt.Errorf("escape at end of %q", s)
			}
			if r, ok := escapes[s[i+1]]; ok {
				b.WriteString(r)
				i++
			} else if s[i+1] == 'u' && i+5 < len(s) {
				code, err := strconv.ParseUint(s[i+2:i+6], 16, 8)
				if err != nil || code >= 0x20 {
					return "", 0, fmt.Errorf("bad escape in %q", s)
				}
				b.WriteByte(byte(code))
				i += 5
			} else {
				return "", 0, fmt.Errorf("bad escape in %q", s)
			}
		default:
			b.WriteByte(s[i])
		}
	}
	return "", 0, fmt.Errorf("no closing quote in %q", s)
}

func main() {
	for _, p := range []string{`$['store']['book'][0]['title']`, `$['a/b']['m~n']`, `$['it\'s']['\u000b']`, `$`, `$[-1]`, `$[01]`, `$.store`} {
		pointer, err := toPointer(p)
		if err != nil {
			fmt.Println("error:", err)
			continue
		}
		fmt.Printf("%-32s %q\n", p, pointer)
	}
}
```

```output for=to-pointer
$['store']['book'][0]['title']   "/store/book/0/title"
$['a/b']['m~n']                  "/a~1b/m~0n"
$['it\'s']['\u000b']             "/it's/\v"
$                                ""
error: bad index "-1"
error: bad index "01"
error: expected [ at ".store"
```

- Verdict: stdlib. The Go standard library has no JSONPath, but the package `strings` and the package `strconv` are enough to convert a Normalized Path into a JSON Pointer. The conversion needs no JSON value, because a Normalized Path has only names in single quotes and non-negative indexes. The function decodes the escapes of the path, such as `\'` and `\u000b`, and then applies the escapes of the pointer, `~0` and `~1`. It rejects `$[-1]` and `$[01]`, which are valid queries but not Normalized Paths, and the dot notation `$.store`. The reverse conversion needs the value, because the token `0` in a pointer can name an array element or an object member.
