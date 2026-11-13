---
tag: 771ACQ
kind: concept
slug: sparse-file
name: Sparse file
aliases: [hole, file hole, sparse data set, apparent size, allocated size, SEEK_HOLE, SEEK_DATA, punch a hole, st_blocks]
area: data-and-persistence/files-and-file-systems
level: advanced
summary: >-
  A sparse file has holes: ranges that read as zero bytes but have no blocks on the disk. So its size and the space it uses differ, and a copy that reads and writes the zeros fills the holes.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1995
  people: [J. Meyering]
  note: POSIX lets a program seek past the end of a file and write there, which leaves a hole. The Wikipedia article on sparse files cites a 1995 change by Jim Meyering that gave GNU cp its --sparse option. POSIX.1-2024 added SEEK_HOLE and SEEK_DATA to lseek.
see_also: [file-system-concept, copy-a-file, truncate-a-file, get-the-size-of-a-file, file-metadata]
same_as:
  wikidata: Q936585
  wikipedia: Sparse_file
sources:
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/lseek.html
  title: The Open Group, POSIX.1-2024, lseek
  locator: Description, Errors, Rationale, Change History
- url: https://man7.org/linux/man-pages/man2/lseek.2.html
  title: lseek(2), Linux manual page
  locator: Description, Seeking file data and holes
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/ftruncate.html
  title: The Open Group, POSIX.1-2024, ftruncate
  locator: Description
- url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/sys_stat.h.html
  title: The Open Group, POSIX.1-2024, sys/stat.h
  locator: Description, Application Usage
- url: https://man7.org/linux/man-pages/man2/stat.2.html
  title: stat(2), Linux manual page
  locator: Notes
- url: https://man7.org/linux/man-pages/man2/fallocate.2.html
  title: fallocate(2), Linux manual page
  locator: Description, Deallocating file space
- url: https://man7.org/linux/man-pages/man1/du.1.html
  title: du(1), Linux manual page
  locator: --apparent-size
- url: https://man7.org/linux/man-pages/man1/cp.1.html
  title: cp(1), Linux manual page
  locator: --sparse
- url: https://download.samba.org/pub/rsync/rsync.1
  title: rsync(1) manpage
  locator: --sparse, -S
- url: https://learn.microsoft.com/en-us/windows/win32/fileio/sparse-files
  title: Sparse Files, Win32 apps, Microsoft Learn
  locator: Overview
- url: https://learn.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_set_sparse
  title: FSCTL_SET_SPARSE IOCTL (winioctl.h), Win32 apps, Microsoft Learn
  locator: Description
- url: https://docs.python.org/3/library/os.html
  title: The Python Standard Library, os
  locator: os.lseek, os.stat_result.st_blocks
- url: https://nodejs.org/api/fs.html
  title: Node.js, File system
  locator: Class fs.Stats, stats.blocks
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/StandardOpenOption.html
  title: Java SE 25 API, java.nio.file.StandardOpenOption
  locator: SPARSE
- url: https://learn.microsoft.com/en-us/dotnet/api/system.io.filestream.setlength?view=net-10.0
  title: FileStream.SetLength(Int64) Method (System.IO), .NET API, Microsoft Learn
  locator: Remarks
- url: https://pkg.go.dev/os
  title: Go standard library, package os
  locator: func (*File) Truncate, func (*File) Seek
- url: https://doc.rust-lang.org/std/os/unix/fs/trait.MetadataExt.html
  title: The Rust Standard Library, std::os::unix::fs::MetadataExt
  locator: fn blocks
- url: https://doc.rust-lang.org/std/fs/struct.File.html
  title: The Rust Standard Library, std::fs::File
  locator: fn set_len
- url: https://en.cppreference.com/w/cpp/filesystem/resize_file.html
  title: std::filesystem::resize_file, cppreference.com
  locator: Description, Notes
- url: https://en.wikipedia.org/wiki/Sparse_file
  title: Sparse file, Wikipedia
  locator: Disadvantages, Sparse files in Unix, References
---

## Idea

A file has a size, and it has blocks on the disk that hold its bytes. In most files the two agree. A sparse file has ranges, called holes, that read as zero bytes but have no blocks. A program makes a hole when it moves the file offset past the end of the file and writes there, or when it makes the file longer without writing. So a file of 64 MiB can use only one block of the disk.

