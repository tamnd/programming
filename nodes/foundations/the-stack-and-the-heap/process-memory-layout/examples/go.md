```go run id=regions
package main

import (
	"fmt"
	"os"
	"reflect"
	"strconv"
	"strings"
	"unsafe"
)

var counter = 42 // a global with a value: the data segment

// where prints the permissions and the kind of the region that holds addr.
func where(name string, addr uintptr) {
	exe, _ := os.Executable()
	data, _ := os.ReadFile("/proc/self/maps")
	for _, line := range strings.Split(strings.TrimSpace(string(data)), "\n") {
		fields := strings.Fields(line)
		lo, hi, _ := strings.Cut(fields[0], "-")
		start, _ := strconv.ParseUint(lo, 16, 64)
		end, _ := strconv.ParseUint(hi, 16, 64)
		if uint64(addr) >= start && uint64(addr) < end {
			path := strings.Join(fields[min(5, len(fields)):], " ")
			kind := path
			if path == exe {
				kind = "the program file"
			} else if path == "" || strings.HasPrefix(path, "[anon") {
				kind = "an anonymous mapping"
			}
			fmt.Printf("%-8s %s %s\n", name, fields[1], kind)
			return
		}
	}
}

func main() {
	here := 1                 // a local: the stack of the goroutine
	small := new(int)         // a small block
	big := make([]byte, 1<<20) // 1 MiB: too large for the stack

	where("code", reflect.ValueOf(where).Pointer())
	where("global", uintptr(unsafe.Pointer(&counter)))
	where("local", uintptr(unsafe.Pointer(&here)))
	where("small", uintptr(unsafe.Pointer(small)))
	where("big", uintptr(unsafe.Pointer(&big[0])))
}
```

```output for=regions
code     r-xp the program file
global   rw-p the program file
local    rw-p an anonymous mapping
small    rw-p an anonymous mapping
big      rw-p an anonymous mapping
```

- Verdict: native. `unsafe.Pointer` turns a pointer into an address. The code and the global are in the program file, as in C.
- The Go runtime takes its memory with `mmap`. The stack of a goroutine and the heap of Go are both in anonymous mappings, so this program has nothing in `[heap]` or `[stack]`.
