```python wrong id=trust-client
import mimetypes
import pathlib
import tempfile


def save_upload(webroot, filename, content_type, data):
    # The client sends the name and the Content-Type. The code trusts both.
    if not content_type.startswith("image/"):
        return "rejected"
    (webroot / "uploads" / filename).write_bytes(data)
    return f"saved /uploads/{filename}"


def serve(webroot, url_path):
    # A static file server picks the type from the extension.
    path = webroot / url_path.lstrip("/")
    content_type, _ = mimetypes.guess_type(path.name)
    return content_type, path.read_bytes()


with tempfile.TemporaryDirectory() as tmp:
    webroot = pathlib.Path(tmp)
    (webroot / "uploads").mkdir()
    # The client says image/png, but the file is a web page.
    print(save_upload(webroot, "notes.html", "image/png", b"<h1>hello</h1>"))
    print(serve(webroot, "/uploads/notes.html"))
```

```output for=trust-client
saved /uploads/notes.html
('text/html', b'<h1>hello</h1>')
```

The handler trusts the Content-Type and the name that the client sends. The page lands in the web root, and the static server gives it the type `text/html`.

```python run id=checked-upload
import hashlib
import pathlib
import re
import tempfile

MAX_BYTES = 1_000_000
# Each allowed extension, the type to serve it with, and its first bytes.
ALLOWED = {
    ".png": ("image/png", (b"\x89PNG\r\n\x1a\n",)),
    ".jpg": ("image/jpeg", (b"\xff\xd8\xff",)),
    ".gif": ("image/gif", (b"GIF87a", b"GIF89a")),
}
STORED_NAME = re.compile(r"[0-9a-f]{64}\.(png|jpg|gif)")


class UploadError(Exception):
    pass


def save_upload(store, filename, data):
    """Check the file and save it under a name that the server picks."""
    if len(data) > MAX_BYTES:
        raise UploadError("the file is too large")
    ext = pathlib.PurePath(filename).suffix.lower()
    if ext not in ALLOWED:
        raise UploadError(f"the extension {ext or '(none)'} is not allowed")
    content_type, signatures = ALLOWED[ext]
    if not data.startswith(signatures):
        raise UploadError(f"the content is not {content_type}")
    name = hashlib.sha256(data).hexdigest() + ext
    (store / name).write_bytes(data)
    return name


def download(store, name):
    """Serve a stored file with headers that the server sets."""
    if not STORED_NAME.fullmatch(name) or not (store / name).is_file():
        return 404, {}, b""
    headers = {
        "Content-Type": ALLOWED[pathlib.PurePath(name).suffix][0],
        "X-Content-Type-Options": "nosniff",
        "Content-Disposition": "attachment",
    }
    return 200, headers, (store / name).read_bytes()


PNG = b"\x89PNG\r\n\x1a\n" + bytes(16)  # The signature and some padding.

# The store is a directory outside the web root.
with tempfile.TemporaryDirectory() as tmp:
    store = pathlib.Path(tmp)
    for filename, data in [
        ("notes.html", b"<h1>hello</h1>"),
        ("notes.png", b"<h1>hello</h1>"),
        ("photo.php.PNG", PNG),
    ]:
        try:
            name = save_upload(store, filename, data)
            print(f"{filename}: saved as {name}")
        except UploadError as error:
            print(f"{filename}: rejected, {error}")
    status, headers, body = download(store, name)
    print(status, headers, len(body), "bytes")
    print(download(store, "../notes.html")[0])
```

```output for=checked-upload
notes.html: rejected, the extension .html is not allowed
notes.png: rejected, the content is not image/png
photo.php.PNG: saved as f731c2528f4184b386555de339aeb78f1f0908532be04e6bfe1ec790f5f50da9.png
200 {'Content-Type': 'image/png', 'X-Content-Type-Options': 'nosniff', 'Content-Disposition': 'attachment'} 24 bytes
404
```

`save_upload` checks the size, the extension and the first bytes, and then picks the name. `download` accepts only names that `save_upload` can make, and sets the headers itself.

- Verdict: stdlib. Python has no upload handling in the standard library since the `cgi` module was removed in Python 3.13. Web frameworks parse the upload, and the checks above stay the same.
- `pathlib.PurePath(name).suffix` gives only the last extension, so `photo.php.PNG` gives `.PNG`. Lower the case before the check.
- `mimetypes.guess_type` guesses from the name only. Use it to label files that the server made, never to check a file from a client.
- To check an image fully, decode it and encode it again with an image library. The standard library has no image decoder.
