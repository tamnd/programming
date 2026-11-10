---
tag: NEW-compute-a-sha-256-hash
kind: task
slug: compute-a-sha-256-hash
name: Compute a SHA-256 hash
aliases: [sha256 of a string, sha256 of a file, sha256 hex digest, hash a file, checksum a file with sha256]
area: security/cryptography-use
level: core
summary: Compute the SHA-256 digest of the UTF-8 bytes of a string, or of the bytes of a file read in chunks, and give it as 64 lowercase hexadecimal digits.
contract: contract.yaml
category: security
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
    title: NIST, FIPS PUB 180-4, Secure Hash Standard (SHS)
    locator: Announcement item 3 (Explanation); 4.2.2 SHA-224 and SHA-256 Constants; 5.1.1 SHA-1, SHA-224 and SHA-256 (padding); 5.3.3 SHA-256 (initial hash value); 6.2.2 SHA-256 Hash Computation
  - url: https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf
    title: NIST, Cryptographic Standards and Guidelines, Example Values, SHA-256
    locator: One Block Message Sample; Two Block Message Sample
  - url: https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf
    title: NIST, Cryptographic Standards and Guidelines, Example Values, SHA-2 additional test data
    locator: SHA-256 Test Data, #1 to #10
  - url: https://pages.nist.gov/800-63-4/sp800-63b.html
    title: NIST SP 800-63B-4, Digital Identity Guidelines, Authentication and Authenticator Management
    locator: 3.1.1.2 Password Verifiers
  - url: https://docs.python.org/3/library/hashlib.html
    title: The Python Standard Library, hashlib
    locator: hashlib.file_digest, hash.hexdigest, Key derivation
  - url: https://docs.python.org/3/library/hmac.html
    title: The Python Standard Library, hmac
    locator: hmac.compare_digest
  - url: https://docs.python.org/3/library/unicodedata.html
    title: The Python Standard Library, unicodedata
    locator: unicodedata.normalize
  - url: https://nodejs.org/api/crypto.html
    title: Node.js documentation, Crypto
    locator: Class Hash, hash.update(data[, inputEncoding]), hash.digest([encoding])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/security/MessageDigest.html
    title: Java SE 25 API, java.security.MessageDigest
    locator: class description (required algorithms), digest(byte[])
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/HexFormat.html
    title: Java SE 25 API, java.util.HexFormat
    locator: of(), formatHex(byte[])
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.security.cryptography.sha256.hashdata?view=net-10.0
    title: .NET API reference, SHA256.HashData
    locator: HashData(Byte[]), HashData(Stream)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.convert.tohexstring?view=net-10.0
    title: .NET API reference, Convert.ToHexString
    locator: ToHexString(Byte[]), Returns
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.convert.tohexstringlower?view=net-10.0
    title: .NET API reference, Convert.ToHexStringLower
    locator: ToHexStringLower(Byte[])
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.text.encoding.unicode?view=net-10.0
    title: .NET API reference, Encoding.Unicode
    locator: Property Value
  - url: https://pkg.go.dev/crypto/sha256
    title: Go standard library, package crypto/sha256
    locator: package overview, Sum256, New
  - url: https://man7.org/linux/man-pages/man1/sha256sum.1.html
    title: Linux man-pages, sha256sum(1)
    locator: Description
---

## Task

Compute the SHA-256 digest of a message, and give it as 64 lowercase hexadecimal digits.

SHA-256 is one of the hash algorithms of FIPS 180-4. It takes a message of fewer than 2^64 bits and gives a digest of 256 bits, which is 32 bytes. The standard says that it is computationally infeasible to find a message for a given digest, or two messages with the same digest. So a digest can identify a file or a message, for example to check a download.

SHA-256 works on bytes, not on characters. A string must first become bytes, and different encodings give different bytes. This task uses UTF-8 with no byte order mark and no Unicode normalization. See [[utf-8]].

Each solution has two functions:

- `sha256_text(text)` returns the digest of the UTF-8 bytes of the string. This is the entry of the contract.
- `sha256_file(path)` returns the digest of the bytes of a file. It reads the file in chunks, so a large file does not fill the memory. A missing file is an error, and the function returns no digest.

The hexadecimal form has two digits for each byte, so a byte below `0x10` has a leading zero. The solutions give lowercase digits.

## Solutions

