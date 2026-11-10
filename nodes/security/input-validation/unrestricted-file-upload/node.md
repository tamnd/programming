---
tag: NEW-unrestricted-file-upload
kind: weakness
slug: unrestricted-file-upload
name: Unrestricted file upload
aliases: [unrestricted upload of file with dangerous type, dangerous file upload, arbitrary file upload, file upload vulnerability, cwe-434]
area: security/input-validation
level: core
summary: >-
  Unrestricted file upload happens when a program accepts a file of a dangerous type and stores or serves it where the server can run it or a browser can open it as a page.
cwe: 434
abstraction: base
languages_affected: [any-web-server]
owasp_top10_2025: A06
cwe_top25_2025: 12
history:
  year: 2006
  people: []
  note: PLOVER contributed the entry to CWE Draft 3 on 2006-07-19 as Unrestricted File Upload, and CWE changed the name to Unrestricted Upload of File with Dangerous Type on 2010-02-16.
sources:
  - url: https://cwe.mitre.org/data/definitions/434.html
    title: "CWE-434: Unrestricted Upload of File with Dangerous Type"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Weakness Ordinalities; Demonstrative Examples, Example 1; Detection Methods; Observed Examples; Content History; Previous Entry Names
  - url: https://cheatsheetseries.owasp.org/cheatsheets/File_Upload_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, File Upload Cheat Sheet
    locator: File Upload Threats; File Upload Protection; List Allowed Extensions; Block Extensions; Content-Type Validation; File Signature Validation; Filename Safety; File Content Validation; File Storage Location; Upload and Download Limits
  - url: https://www.cve.org/CVERecord?id=CVE-2001-0901
    title: CVE-2001-0901, Hypermail
    locator: the description and the publication date
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 12, CVEs in KEV, Rank Change vs. 2024
  - url: https://top10.owasp.org/2025/A06_2025-Insecure_Design/
    title: "OWASP Top 10:2025, A06:2025 Insecure Design"
    locator: Background, notable CWEs
  - url: https://mimesniff.spec.whatwg.org/
    title: WHATWG, MIME Sniffing Standard
    locator: 6.1 Matching an image type pattern
  - url: https://pkg.go.dev/net/http
    title: Go standard library, package net/http
    locator: func DetectContentType; func MaxBytesReader
  - url: https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/X-Content-Type-Options
    title: MDN Web Docs, X-Content-Type-Options
    locator: the nosniff directive, MIME type sniffing disabled
  - url: https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Content-Disposition
    title: MDN Web Docs, Content-Disposition
    locator: the introduction; Syntax, as a response header for the main body
  - url: https://codeql.github.com/codeql-query-help/csharp/cs-web-file-upload/
    title: CodeQL query help for C#, Use of file upload
    locator: the query ID; Recommendation
  - url: https://www.cve.org/CVERecord?id=CVE-2006-4558
    title: CVE-2006-4558, DeluxeBB
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2006-6994
    title: CVE-2006-6994, OzzyWork Gallery
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-35489
    title: CVE-2020-35489, Contact Form 7 for WordPress
    locator: the description and the publication date
---

## Description

Many web applications accept files from users, such as a profile photo, a CV or an attachment. CWE-434 happens when the product allows the upload or transfer of dangerous file types that are automatically processed within its environment.[^1] The worst case is a file that the server runs as code. CWE notes that web servers often treat extensions such as .asp and .php as executable, even when the file system permissions do not allow execution.[^1] A file that the site serves back to other users is also a risk. An HTML page from the site's own origin can run script in the browser of every user who opens it. That is [[cross-site-scripting]].

CWE gives a short example. A PHP page saves a picture under the name that the client sends, in a directory under the web document root. A client that sends a file named `malicious.php` gets a file that the web server runs when someone requests its URL.[^1]

Upload checks fail in a few common ways:

- **Trusting the client.** The client sends the Content-Type of each file. The OWASP File Upload Cheat Sheet says that this value cannot be trusted, because it is trivial to spoof.[^2] A check in the browser can be removed, so CWE says to repeat every client-side check on the server.[^1]
- **Blocking a list of extensions.** The cheat sheet calls this a weak protection method on its own.[^2] A list misses an extension that the server also runs. In CVE-2001-0901, an attachment with the extension .shtml ran on servers with server-side includes.[^3]
- **Double extensions.** Some web servers, including some versions of Apache, may process a file by an inner extension. Then `filename.php.gif` goes to the PHP interpreter.[^1]
- **Case.** On a server with case-insensitive file names, compare extensions without regard to case.[^1]
- **Content checks alone.** A valid image can still carry code in some segment. CWE gives the free-form comment field of a GIF as an example.[^1] The cheat sheet says that a file signature check should not be used on its own.[^2]
- **The client's file name.** A name can contain special characters, path separators or names that the file system reserves.[^2] A name such as `../index.html` is a [[path-traversal]] problem as well as an upload problem.

Uploads have other risks too. Very large files, ZIP bombs and XML bombs can fill the storage. A file can attack the parser that processes it. A file can overwrite an existing file.[^2] CWE calls the older name "Unrestricted File Upload" insufficiently precise. It could also mean a missing limit on the size or number of files, which is a resource consumption issue.[^1]

CWE-434 is a base weakness and a child of CWE-669, Incorrect Resource Transfer Between Spheres.[^1] It is rank 12 on the 2025 CWE Top 25, with 4 CVEs in the CISA KEV catalog. It was rank 10 the year before.[^4] The OWASP Top 10:2025 names it as a notable CWE of A06:2025 Insecure Design.[^5]

### History

