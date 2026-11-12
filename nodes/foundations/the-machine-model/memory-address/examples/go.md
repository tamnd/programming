```go run id=unsafe-pointer-arithmetic
package main

import (
	"fmt"
	"unsafe"
)

type Rec struct {
	Flag  byte
	Count int32
	Tag   byte
}

func main() {
	xs := [4]int32{10, 20, 30, 40}
	p0 := &xs[0]
	p2 := &xs[2]
	// uintptr is a plain integer: subtracting two addresses gives a distance in bytes.
	fmt.Println("distance:", uintptr(unsafe.Pointer(p2))-uintptr(unsafe.Pointer(p0)))

	// unsafe.Add moves a pointer by a number of bytes.
	p3 := (*int32)(unsafe.Add(unsafe.Pointer(p0), 12))
	fmt.Println("value 12 bytes after xs[0]:", *p3)

	var r Rec
	fmt.Println("offsets:", unsafe.Offsetof(r.Flag), unsafe.Offsetof(r.Count), unsafe.Offsetof(r.Tag))
	fmt.Println("size:", unsafe.Sizeof(r), "align:", unsafe.Alignof(r))

	s := xs[1:3] // a slice holds the address of its first element
	fmt.Println("slice starts at xs[1]:", unsafe.SliceData(s) == &xs[1])
}
```

```output for=unsafe-pointer-arithmetic
distance: 8
value 12 bytes after xs[0]: 40
offsets: 0 4 8
size: 12 align: 4
slice starts at xs[1]: true
```

- Verdict: native. Go has `&` and `*` but no pointer arithmetic in normal code. The `unsafe` package converts a pointer to a `uintptr` to measure a distance, and `unsafe.Add` moves a pointer by bytes. `unsafe.Offsetof`, `Sizeof` and `Alignof` give the layout of a struct. `unsafe.SliceData` gives the pointer to the first element of a slice, which shows that a slice of an array shares its memory.