- **Python.** `hashlib.sha256(text.encode("utf-8")).hexdigest()`. The `hexdigest` method gives the digest as hexadecimal digits. For a file, `hashlib.file_digest(f, "sha256")` reads the file. The documentation says that the file must be open for reading in binary mode, so the solution opens it with `"rb"`.
- **JavaScript and TypeScript (Node.js).** `createHash("sha256").update(text, "utf8").digest("hex")`. When the data is a string and no encoding is given, `hash.update` uses UTF-8. The solution gives `"utf8"` to make this visible. For a file, a `for await` loop reads the chunks of `fs.createReadStream` and gives each chunk to `hash.update`. The TypeScript file adds types only.
- **Java.** `MessageDigest.getInstance("SHA-256")`. Every Java platform must support SHA-256, so the solution turns the checked `NoSuchAlgorithmException` into an `AssertionError`. `digest(text.getBytes(UTF_8))` gives the bytes, and `HexFormat.of().formatHex` gives lowercase digits with no delimiter. For a file, a loop reads an `InputStream` into a buffer and calls `update`.
- **C#.** `SHA256.HashData(Encoding.UTF8.GetBytes(text))`, then `Convert.ToHexStringLower`. `Convert.ToHexString` gives uppercase digits. For a file, `SHA256.HashData(stream)` with a stream from `File.OpenRead`.
- **Go.** `sha256.Sum256([]byte(text))` returns a `[32]byte` array, and `hex.EncodeToString(sum[:])` gives lowercase digits. For a file, `sha256.New()` gives a `hash.Hash`, and `io.Copy` writes the file into it. The test checks the error of a missing file with `errors.Is(err, fs.ErrNotExist)`.
- **Rust.** The standard library has no SHA-256. The solution is a short implementation of FIPS 180-4: the 64 constants of section 4.2.2, the initial hash value of section 5.3.3, the padding of section 5.1.1, and the computation of section 6.2.2. A `Sha256` value takes the message in parts with `update`, and `finalize` returns the 32 bytes. `sha256_file` returns `io::Result<String>`. A program with dependencies can use a crate for SHA-256 in place of this code.
- **C.** The standard library has no SHA-256. The solution implements FIPS 180-4 as in Rust, with `sha256_init`, `sha256_update` and `sha256_final`. `sha256_hex(data, len, out)` writes 64 digits and a NUL into `out`. `sha256_file` reads the file with `fread` and returns `-1` with `errno` set on an error.
- **C++.** The standard library has no SHA-256. The solution is the same algorithm as a class `Sha256` with `update` and `finalize`. `sha256_file` returns `std::expected<std::string, HashError>`, so the caller must check the result.
- **Bash.** Bash has no SHA-256. The solution calls `sha256sum` from GNU coreutils. `printf '%s' "$text" | sha256sum` hashes the bytes of the text with no newline. With no file operand, `sha256sum` reads the standard input, so the solution gives the file with `<"$path"`. The output starts with the digest, and `${out%% *}` keeps the part before the first space.

## Edge cases

The cases of `sha256_text` are in `cases/basic.jsonl` and `cases/edge.jsonl`. The expected digests of `abc` and of the 56-byte message `abcdbcdecdef...nopq` are the one-block and two-block samples of the NIST example values.

- `empty`: the empty string has a digest too. It is `e3b0c442...b855`.
- `a-55`, `a-56`, `a-63` and `a-64`: the padding adds a 1 bit and a 64-bit length. A message of 55 bytes fills one block of 64 bytes with its padding. A message of 56 bytes needs a second block. These cases catch an error in the padding of the Rust, C and C++ solutions.
- `multi-block-112`: a message of 112 bytes. With its padding, it is two blocks of 64 bytes.
- `non-ascii`: characters of 2, 3 and 4 bytes in UTF-8.
- `nfc-cafe` and `nfd-cafe`: the same visible word `café` in two forms. NFC has `é` as one code point. NFD has `e` and a combining accent. The bytes differ, so the digests differ.
- `trailing-newline`: `abc` and `abc\n` have different digests.

The cases of `sha256_file` are in `cases/files.jsonl`, with the files in `fixtures/`: `abc`, `abc\n`, `abc\r\n`, an empty file, and three bytes `00 01 ff`. The toolchain does not load this file, so each test file holds these cases by name. Each test file also hashes a file of one million `a` bytes, which is more than one chunk. The case `missing-file` in `cases/errors.jsonl` gives a path that does not exist. Each solution reports a "not found" error.

The Rust, C and C++ tests also check the ten SHA-256 messages of the NIST additional test data, and check that a message in parts of 1 to 130 bytes gives the same digest as the full message.

## Pitfalls

- **`echo` adds a newline in Bash.** The digest is of the text and the newline:

```bash wrong id=bash-echo
echo abc | sha256sum
printf '%s' abc | sha256sum
```

```output
edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb  -
ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad  -
```

Use `printf '%s'`. The same mistake happens when a program reads a line from a file and keeps the line terminator.

- **`BigInteger.toString(16)` drops the leading zeros in Java.** The digest of `id-45` starts with the digit `0`:

```java wrong id=java-biginteger
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.HexFormat;

public class Main {
    public static void main(String[] args) throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        byte[] digest = md.digest("id-45".getBytes(StandardCharsets.UTF_8));
        String wrong = new BigInteger(1, digest).toString(16);
        String right = HexFormat.of().formatHex(digest);
        System.out.println(wrong.length() + " " + wrong);
        System.out.println(right.length() + " " + right);
    }
}
```

