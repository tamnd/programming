---
tag: 7GG506
kind: feature
owner: python
slug: data-compression
name: Data compression
aliases: [zlib, gzip, bz2, lzma, zipfile, tarfile, compression.zstd, compression package, zstd, archives]
area: languages/python/stdlib
level: core
summary: >-
  The standard library compresses bytes with zlib, gzip, bz2, lzma and zstd, and it reads and writes ZIP and tar archives with zipfile and tarfile. Each one has compress and decompress.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/archiving.html", locator: "Data Compression and Archiving"}
proposals: []
see_also: [python/the-standard-library-tour, python/file-format-modules, python/pickle-and-object-serialization, python/bytes-and-bytearray]
sources:
  - url: https://docs.python.org/3.15/library/archiving.html
    title: Data Compression and Archiving, Python 3.15 documentation
    locator: Data Compression and Archiving
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/lzma.html
    title: lzma, Compression using the LZMA algorithm, Python 3.15 documentation
    locator: Introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/compression.html
    title: The compression package, Python 3.15 documentation
    locator: The compression package
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/zlib.html
    title: zlib, Compression compatible with gzip, Python 3.15 documentation
    locator: Introduction; crc32(); compressobj(); decompressobj()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/gzip.html
    title: gzip, Support for gzip files, Python 3.15 documentation
    locator: Introduction; open(); BadGzipFile
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/zipfile.html
    title: zipfile, Work with ZIP archives, Python 3.15 documentation
    locator: Introduction; ZipFile objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/tarfile.html
    title: tarfile, Read and write tar archive files, Python 3.15 documentation
    locator: Introduction; TarFile.extractall(); Extraction filters
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/compression.zstd.html
    title: compression.zstd, Compression compatible with the Zstandard format, Python 3.15 documentation
    locator: Introduction
    accessed: 2026-11-16
---

## Synopsis

```python run id=one-shot
import bz2
import lzma
import zlib
from compression import zstd

data = b"witch which has which witches wrist watch " * 50
for module in (zlib, bz2, lzma, zstd):
    packed = module.compress(data)
    print(module.__name__, len(packed) < len(data), module.decompress(packed) == data)
print(len(data), zlib.crc32(data[:41]))
```

```output for=one-shot
zlib True True
bz2 True True
lzma True True
compression.zstd True True
2100 226805979
```

## Semantics

