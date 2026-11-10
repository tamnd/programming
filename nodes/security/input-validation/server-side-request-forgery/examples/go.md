```go wrong id=get-any-url
package main

import (
	"fmt"
	"io"
	"net/http"
	"net/http/httptest"
)

func preview(url string) string {
	resp, err := http.Get(url)
	if err != nil {
		return "error"
	}
	defer resp.Body.Close()
	body, _ := io.ReadAll(io.LimitReader(resp.Body, 200))
	return string(body)
}

func main() {
	// httptest.NewServer listens on 127.0.0.1. It stands for an internal service.
	internal := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "internal admin page: 3 users, debug mode on")
	}))
	defer internal.Close()

	fmt.Println(preview(internal.URL + "/admin"))
}
```

```output for=get-any-url
internal admin page: 3 users, debug mode on
```

`http.Get` connects to any host and follows up to 10 redirects. The preview returns the internal page.

```go run id=dial-check
package main

import (
	"errors"
	"fmt"
	"io"
	"net"
	"net/http"
	"net/http/httptest"
	"net/netip"
	"syscall"
	"time"
)

var errBlocked = errors.New("destination address is not allowed")

// The ranges that the service must never reach. Keep this list in one place and test it.
var blocked = []netip.Prefix{
	netip.MustParsePrefix("0.0.0.0/8"),
	netip.MustParsePrefix("10.0.0.0/8"),
	netip.MustParsePrefix("127.0.0.0/8"),
	netip.MustParsePrefix("169.254.0.0/16"),
	netip.MustParsePrefix("172.16.0.0/12"),
	netip.MustParsePrefix("192.168.0.0/16"),
	netip.MustParsePrefix("::/128"),
	netip.MustParsePrefix("::1/128"),
	netip.MustParsePrefix("fc00::/7"),
	netip.MustParsePrefix("fe80::/10"),
}

// checkAddress runs after DNS resolution, on the address that the socket
// connects to, so a host name cannot hide an internal address.
func checkAddress(network, address string, _ syscall.RawConn) error {
	host, _, err := net.SplitHostPort(address)
	if err != nil {
		return err
	}
	ip, err := netip.ParseAddr(host)
	if err != nil {
		return err
	}
	ip = ip.Unmap()
	for _, p := range blocked {
		if p.Contains(ip) {
			return errBlocked
		}
	}
	return nil
}

var client = &http.Client{
	Timeout: 5 * time.Second,
	Transport: &http.Transport{
		Proxy:       nil,
		DialContext: (&net.Dialer{Control: checkAddress}).DialContext,
	},
	CheckRedirect: func(*http.Request, []*http.Request) error {
		return http.ErrUseLastResponse
	},
}

func preview(url string) string {
	resp, err := client.Get(url)
	if errors.Is(err, errBlocked) {
		return "refused: " + errBlocked.Error()
	}
	if err != nil {
		return "error"
	}
	defer resp.Body.Close()
	body, _ := io.ReadAll(io.LimitReader(resp.Body, 200))
	return string(body)
}

func main() {
	internal := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "internal admin page: 3 users, debug mode on")
	}))
	defer internal.Close()

	fmt.Println(preview(internal.URL + "/admin"))
}
```

```output for=dial-check
refused: destination address is not allowed
```

The client resolves the name and creates the socket, and then `checkAddress` sees the address before the connection. The loopback address is in the blocked list, so the request never leaves the process.

- Verdict: stdlib. `net.Dialer` has a `Control` function that runs after the socket is created and before it dials. A check there sees the resolved address. It also covers names that resolve to internal addresses, and DNS answers that change between a check and the connection.
- `http.Client` follows up to 10 redirects by default. `CheckRedirect` that returns `http.ErrUseLastResponse` stops at the first response. With the dial check, a redirect to an internal address also fails, because each connection goes through `Control`.
- `Proxy: nil` turns off proxies. When the client uses a proxy, the dial check sees the address of the proxy, not of the destination. The proxy must then apply the same rules.
- The documentation of `netip.Addr.IsPrivate` says that it does not describe a security property of addresses and should not be used for access control. The example keeps an explicit list of prefixes instead.
- gosec rule G107 flags a URL from tainted input in an HTTP request, and rule G704 finds SSRF with taint analysis. CodeQL has `go/request-forgery`.