PLOVER contributed the entry to CWE Draft 3 on 2006-07-19.[^1] Its name was "Unrestricted File Upload" until 2010-02-16, when CWE changed it to "Unrestricted Upload of File with Dangerous Type".[^1] The weakness is older than the entry. CVE-2001-0901, published in 2002, records that Hypermail archived attachments with a .shtml extension on the server, where a request for the URL ran them.[^3]

## Vulnerable code

The handler below accepts any file that the client labels as an image. It keeps the client's file name and saves the file under the web root. A static file server then picks the type of each file from its extension. The demonstration upload is an HTML file named `notes.html` that claims to be `image/png`.

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

The server saved the page and serves it as `text/html` from its own origin. A browser that opens `/uploads/notes.html` shows it as a page of the site, and any script in it runs there. On a server that runs scripts by extension, the same handler would accept a script.

## Fixed code

The fix checks the file on the server, and it trusts nothing from the client except the extension that it checks. It picks the name, stores the file outside the web root and serves it with fixed headers.

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

The page is rejected by its extension. The second file has an allowed extension, but its first bytes are not a PNG signature. The third file passes, and its stored name has one extension that the server chose. The download of `../notes.html` gets 404, because only names that the server made are valid.

### Check the type in layers

Allow a short list of extensions that the business needs. The cheat sheet gives examples: one image type for an image upload, and docx and pdf for a CV.[^2] CWE says to consider [[cross-site-scripting]] before allowing .html or .htm.[^1] Lower the case of the extension before the check, and take only the last one.

Then check that the first bytes match the extension. The WHATWG MIME Sniffing Standard lists the signatures. PNG starts with the bytes `89 50 4E 47 0D 0A 1A 0A`, JPEG with `FF D8 FF`, and GIF with the text `GIF87a` or `GIF89a`.[^6] Go's `http.DetectContentType` implements that algorithm and considers at most the first 512 bytes.[^7] The Go example file detects the type from the bytes and ignores the client's name.

A signature check is not a full check. For images, the cheat sheet suggests to decode the file and encode it again in an allowed format, without the metadata. It warns that rewriting does not guarantee that all malicious content is removed, and that the image library also handles untrusted input.[^2]

### Pick the name on the server

CWE says to generate a unique file name for each upload, instead of the name that the user supplied.[^1] The cheat sheet calls a random name, such as a UUID, essential.[^2] The example uses a hash of the content, which also stores each file once. A generated name has one extension and no path separators, so double extensions and [[path-traversal]] have nothing to work with. If users need to see the original name, store it in the database as text, and never use it as a path.

### Store outside the web root

CWE suggests storing uploaded files outside the web document root, and delivering them with other mechanisms.[^1] The cheat sheet orders the choices by security. A different host is first, then a directory outside the web root with only administrative access. A directory inside the web root with write permissions only comes last.[^2] As a second layer, it says to turn off per-directory configuration overrides on upload directories, such as with Apache `AllowOverride None`.[^2]

### Serve with fixed headers

Serve each file with the type that the server stored, not one that it guesses from the name. Add `X-Content-Type-Options: nosniff`. With it, the browser uses the supplied Content-Type as it is, and does not examine the content to infer the type.[^8] MDN says that this prevents XSS attacks where user-uploaded content runs as an HTML document.[^8] `Content-Disposition: attachment` asks the browser to download the file instead of showing it.[^9]

### Limit the size

Set a size limit for the upload, and set it after decompression if the server extracts archives.[^2] In Go, `http.MaxBytesReader` limits the size of a request body.[^7]

## How to detect it

- **Review each upload handler.** Ask where the file goes and who picks its name. Ask which checks run on the server, and how the file is served later. CWE rates manual source code review and automated static analysis as highly effective for this weakness.[^1]
- **Static analysis.** CodeQL for C# has the query `cs/web/file-upload`. Its recommendation is to validate each path where uploaded data is written, and to check the content without relying only on the MIME type.[^10]
- **Tests with the classic inputs.** Upload `notes.html`, a text file named `photo.png` and a double extension such as `photo.php.png`. Also try a name with `../` and a file over the size limit. Each one must be rejected or stored safely.
- **Dynamic scanners.** CWE rates web application scanners and fuzzers as partial for this weakness.[^1]
- **Check the server configuration.** Find the directories where uploads land, and confirm that the web server does not run scripts there.

## How to prevent it

- **Allow a short list of extensions, and check the content.** Use both, and never rely on the Content-Type from the client.[^2]
- **Generate the file name.** Keep the client's name out of every path.[^1][^2]
- **Store files outside the web root**, or on a different host.[^1][^2]
- **Serve with a stored type, `nosniff` and `attachment`.**[^8][^9]
- **Limit sizes and counts.** Check the size after decompression too.[^2]
- **Repeat client checks on the server.**[^1]
- **Run with low privileges or in a sandbox.** CWE suggests the lowest privileges that the task needs. It also suggests a jail such as chroot, AppArmor or SELinux.[^1]
- **Check who uploads.** The cheat sheet says that the user should be authenticated and should have permission to upload.[^2] See [[missing-authorization]].

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2001-0901 | published 2002-06-25 | Hypermail archived an attachment with a .shtml extension on the server. On a server with server-side includes, a request for its URL ran commands.[^3] |
| CVE-2006-4558 | published 2006-09-06 | DeluxeBB 1.06 and earlier, on Apache HTTP Server with mod_mime, let remote attackers run PHP code by uploading files with double extensions.[^11] |
| CVE-2006-6994 | published 2007-02-12 | In OzzyWork Gallery, remote attackers could upload and run ASP files by removing the client-side security checks.[^12] |
| CVE-2020-35489 | published 2020-12-17 | Contact Form 7 for WordPress before 5.3.2 allowed unrestricted file upload and remote code execution, because a file name may contain special characters.[^13] |

## References