**r1.** "The modules described in this chapter support data compression with the zlib, gzip, bzip2, lzma, and zstd algorithms, and the creation of ZIP- and tar-format archives."[^1] Example: [`one-shot`](#one-shot).

**r2.** A compression module works on bytes. `compress` takes bytes and returns bytes, and `decompress` reverses it. The `lzma` interface "is very similar to that of the bz2 module."[^2]

**r3.** The `compression` package groups the modules. "Some of these modules have historically been available as separate modules; those will continue to be available under their original names for compatibility reasons".[^3] `compression.zstd` is new, and it is a "Wrapper for the Zstandard compression library".[^3] Example: [`one-shot`](#one-shot).

**r4.** `zlib.compressobj` "Returns a compression object, to be used for compressing data streams that won’t fit into memory at once."[^4] `decompressobj` does the reverse. Example: [`incremental`](#incremental).

**r5.** `zlib.crc32` "Computes a CRC (Cyclic Redundancy Check) checksum of data." "The algorithm is not cryptographically strong".[^4]

**r6.** "This module provides a simple interface to compress and decompress files just like the GNU programs gzip and gunzip would."[^5] `gzip.open` returns a file object, in binary or text mode. Example: [`gzip-file`](#gzip-file).

**r7.** "The ZIP file format is a common archive and compression standard."[^6] `zipfile.ZipFile` opens an archive with mode "'r' to read an existing file, 'w' to truncate and write a new file, 'a' to append to an existing file, or 'x' to exclusively create and write a new file."[^6] Example: [`zip-archive`](#zip-archive).

**r8.** "The tarfile module makes it possible to read and write tar archives, including those using gzip, bz2 and lzma compression."[^7] The mode string names the compression, as in `"w:gz"` and `"r:gz"`. Example: [`tar-archive`](#tar-archive).

**r9.** An extraction filter limits what `extractall` can do. The `data` filter disallows "some dangerous features such as links to absolute paths or paths outside of the destination."[^7] Even so, "Never extract archives from untrusted sources without prior inspection."[^7] Example: [`tar-outside`](#tar-outside).

## Examples

```python run id=gzip-file
import gzip
import os
import tempfile

path = os.path.join(tempfile.mkdtemp(), "notes.txt.gz")
with gzip.open(path, "wt", encoding="utf-8") as f:
    f.write("line one\nline two\n")
with gzip.open(path, "rt", encoding="utf-8") as f:
    print(f.read().splitlines())
with open(path, "rb") as f:
    print(f.read(2))
```

```output for=gzip-file
['line one', 'line two']
b'\x1f\x8b'
```

Mode `"wt"` writes text and `"rt"` reads it back (rule r6). The file on disk is gzip data, which starts with the two bytes `1f 8b`.

```python run id=zip-archive
import io
import zipfile

buffer = io.BytesIO()
with zipfile.ZipFile(buffer, "w", compression=zipfile.ZIP_DEFLATED) as zf:
    zf.writestr("readme.txt", "hello\n")
    zf.writestr("data/values.csv", "a,b\n1,2\n")
with zipfile.ZipFile(buffer) as zf:
    print(zf.namelist())
    print(zf.read("data/values.csv").decode().splitlines())
    info = zf.getinfo("readme.txt")
    print(info.file_size, info.compress_type == zipfile.ZIP_DEFLATED)
print(zipfile.is_zipfile(buffer))
```

```output for=zip-archive
['readme.txt', 'data/values.csv']
['a,b', '1,2']
6 True
True
```

A `ZipFile` takes a path or a file object, so an archive can live in memory. `ZIP_DEFLATED` is "the usual ZIP compression method".[^6] The default, `ZIP_STORED`, does not compress.

```python run id=tar-archive
import io
import os
import tarfile
import tempfile

def add(tar, name, text):
    payload = text.encode()
    info = tarfile.TarInfo(name)
    info.size = len(payload)
    tar.addfile(info, io.BytesIO(payload))

buffer = io.BytesIO()
with tarfile.open(fileobj=buffer, mode="w:gz") as tar:
    add(tar, "pkg/a.txt", "alpha")
    add(tar, "pkg/b.txt", "beta")
buffer.seek(0)
target = tempfile.mkdtemp()
with tarfile.open(fileobj=buffer, mode="r:gz") as tar:
    print(tar.getnames())
    tar.extractall(target, filter="data")
print(sorted(os.listdir(os.path.join(target, "pkg"))))
```

```output for=tar-archive
['pkg/a.txt', 'pkg/b.txt']
['a.txt', 'b.txt']
```

A `TarInfo` holds the name and size of a member, and `addfile` reads the bytes from a file object (rule r8). Pass the filter name to say which filter you want. The docs recommend it "to support Python versions with a less secure default (3.13 and lower)."[^7]

```python run id=incremental
import zlib

compressor = zlib.compressobj()
chunks = [compressor.compress(b"part %d;" % i) for i in range(1000)]
chunks.append(compressor.flush())
packed = b"".join(chunks)
decompressor = zlib.decompressobj()
restored = decompressor.decompress(packed) + decompressor.flush()
print(restored[:14], len(restored), len(packed) < len(restored))
```

```output for=incremental
b'part 0;part 1;' 8890 True
```

The compressor takes the data in pieces, and `flush` returns the last of the output (rule r4). A program can stream a large file this way and keep only one piece in memory.

## Versions

> [!since v="3.14"]
> The `compression` package and the `compression.zstd` module are new.[^3][^8] The default extraction filter of `tarfile` is `data`: "Previously, the filter strategy was equivalent to fully_trusted."[^7]

## Pitfalls

```python wrong id=zip-mode-w
import io
import zipfile

buffer = io.BytesIO()
with zipfile.ZipFile(buffer, "w") as zf:
    zf.writestr("first.txt", "1")
with zipfile.ZipFile(buffer, "w") as zf:
    zf.writestr("second.txt", "2")
print(zipfile.ZipFile(buffer).namelist())
buffer = io.BytesIO()
with zipfile.ZipFile(buffer, "w") as zf:
    zf.writestr("first.txt", "1")
with zipfile.ZipFile(buffer, "a") as zf:
    zf.writestr("second.txt", "2")
print(zipfile.ZipFile(buffer).namelist())
```

```output for=zip-mode-w
['second.txt']
['first.txt', 'second.txt']
```

Mode `"w"` truncates the archive, so the second open removes `first.txt`. Use `"a"` to add members (rule r7).

```python fails id=tar-outside expect=exception:OutsideDestinationError
import io
import tarfile
import tempfile

buffer = io.BytesIO()
with tarfile.open(fileobj=buffer, mode="w") as tar:
    info = tarfile.TarInfo("../escape.txt")
    info.size = 2
    tar.addfile(info, io.BytesIO(b"hi"))
buffer.seek(0)
with tarfile.open(fileobj=buffer) as tar:
    tar.extractall(tempfile.mkdtemp())
```

The member name `../escape.txt` points outside the target directory. The `data` filter rejects it before it writes a file (rule r9). The old default, `fully_trusted`, does not check names: it does "Honor all metadata as specified in the archive."[^7]

```python fails id=not-gzip expect=exception:BadGzipFile
import gzip

print(gzip.decompress(b"plain text, not gzip"))
```

`BadGzipFile` is "An exception raised for invalid gzip files. It inherits from OSError."[^5] Each module has its own error for bad input, such as `zlib.error` and `compression.zstd.ZstdError`.

## Comparison

The modules differ in speed and ratio. The zstd manual describes Zstandard as "a fast lossless compression algorithm, targeting real-time compression scenarios at zlib-level and better compression ratios."[^8] For `gzip`, "The data compression is provided by the zlib module."[^5] `ZIP_DEFLATED` also "requires the zlib module."[^6] For a whole directory tree, the archiving chapter points to the higher-level functions of `shutil`.[^1]
