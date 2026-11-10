```javascript run id=checked-upload
import { createHash } from "node:crypto";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { extname, join } from "node:path";

const MAX_BYTES = 1_000_000;
// Each allowed extension, the type to serve it with, and its first bytes.
const ALLOWED = new Map([
  [".png", { type: "image/png", signatures: [Buffer.from("89504e470d0a1a0a", "hex")] }],
  [".jpg", { type: "image/jpeg", signatures: [Buffer.from("ffd8ff", "hex")] }],
  [".gif", { type: "image/gif", signatures: [Buffer.from("GIF87a"), Buffer.from("GIF89a")] }],
]);

function saveUpload(store, filename, data) {
  if (data.length > MAX_BYTES) throw new Error("the file is too large");
  const ext = extname(filename).toLowerCase();
  const rule = ALLOWED.get(ext);
  if (!rule) throw new Error(`the extension ${ext || "(none)"} is not allowed`);
  if (!rule.signatures.some((s) => data.subarray(0, s.length).equals(s))) {
    throw new Error(`the content is not ${rule.type}`);
  }
  // The server picks the name. Only the checked extension comes from the client.
  const name = createHash("sha256").update(data).digest("hex").slice(0, 16) + ext;
  writeFileSync(join(store, name), data, { mode: 0o600 });
  return { name, headers: { "Content-Type": rule.type, "X-Content-Type-Options": "nosniff", "Content-Disposition": "attachment" } };
}

// The store is a private directory outside the web root.
const store = mkdtempSync(join(tmpdir(), "uploads-"));
const png = Buffer.concat([Buffer.from("89504e470d0a1a0a", "hex"), Buffer.alloc(16)]);
for (const [filename, data] of [
  ["notes.html", Buffer.from("<h1>hello</h1>")],
  ["notes.png", Buffer.from("<h1>hello</h1>")],
  ["photo.php.PNG", png],
]) {
  try {
    const { name, headers } = saveUpload(store, filename, data);
    console.log(`${filename}: saved as ${name}`);
    console.log("  serve with", JSON.stringify(headers));
  } catch (error) {
    console.log(`${filename}: rejected, ${error.message}`);
  }
}
rmSync(store, { recursive: true });
```

```output for=checked-upload
notes.html: rejected, the extension .html is not allowed
notes.png: rejected, the content is not image/png
photo.php.PNG: saved as f731c2528f4184b3.png
  serve with {"Content-Type":"image/png","X-Content-Type-Options":"nosniff","Content-Disposition":"attachment"}
```

`saveUpload` checks the size, the extension and the first bytes. It writes the file under a name that the server picks, with mode `0o600`, and returns the headers to serve it with.

- Verdict: stdlib. Node.js has no multipart parser in its standard library. A parser library reads the upload, and the checks above stay the same.
- `path.extname` returns only the last extension, so `photo.php.PNG` gives `.PNG`.
- The `accept` attribute of an `<input type="file">` element only helps the user pick a file. The server must check again.
