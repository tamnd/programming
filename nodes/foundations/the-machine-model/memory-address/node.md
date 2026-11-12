---
tag: 6D9A12
kind: concept
slug: memory-address
name: Memory address
aliases: [memory address, address, addresses, byte address, virtual address, physical address, address space, alignment, offset, address arithmetic]
area: foundations/the-machine-model
level: core
summary: >-
  A memory address is the number of a byte in memory. A value that takes several bytes lives at the address of its first byte, and pointers and references are addresses with rules on top.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1945
  people: [John von Neumann, Tom Kilburn, Frederic Williams, Fritz-Rudolf Güntsch]
  note: The EDVAC report of 1945 described one memory of numbered words for both code and data. Virtual memory, which gives each program its own addresses, came with the Atlas computer in 1962.
see_also: [pointers-and-references, processor-and-memory, bits-and-bytes, call-stack-and-heap, stack-and-heap, use-after-free, out-of-bounds-read, garbage-collection]
same_as:
  wikidata: Q835713
  wikipedia: Memory_address
sources:
- url: https://en.cppreference.com/w/c/language/pointer.html
  title: Pointer declaration, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/c/types/offsetof.html
  title: offsetof, C reference, cppreference.com
  locator: Description
- url: https://docs.python.org/3/library/functions.html
  title: Built-in Functions, Python documentation
  locator: id()
- url: https://pkg.go.dev/unsafe
  title: Package unsafe, Go standard library documentation
  locator: type Pointer
- url: https://doc.rust-lang.org/std/ptr/index.html
  title: Module ptr, Rust standard library documentation
  locator: Provenance, Pointers Vs Integers
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/foreign/MemorySegment.html
  title: Interface MemorySegment, Java SE 25 API documentation
  locator: address() and asSlice
- url: https://learn.microsoft.com/en-us/dotnet/api/system.runtime.compilerservices.unsafe.byteoffset
  title: Unsafe.ByteOffset Method, .NET API documentation
  locator: Definition
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/DataView
  title: DataView, MDN Web Docs
  locator: Description
- url: https://docs.python.org/3/library/ctypes.html
  title: ctypes, A foreign function library for Python, Python documentation
  locator: addressof and alignment
---

## Idea

Memory is a long row of bytes, and each byte has a number, its address. To read or write a value, the processor needs its address. A value that takes several bytes, such as a 4-byte integer, lives at the address of its first byte, so the items of an array are a fixed number of bytes apart, and the fields of a structure are at fixed offsets from its start. Every variable, object and function in a running program is at some address, and a pointer or a reference is, at the bottom, an address that the language lets the program use.

```python run id=idea
import ctypes

Int32x4 = ctypes.c_int32 * 4
a = Int32x4(10, 20, 30, 40)
base = ctypes.addressof(a)  # a number, different on each run
size = ctypes.sizeof(ctypes.c_int32)
for i in range(4):
    addr = base + i * size  # element i is i * 4 bytes after the start
    print(f"a[{i}] at base + {addr - base}: {ctypes.c_int32.from_address(addr).value}")

raw = bytes((ctypes.c_ubyte * 8).from_address(base))
print("first 8 bytes:", raw.hex(" "))  # little-endian: low byte first

ctypes.c_int32.from_address(base + 2 * size).value = 99  # write through an address
print("after a write at base + 8:", list(a))


class Record(ctypes.Structure):
    _fields_ = [("flag", ctypes.c_char), ("count", ctypes.c_int32), ("tag", ctypes.c_char)]


for name, _ in Record._fields_:
    print(f"Record.{name} at offset {getattr(Record, name).offset}")
print("sizeof(Record):", ctypes.sizeof(Record), "alignment:", ctypes.alignment(Record))
```

```output for=idea
a[0] at base + 0: 10
a[1] at base + 4: 20
a[2] at base + 8: 30
a[3] at base + 12: 40
first 8 bytes: 0a 00 00 00 14 00 00 00
after a write at base + 8: [10, 20, 99, 40]
Record.flag at offset 0
Record.count at offset 4
Record.tag at offset 8
sizeof(Record): 12 alignment: 4
```