```output
63 f7f3525a5254a898edc33391b8a4ebd1a6afe61833e368469dd54f07fa40b7b
64 0f7f3525a5254a898edc33391b8a4ebd1a6afe61833e368469dd54f07fa40b7b
```

A compare with a stored digest of 64 digits fails. `HexFormat` gives two digits for each byte.

- **The bytes and the case of the digits must match in C#.** `Encoding.Unicode` is UTF-16 in little-endian byte order, so it gives other bytes and another digest. `Convert.ToHexString` gives uppercase digits, so a compare with a lowercase digest is false:

```csharp wrong id=csharp-utf16
using System.Security.Cryptography;
using System.Text;

string want = "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad";
byte[] utf16 = SHA256.HashData(Encoding.Unicode.GetBytes("abc"));
byte[] utf8 = SHA256.HashData(Encoding.UTF8.GetBytes("abc"));
Console.WriteLine(Convert.ToHexStringLower(utf16));
Console.WriteLine(Convert.ToHexString(utf8));
Console.WriteLine(Convert.ToHexString(utf8) == want);
Console.WriteLine(Convert.ToHexStringLower(utf8) == want);
```

```output
13e228567e8249fce53337f25d7970de3bd68ab2653424c7b8f9fd05e33caedf
BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD
False
True
```

- **Equal text can have different bytes.** Two strings that look the same can have different code points. Python shows it with `unicodedata.normalize`:

```python run id=python-normalization
import hashlib
import unicodedata

nfc = "caf\u00e9"
nfd = unicodedata.normalize("NFD", nfc)
print(nfc == nfd, len(nfc), len(nfd))
print(hashlib.sha256(nfc.encode("utf-8")).hexdigest())
print(hashlib.sha256(nfd.encode("utf-8")).hexdigest())
print(hashlib.sha256(unicodedata.normalize("NFC", nfd).encode("utf-8")).hexdigest())
```

```output
False 4 5
850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e
81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a
850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e
```

If two systems must agree on the digest of text, they must agree on one normalization form before they hash. The solutions on this page do not normalize, because a file or a message must keep its bytes.

- **A `Hash` object in Node.js gives one digest only.** After `digest`, the object cannot be used again, and a second call throws an error:

```javascript run id=js-digest-twice
import { createHash } from "node:crypto";

const hash = createHash("sha256").update("abc");
console.log(hash.digest("hex"));
try {
  hash.digest("hex");
} catch (e) {
  console.log(e.code);
}
```

```output
ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
ERR_CRYPTO_HASH_FINALIZED
```

Make a new `Hash` for each message.

- **A plain SHA-256 is not a password hash.** The Python documentation says that naive algorithms such as `sha1(password)` are not resistant to brute-force attacks. Also, the same password always gives the same digest. So two users with the same password have the same stored value:

```python wrong id=python-password
import hashlib

for user in ["ana", "ben"]:
    print(user, hashlib.sha256(b"hunter2").hexdigest())
```

```output
ana f52fbd32b2b3b86ff88ef6c490628285f482af15ddcb29541f94bcf526a3f6c7
ben f52fbd32b2b3b86ff88ef6c490628285f482af15ddcb29541f94bcf526a3f6c7
```

NIST SP 800-63B-4 says that passwords shall be salted and hashed with a suitable password hashing scheme. The Python documentation says that a password hash must be tunable, slow, and include a salt. `hashlib.pbkdf2_hmac` and `hashlib.scrypt` are two such functions in the standard library. Store the salt with the hash. Compare with `hmac.compare_digest`, which the `hmac` documentation recommends in place of `==` to reduce the risk of a timing attack:

```python run id=python-pbkdf2
import hashlib
import hmac
import os


def hash_password(password: str, salt: bytes) -> bytes:
    return hashlib.pbkdf2_hmac("sha256", password.encode("utf-8"), salt, 600_000)


salt_ana, salt_ben = os.urandom(16), os.urandom(16)
stored_ana = hash_password("hunter2", salt_ana)
stored_ben = hash_password("hunter2", salt_ben)
print("same stored value:", stored_ana == stored_ben)
print("hunter2:", hmac.compare_digest(stored_ana, hash_password("hunter2", salt_ana)))
print("hunter3:", hmac.compare_digest(stored_ana, hash_password("hunter3", salt_ana)))
```

```output
same stored value: False
hunter2: True
hunter3: False
```

The iteration count sets the cost. SP 800-63B-4 says that the cost should be as high as practical.

## Related tasks

- [[base64-encode-and-decode]]: give the 32 bytes of a digest as Base64 text in place of hexadecimal digits.
- [[read-a-file-line-by-line]]: read a file in parts, as `sha256_file` does.
- [[write-a-file]]: write the bytes that a test hashes.
- [[read-from-standard-input]]: hash the standard input, as `sha256sum` does with no file operand.
- [[run-a-subprocess]]: run `sha256sum` from another language, and read its output.
- [[read-and-write-json]]: the text of a JSON value can change and keep the same meaning, so hash the bytes, not the value.
- [[utf-8]]: the bytes of text that the task hashes.

## References
