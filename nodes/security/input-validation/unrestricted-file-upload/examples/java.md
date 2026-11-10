```java run id=checked-upload
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.attribute.PosixFilePermissions;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.HexFormat;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

public class Main {
    static final int MAX_BYTES = 1_000_000;

    record Rule(String type, List<byte[]> signatures) {}

    // Each allowed extension, the type to serve it with, and its first bytes.
    static final Map<String, Rule> ALLOWED = Map.of(
            ".png", new Rule("image/png", List.of(HexFormat.of().parseHex("89504e470d0a1a0a"))),
            ".jpg", new Rule("image/jpeg", List.of(HexFormat.of().parseHex("ffd8ff"))),
            ".gif", new Rule("image/gif", List.of("GIF87a".getBytes(StandardCharsets.US_ASCII),
                    "GIF89a".getBytes(StandardCharsets.US_ASCII))));

    static class UploadException extends Exception {
        UploadException(String message) { super(message); }
    }

    static boolean startsWith(byte[] data, byte[] prefix) {
        return data.length >= prefix.length && Arrays.equals(data, 0, prefix.length, prefix, 0, prefix.length);
    }

    static String saveUpload(Path store, String filename, byte[] data)
            throws UploadException, IOException, NoSuchAlgorithmException {
        if (data.length > MAX_BYTES) throw new UploadException("the file is too large");
        int dot = filename.lastIndexOf('.');
        String ext = dot < 0 ? "" : filename.substring(dot).toLowerCase(Locale.ROOT);
        Rule rule = ALLOWED.get(ext);
        if (rule == null) throw new UploadException("the extension " + ext + " is not allowed");
        if (rule.signatures().stream().noneMatch(s -> startsWith(data, s))) {
            throw new UploadException("the content is not " + rule.type());
        }
        // The server picks the name. Only the checked extension comes from the client.
        byte[] hash = MessageDigest.getInstance("SHA-256").digest(data);
        String name = HexFormat.of().formatHex(hash, 0, 8) + ext;
        Files.write(store.resolve(name), data);
        return name;
    }

    public static void main(String[] args) throws Exception {
        // The store is a private directory outside the web root.
        Path store = Files.createTempDirectory("uploads", PosixFilePermissions.asFileAttribute(
                PosixFilePermissions.fromString("rwx------")));
        byte[] png = Arrays.copyOf(HexFormat.of().parseHex("89504e470d0a1a0a"), 24);
        Map<String, byte[]> uploads = new LinkedHashMap<>();
        uploads.put("notes.html", "<h1>hello</h1>".getBytes(StandardCharsets.UTF_8));
        uploads.put("notes.png", "<h1>hello</h1>".getBytes(StandardCharsets.UTF_8));
        uploads.put("photo.php.PNG", png);
        for (var upload : uploads.entrySet()) {
            try {
                System.out.println(upload.getKey() + ": saved as " + saveUpload(store, upload.getKey(), upload.getValue()));
            } catch (UploadException e) {
                System.out.println(upload.getKey() + ": rejected, " + e.getMessage());
            }
        }
        try (var files = Files.list(store)) {
            files.forEach(f -> f.toFile().delete());
        }
        Files.delete(store);
    }
}
```

```output for=checked-upload
notes.html: rejected, the extension .html is not allowed
notes.png: rejected, the content is not image/png
photo.php.PNG: saved as f731c2528f4184b3.png
```

`saveUpload` checks the size, the extension and the first bytes, and names the file after its SHA-256 hash. The store directory has the permissions `rwx------`.

- Verdict: stdlib. Servlet containers parse uploads with `Part`, and the checks above stay the same. `Part.getSubmittedFileName` is the client's name: check its extension and do not use it as a path.
- The result of `Files.probeContentType` depends on the installed file type detectors. Do not use it as the check.
- `Arrays.equals` with ranges compares the signature with no extra copy.
