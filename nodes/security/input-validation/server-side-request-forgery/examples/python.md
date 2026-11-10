```python wrong id=fetch-any-url
import http.server
import pathlib
import tempfile
import threading
import urllib.request


class InternalAdmin(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"internal admin page: 3 users, debug mode on")

    def log_message(self, *args):
        pass


# A service that only the local network should reach.
internal = http.server.HTTPServer(("127.0.0.1", 0), InternalAdmin)
threading.Thread(target=internal.serve_forever, daemon=True).start()
port = internal.server_address[1]

# No proxy, so every request in this example stays on this machine.
opener = urllib.request.build_opener(urllib.request.ProxyHandler({}))


def preview(url):
    # The feature: show a preview of a page that the user names.
    with opener.open(url, timeout=2) as response:
        return response.read(200).decode()


print(preview(f"http://127.0.0.1:{port}/admin"))

with tempfile.TemporaryDirectory() as tmp:
    settings = pathlib.Path(tmp) / "settings.ini"
    settings.write_text("api_key = placeholder-not-a-real-key\n")
    print(preview(settings.as_uri()))
```

```output for=fetch-any-url
internal admin page: 3 users, debug mode on
api_key = placeholder-not-a-real-key
```

The first call reads the internal page. The second call reads a local file through a `file:` URL. The function trusts the destination in the input.

```python run id=allow-list
import http.server
import threading
import urllib.error
import urllib.parse
import urllib.request


class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/cat.txt":
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"a picture of a cat")
        elif self.path == "/moved":
            self.send_response(302)
            self.send_header("Location", f"http://127.0.0.1:{admin_port}/admin")
            self.end_headers()
        else:
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b"internal admin page: 3 users, debug mode on")

    def log_message(self, *args):
        pass


def start():
    server = http.server.HTTPServer(("127.0.0.1", 0), Handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server.server_address[1]


media_port = start()  # stands for a public media host
admin_port = start()  # stands for an internal service

# The operator chooses the destinations. Here both servers run on 127.0.0.1,
# so the port tells them apart. In production, the host names do.
ALLOWED = {("http", "127.0.0.1", media_port)}


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        raise urllib.error.HTTPError(req.full_url, code, "redirect refused", headers, fp)


opener = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect)


def preview(url):
    parts = urllib.parse.urlsplit(url)
    if (parts.scheme, parts.hostname, parts.port) not in ALLOWED:
        return "refused: destination not allowed"
    try:
        with opener.open(url, timeout=2) as response:
            return response.read(200).decode()
    except urllib.error.HTTPError as error:
        return f"refused: upstream answered {error.code}"


print(preview(f"http://127.0.0.1:{media_port}/cat.txt"))
print(preview(f"http://127.0.0.1:{admin_port}/admin"))
print(preview("file:///tmp/settings.ini"))
print(preview(f"http://127.0.0.1:{media_port}/moved"))
```

```output for=allow-list
a picture of a cat
refused: destination not allowed
refused: destination not allowed
refused: upstream answered 302
```

The check runs on the parsed URL, before any request. `NoRedirect` stops at the first response, so an allowed host cannot send the client to another host.

- Verdict: stdlib. `urllib.parse.urlsplit` parses the URL, and `hostname` and `port` give the parts to compare. `http.client` makes a request to one host and port and never follows redirects.
- `urllib.request.urlopen` and the default opener handle `http:`, `https:`, `ftp:`, `file:` and `data:` URLs. Check the scheme before the call, or build an opener with only the handlers that the feature needs.
- The default opener follows redirects. A subclass of `HTTPRedirectHandler` whose `redirect_request` raises `HTTPError` stops them.
- `ProxyHandler({})` turns off the proxies from the environment. The examples use it so that every request stays on the local machine.
- Python has no hook in `urllib` that checks the address at connect time. For a feature that must reach any public host, resolve the name once, check each address with `ipaddress`, and connect to that address. The Go example file shows the check at connect time.
- Ruff rule S310 flags `urlopen` calls that can open schemes other than `http:` and `https:`. CodeQL has `py/full-ssrf` and `py/partial-ssrf`.