```python run id=idea
import os
import tempfile

os.chdir(tempfile.mkdtemp())
MiB = 1024 * 1024
with open("sparse.bin", "wb") as f:
    f.seek(64 * MiB)     # move past the end; nothing is written yet
    f.write(b"end")      # this write leaves a hole of 64 MiB before it

st = os.stat("sparse.bin")
print("size:           ", st.st_size)
print("uses less space:", st.st_blocks * 512 < st.st_size)
with open("sparse.bin", "rb") as f:
    data = f.read()
print("hole reads as:  ", data[:4], "zero bytes:", data.count(0))

fd = os.open("sparse.bin", os.O_RDONLY)
print("first data at:  ", os.lseek(fd, 0, os.SEEK_DATA))
print("next hole at:   ", os.lseek(fd, 64 * MiB, os.SEEK_HOLE))
os.close(fd)
```

```output for=idea
size:            67108867
uses less space: True
hole reads as:   b'\x00\x00\x00\x00' zero bytes: 67108864
first data at:   67108864
next hole at:    67108867
```

The file has a size of 67108867 bytes, but it uses less space than that, because the first 64 MiB are a hole. A read of the hole gives zero bytes, the same as a read of zeros that were written. `SEEK_DATA` finds the first byte of data, and `SEEK_HOLE` after the data gives the end of the file, because each file has a hole at its end.

## How it works

**Make a hole.** POSIX lets the offset go past the end: "The lseek () function shall allow the file offset to be set beyond the end of the existing data in the file. If data is later written at this point, subsequent reads of data in the gap shall return bytes with the value 0 until data is actually written into the gap"[^1]. The seek alone does not change the size: "The lseek () function shall not, by itself, extend the size of a file"[^1]. `ftruncate` can also make a file longer: "If the file size is increased, the extended area shall appear as if it were zero-filled"[^3]. Linux calls the gap a hole: "subsequent reads of the data in the gap (a "hole") return null bytes ('\0') until data is actually written into the gap"[^2].

The standards say what a read gives, not what the disk holds. A file system with holes does not give blocks to the gap. Linux also has a call that removes blocks from a file that has them: with `FALLOC_FL_PUNCH_HOLE`, `fallocate` "deallocates space (i.e., creates a hole) in the byte range starting at offset and continuing for size bytes"[^6]. Then "partial filesystem blocks are zeroed, and whole filesystem blocks are removed from the file"[^6], and the size does not change. Wikipedia notes: "There is no portable system call to punch holes"[^20].

**Two sizes.** `st_size` is the size that a read sees, and `st_blocks` is the "Number of blocks allocated for this object"[^4]. The unit of `st_blocks` "is not defined within POSIX.1-2024. In some implementations it is 512 bytes. It may differ on a file system basis"[^4]. Linux warns: "Use of the st_blocks and st_blksize fields may be less portable"[^5]. Python documents it as the "Number of 512-byte blocks allocated for file. This may be smaller than st_size /512 when the file has holes"[^12]. The `du` command shows the space, and its `--apparent-size` option will "print apparent sizes rather than device usage; although the apparent size is usually smaller, it may be larger due to holes in ('sparse') files, internal fragmentation, indirect blocks, etc."[^7].

**Find the holes.** POSIX.1-2024 added two values of `whence`[^1]. "If whence is SEEK_HOLE, the file offset shall be set to the smallest location of a byte within a hole and not less than offset"[^1], and "If whence is SEEK_DATA, the file offset shall be set to the smallest location of a byte not within a hole and not less than offset"[^1]. Each file has a hole at its end: "all seekable files shall have a virtual hole starting at the current size of the file, whether or not the file is sparse"[^1]. When the offset is in the hole at the end, `SEEK_DATA` fails with `ENXIO`[^1].

These calls are a hint, not a map of the disk. POSIX says that "Not all bytes with the value zero need belong to a hole"[^1], and Linux says that "a filesystem is not obliged to report holes, so these operations are not a guaranteed mechanism for mapping the storage space actually allocated to a file"[^2]. A file system can report the whole file as data and give a correct answer.

**Copy a sparse file.** A plain copy reads each byte and writes it. It reads zeros from a hole and writes them as data, so the copy has no holes. A copy that knows about holes asks for each run of data and writes only that.