Python hides addresses, but `ctypes` makes C data and exposes them. The program makes an array of four 32-bit integers and prints each address as an offset from the start, because the address itself is different on each run. Element `i` is at the start plus `4 * i`. The raw bytes show the order inside each integer: 10 is `0a 00 00 00`, low byte first, because the machine is little-endian. Writing at the start plus 8 changes `a[2]`. The structure has a 1-byte field, a 4-byte field and a 1-byte field, but the 4-byte field starts at offset 4 and the whole record takes 12 bytes, because the compiler aligns each field to a multiple of its size.

## How it works

**Addresses and sizes.** On most machines today the unit of addressing is the byte, and an address is an unsigned integer of 64 bits, of which 48 to 57 are in use on x86-64 and ARM64. A value of type `T` at address `a` takes the bytes from `a` to `a + size(T) - 1`. Element `i` of an array that starts at `a` is at `a + i * size(T)`, which is why indexing an array takes constant time. The byte order inside a value, little-endian or big-endian, belongs to the machine: x86 and almost all ARM systems are little-endian.

**Alignment and offsets.** Processors read memory fastest, and some only, at addresses that are a multiple of the size of the value. So each type has an alignment, and compilers place each field of a structure at an offset that is a multiple of its alignment, with padding bytes between fields and at the end. C has a macro for the offset: `offsetof` gives "the offset, in bytes, from the beginning of an object of specified type to its specified subobject, including padding if any."[^2] Python's `ctypes.alignment` and Rust's `align_of` give the alignment of a type.[^9] Putting the largest fields first reduces padding.

**Virtual addresses.** A program does not see the numbers of the physical memory chips. The operating system gives each process its own virtual address space, and the memory management unit of the processor translates each virtual address to a physical one through page tables, usually in pages of 4 KiB or more. So two processes can use the same address for different data, a process cannot read the memory of another, and an address that maps to no page causes a fault, such as a segmentation fault. The operating system also places the stack, the heap and the libraries at random addresses on each run, a defense called address space layout randomization, which is why the examples print offsets and not addresses.

**From addresses to pointers and references.** Languages build safer things on top of addresses. In C, a pointer "is a type of an object that refers to a function or an object of another type", and the operators `&` and `*` take an address and follow one.[^1] Pointer arithmetic counts in elements, not bytes. Go and Rust have pointers too, but rules limit what a program can do with them, and a raw address needs a special path: Go's `unsafe.Pointer` "allows a program to defeat the type system and read and write arbitrary memory".[^4] Rust warns that an address alone is not a pointer: "a usize cannot accurately represent a pointer, and converting from a pointer to a usize is generally an operation which only extracts the address."[^5] A pointer also carries its provenance, the allocation that it may access. Java, C#, JavaScript and Python give references, which name objects but expose no address. See [pointers and references](/pointers-and-references/).

**Moving objects.** A compacting garbage collector moves objects to new addresses and updates the references to them, so the address of an object in Java, C# or Go can change while the program runs. An integer that holds an old address is not updated: Go's documentation says that "Even if a uintptr holds the address of some object, the garbage collector will not update that uintptr's value if the object moves, nor will that uintptr keep the object from being reclaimed."[^4] C# pins an object to stop the collector from moving it while native code uses its address. In CPython objects never move, and `id()` gives the address: "CPython implementation detail: This is the address of the object in memory."[^3]

**Addresses outside the garbage-collected heap.** Code that talks to hardware, to files mapped into memory or to libraries in C needs real addresses. Java's foreign memory API gives memory segments with an `address()` and slices at offsets.[^6] .NET's `Unsafe.ByteOffset` "Determines the byte offset from origin to target from the given managed pointers."[^7] JavaScript has no addresses at all, but an `ArrayBuffer` is a block of bytes, typed arrays and `DataView` read it at byte offsets, and `DataView` reads values "without having to care about the platform's endianness".[^8]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | references only; `id()` in CPython; `ctypes.addressof` |
| JavaScript | absent | no addresses; byte offsets in an `ArrayBuffer` |
| TypeScript | absent | as JavaScript |
| Java | stdlib | references only; the foreign memory API for native memory |
| Kotlin | stdlib | references only; the JVM foreign memory API, or Kotlin/Native `CPointer` |
| C# | native | `&` and pointers in `unsafe` code; `ref`; pinning |
| Go | native | `&` and `*`; `unsafe.Pointer`, `uintptr`, `unsafe.Offsetof` |
| Rust | native | references; raw pointers, `addr`, `offset_of!` |
| C | native | `&`, `*`, pointer arithmetic, `offsetof`, `uintptr_t` |
| C++ | native | as C, with `std::addressof` and references |
| Bash | absent | no access to memory |
| PowerShell | stdlib | .NET interop types such as `IntPtr` |
| SQL | n/a | rows and keys, not addresses |
| Regex | n/a | positions in text, not addresses |
| HTML | n/a | no memory model |
| CSS | n/a | no memory model |

