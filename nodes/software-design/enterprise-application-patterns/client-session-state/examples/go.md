```go run id=cookie-cart
package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"errors"
	"fmt"
	"strconv"
	"strings"
)

var secret = []byte("server secret, never sent to the client")

func sign(data string) string {
	mac := hmac.New(sha256.New, secret)
	mac.Write([]byte(data))
	return hex.EncodeToString(mac.Sum(nil))
}

// line is one item of the cart. A slice keeps the order of the items.
type line struct {
	item string
	qty  int
}

// encode serializes the session state into the value of a cookie: the data, a bar and the signature.
func encode(cart []line) string {
	parts := make([]string, len(cart))
	for i, l := range cart {
		parts[i] = l.item + ":" + strconv.Itoa(l.qty)
	}
	data := strings.Join(parts, ",")
	return data + "|" + sign(data)
}

// decode checks the signature and gives back the state. A changed cookie gives an error.
func decode(cookie string) ([]line, error) {
	bar := strings.LastIndex(cookie, "|")
	data, tag := cookie[:bar], cookie[bar+1:]
	if !hmac.Equal([]byte(tag), []byte(sign(data))) {
		return nil, errors.New("bad signature")
	}
	var cart []line
	for _, part := range strings.Split(data, ",") {
		if part == "" {
			continue
		}
		item, qty, _ := strings.Cut(part, ":")
		n, err := strconv.Atoi(qty)
		if err != nil {
			return nil, err
		}
		cart = append(cart, line{item, n})
	}
	return cart, nil
}

// handleAdd is one request. The server keeps nothing between requests:
// the state comes in and goes out in the cookie.
func handleAdd(cookie, item string) (string, error) {
	var cart []line
	if cookie != "" {
		var err error
		if cart, err = decode(cookie); err != nil {
			return "", err
		}
	}
	for i := range cart {
		if cart[i].item == item {
			cart[i].qty++
			return encode(cart), nil
		}
	}
	return encode(append(cart, line{item, 1})), nil
}

func show(cookie string) string {
	bar := strings.LastIndex(cookie, "|")
	return fmt.Sprintf("%s|%s... (%d bytes)", cookie[:bar], cookie[bar+1:bar+9], len(cookie))
}

func main() {
	cookie := ""
	for _, item := range []string{"pen", "pen", "ink"} {
		var err error
		if cookie, err = handleAdd(cookie, item); err != nil {
			panic(err)
		}
		fmt.Printf("add %s: cookie %s\n", item, show(cookie))
	}
	tampered := strings.Replace(cookie, "ink:1", "ink:9", 1)
	if _, err := handleAdd(tampered, "pen"); err != nil {
		fmt.Println("a client changes ink:1 to ink:9:", err)
	}
	cart, _ := decode(cookie)
	parts := make([]string, len(cart))
	for i, l := range cart {
		parts[i] = fmt.Sprintf("%s %d", l.item, l.qty)
	}
	fmt.Println("cart:", strings.Join(parts, ", "))
}
```

```output
add pen: cookie pen:1|2c06beca... (70 bytes)
add pen: cookie pen:2|bdba40d9... (70 bytes)
add ink: cookie pen:2,ink:1|8bdcc52b... (76 bytes)
a client changes ink:1 to ink:9: bad signature
cart: pen 2, ink 1
```

- Verdict: stdlib. `hmac.New(sha256.New, secret)` signs the data, and `hmac.Equal` compares the signatures in constant time.
- The cart is a slice of `line` values, not a map, because the order of a range over a map is not defined.