```python run id=copy-keeps-holes
import os
import tempfile

os.chdir(tempfile.mkdtemp())
MiB = 1024 * 1024
with open("sparse.bin", "wb") as f:
    f.write(b"head")
    f.seek(64 * MiB)                 # leave a hole of about 64 MiB
    f.write(b"tail")

def allocated(path):
    return os.stat(path).st_blocks * 512   # st_blocks counts 512-byte units

# A plain copy reads the zeros of the hole and writes them as data.
with open("sparse.bin", "rb") as src, open("plain.bin", "wb") as dst:
    dst.write(src.read())

# A copy that knows about holes asks for each run of data and skips the holes.
src = os.open("sparse.bin", os.O_RDONLY)
dst = os.open("holes.bin", os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o644)
size = os.fstat(src).st_size
pos = 0
while pos < size:
    try:
        start = os.lseek(src, pos, os.SEEK_DATA)
    except OSError:                  # ENXIO: only a hole is left
        break
    end = os.lseek(src, start, os.SEEK_HOLE)
    os.lseek(src, start, os.SEEK_SET)
    os.lseek(dst, start, os.SEEK_SET)
    os.write(dst, os.read(src, end - start))
    pos = end
os.ftruncate(dst, size)              # keep a hole at the end, if there is one
os.close(src)
os.close(dst)

same = open("plain.bin", "rb").read() == open("holes.bin", "rb").read()
print("same bytes:      ", same)
print("plain is sparse: ", allocated("plain.bin") < os.path.getsize("plain.bin"))
print("holes is sparse: ", allocated("holes.bin") < os.path.getsize("holes.bin"))
```

```output for=copy-keeps-holes
same bytes:       True
plain is sparse:  False
holes is sparse:  True
```

The two copies have the same bytes, but only the second one has holes. The last `ftruncate` sets the size, so a hole at the end of the source stays a hole. The tools do the same work. For GNU `cp`, "By default, sparse SOURCE files are detected by a crude heuristic and the corresponding DEST file is made sparse as well"[^8], and "Specify --sparse = always to create a sparse DEST file whenever the SOURCE file contains a long enough sequence of zero bytes"[^8]. The `rsync` option `--sparse` will "Try to handle sparse files efficiently so they take up less space on the destination"[^9].

**Windows.** NTFS has sparse files, but a program must ask for them. "When sparse file functionality is enabled, the system does not allocate hard disk drive space to a file except in regions where it contains nonzero data"[^10]. The control code `FSCTL_SET_SPARSE` "Marks the indicated file as sparse or not sparse"[^11]. For a read, "the file system returns the appropriate number of zeros in the buffer allocated for the read operation"[^10].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `seek` past the end, `os.truncate`; `os.lseek` with `os.SEEK_DATA` and `os.SEEK_HOLE`; `st_blocks` of `os.stat` |
| JavaScript | stdlib | Node.js `fs.ftruncateSync` or a write at a position; `stats.blocks` of `fs.statSync` |
| TypeScript | stdlib | the `node:fs` module of Node.js, with the types of `@types/node` |
| Java | stdlib | `StandardOpenOption.SPARSE` as a hint; a write at a position of a `FileChannel`; no call for the allocated size |
| Kotlin | stdlib | the `java.nio` calls of the JVM |
| C# | stdlib | `FileStream.SetLength` and `Seek`; on Windows, `FSCTL_SET_SPARSE` through `DeviceIoControl` |
| Go | stdlib | `File.Seek` and `File.Truncate`; `Blocks` of `syscall.Stat_t` on Unix |
| Rust | stdlib | `File::set_len`; `MetadataExt::blocks` on Unix |
| C | library | ISO C has no holes; POSIX `lseek`, `ftruncate`, `SEEK_DATA` and `SEEK_HOLE`, `st_blocks` |
| C++ | stdlib | `std::filesystem::resize_file`; the POSIX calls for the holes |
| Bash | native | `truncate -s`, `dd` with `seek`, `du --apparent-size`, `cp --sparse` |
| PowerShell | native | `fsutil sparse` and the .NET calls |
| SQL | n/a | a database stores rows, and the engine chooses the layout of its files |
| Regex | n/a | a pattern matches text and does not write files |
| HTML | n/a | a page does not write files |
| CSS | n/a | style rules do not write files |