C and C++ expose addresses as a normal part of the language, and C#, Go and Rust expose them behind a marked unsafe path, so the verdict for these five is native. Python and Java give references in the language and addresses only through the standard library: `ctypes` and the foreign memory API. JavaScript has no addresses at all, and byte offsets inside an `ArrayBuffer` are the closest idea. The languages with a moving garbage collector, Java, C# and Go, keep addresses out of normal code so that the collector can move objects.

The example files show the same layout from each language. Python shows identity and a `memoryview` that shares bytes. JavaScript reads one buffer through views of different sizes and byte orders. Java takes addresses of native memory and checks bounds. C# gets the distance between two array elements and reads through a pinned address. Go measures the distance with `uintptr` and moves a pointer with `unsafe.Add`. Rust compares the layout of a `repr(C)` struct with the default layout, which reorders fields. C uses `&`, `*`, pointer arithmetic and `offsetof`, and C++ shows a vector that moves its elements to a new address.

## Variations and related ideas

- **Word addressing.** Some old machines and some digital signal processors give an address to each word, not each byte.
- **Segmented addresses.** The 8086 formed an address from a segment and an offset; protected modes later used segments as a second level of mapping.
- **Tagged and compressed addresses.** ARM's top-byte ignore and memory tagging use the high bits of a 64-bit address for metadata, and the JVM can store references as 32-bit compressed offsets.
- **Memory-mapped I/O.** Device registers have addresses, so writing to an address can send a command to hardware.
- **Handles and indexes.** Many programs refer to objects by an index into an array or by a handle, which stays valid when the storage moves and can be checked.
- **Pointers and references.** [Pointers and references](/pointers-and-references/) are the language features built on addresses.

## Pitfalls

- **Keeping an address after the storage moves or ends.** A pointer into a vector, a string or a slice is invalid after the container grows, and a pointer to a local is invalid after the function returns. See [use after free](/use-after-free/).
- **Computing addresses out of bounds.** Arithmetic that goes past the end of an array reads or writes other data. See [out-of-bounds read](/out-of-bounds-read/).
- **Treating addresses as stable numbers.** Addresses change from run to run because of address space layout randomization, and objects move in garbage-collected languages. Do not print, hash or sort by them where the order matters.
- **Misaligned access.** Reading a 4-byte value at an address that is not a multiple of 4 is slow on some processors, faults on others and is undefined behavior in C and C++. Use `memcpy` to read unaligned data.
- **Assuming a layout.** Field order, padding and byte order depend on the compiler, the target and the language. Rust reorders fields unless the type is `repr(C)`. Write files and network data field by field.
- **Confusing elements and bytes.** In C, `p + 1` moves by one element, not one byte, so code that mixes byte offsets and element offsets lands in the wrong place.

## Cost

Computing an address costs one or two machine instructions: a base, plus an index times a size, plus a constant offset. The cost of using it depends on where the data is. A read from the L1 cache takes a few cycles, from main memory about a hundred times longer, and a read that misses the translation cache of the memory management unit adds a walk of the page tables. Data at nearby addresses shares cache lines of 64 bytes, so arrays read in order are fast and pointer-heavy structures that jump around memory are slow. Padding costs memory: a structure with badly ordered fields can be half padding.

## History

The EDVAC report of 1945, by John von Neumann, described a memory of numbered words that held both instructions and data, and the stored-program computers built from 1948 used this model. Tom Kilburn and Frederic Williams's Manchester Baby of 1948 had 32 words of memory, each with an address. Early machines addressed words; IBM's System/360 of 1964 made the 8-bit byte the unit of addressing, and most later machines followed.

Fritz-Rudolf Güntsch described virtual memory in 1956, and the Atlas computer at Manchester, from 1962, translated addresses through pages, so that programs saw more memory than the machine had. Address sizes grew from 16 bits in minicomputers to 32 bits in the 1980s and 64 bits in the 2000s. Languages moved the other way: C, from 1972, exposed addresses directly, while Lisp, Smalltalk, Java and their successors hid them behind references and let the garbage collector move objects.

## References