Each language with files can make a hole, because a seek past the end and a longer size are ordinary calls. The C++ notes say: "On systems that support sparse files, increasing the file size does not increase the space it occupies on the file system: space allocation takes place only when non-zero bytes are written to the file"[^19]. Rust's `set_len` "Truncates or extends the underlying file, updating the size of this file to become size"[^18], and Go's `Truncate` "changes the size of the file. It does not change the I/O offset"[^16].

The allocated size is less portable. Rust has `blocks` only on Unix, and it "Returns the number of blocks allocated to the file, in 512-byte units. Please note that this may be smaller than st_size / 512 when the file has holes"[^17]. Node.js has `stats.blocks`, "The number of blocks allocated for this file"[^13]. Java has no call for it. Its `SPARSE` option is a hint: "When used with the CREATE_NEW option then this option provides a hint that the new file will be sparse. The option is ignored when the file system does not support the creation of sparse files"[^14]. In .NET, a longer size is not always zeros: "If the stream is expanded, the contents of the stream between the old and the new length are undefined on Windows, while on Linux, that space is filled with zeros"[^15].

The example files show a hole in Go, Rust, C, C++, Node.js and Java.

## Variations and related ideas

- **Punch a hole.** On Linux, `fallocate` with `FALLOC_FL_PUNCH_HOLE` removes the blocks of a range in a file that has them, and the range then reads as zeros[^6].
- **Preallocate.** This is the opposite of a hole. `fallocate` with no flags gives blocks to a range before a program writes it, and then "subsequent writes into the range specified by offset and size are guaranteed not to fail because of lack of disk space"[^6]. POSIX has `posix_fallocate` for the same work[^6].
- **Disk images and databases.** Wikipedia says: "Sparse files are commonly used for disk images , database snapshots , log files and in scientific applications"[^20]. A virtual disk of a fixed size uses space only for the blocks that the guest writes.
- **Copy and size.** A copy can keep or lose the holes, and the size of a file has two meanings. See [[copy-a-file]] and [[get-the-size-of-a-file]].

## Pitfalls

- **A copy fills the holes.** A program that reads a file and writes the bytes makes a copy with no holes, as the second example shows. A file of 1 GiB with 1 MiB of data becomes 1 GiB on the disk. Use a copy that knows about holes, such as `cp` or `rsync --sparse`[^9].
- **A write into a hole can fail for lack of space.** A hole has no blocks, so a write there needs new blocks. Wikipedia lists "disk-full or quota-exceeded errors when merely overwriting an existing portion of a file that happened to have been sparse"[^20]. A program that must not fail later can preallocate the space[^6].
- **The size is not the space.** `st_size`, `ls` and the size calls of each language give the apparent size. Use `st_blocks` or `du` for the space, and do not use the apparent size to plan the free space of a disk[^7].
- **The unit of st_blocks varies.** POSIX does not define it[^4]. Linux and Python use 512 bytes[^12], which is not the block size of the file system.
- **Holes are not portable.** Some file systems have no holes, and an archive or a network copy can lose them. A program must give the same result when the file has no holes, and use holes only to save space.

## Cost

A hole costs no blocks, only the metadata that marks it. A read of a hole gives zeros with no read of the disk. A write into a hole allocates blocks, so the file can become fragmented: Wikipedia says that "sparse files may become fragmented"[^20]. A copy that uses `SEEK_DATA` and `SEEK_HOLE` does work for each run of data, not for each byte, so it is fast on a file with large holes.

## History

POSIX lets a program seek past the end of a file and write there, and it requires a read of the gap to give zeros[^1]. The Wikipedia article on sparse files cites a change by Jim Meyering on 1995-12-21 that gave GNU `cp` its `--sparse` option, and a change by Andrew Tridgell on 1996-06-29 with "better sparse handling" in `rsync`[^20]. POSIX added `SEEK_HOLE` and `SEEK_DATA` in Issue 8: "Austin Group Defect 415 is applied, adding SEEK_HOLE and SEEK_DATA"[^1]. NTFS has sparse files that a program turns on with `FSCTL_SET_SPARSE`[^10][^11].
