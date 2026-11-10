```go run id=inspect
package main

import (
	"fmt"
	"reflect"
)

type Point struct {
	X int `json:"x"`
	Y int `json:"y"`
}

func (p Point) Sum() int { return p.X + p.Y }

func main() {
	var v any = Point{3, 4}
	t := reflect.TypeOf(v)
	val := reflect.ValueOf(v)
	fmt.Println("type:", t.Name(), "kind:", t.Kind())
	for i := 0; i < t.NumField(); i++ {
		f := t.Field(i)
		fmt.Printf("%s %s tag=%q = %v\n", f.Name, f.Type, f.Tag.Get("json"), val.Field(i))
	}
	out := val.MethodByName("Sum").Call(nil)
	fmt.Println("Sum() =", out[0].Int())
}
```

```output for=inspect
type: Point kind: struct
X int tag="x" = 3
Y int tag="y" = 4
Sum() = 7
```

`reflect.TypeOf` and `reflect.ValueOf` take an interface value and return its dynamic type and value. Struct tags, such as `json:"x"`, are strings attached to fields. The language gives them no meaning, and a program reads them through reflection.

```go fails expect=panic id=not-settable
package main

import (
	"fmt"
	"reflect"
)

type Point struct {
	X, Y int
}

func main() {
	p := Point{3, 4}
	v := reflect.ValueOf(p)
	v.Field(0).SetInt(10)
	fmt.Println(p)
}
```

`ValueOf(p)` receives a copy of `p`, so changing it would change nothing. The reflection value is not settable, and `SetInt` panics. The compiler accepts the program.

```go run id=settable
package main

import (
	"fmt"
	"reflect"
)

type Point struct {
	X, Y int
}

func main() {
	p := Point{3, 4}
	v := reflect.ValueOf(&p).Elem()
	fmt.Println("settable:", v.Field(0).CanSet())
	v.Field(0).SetInt(10)
	fmt.Println(p)
}
```

```output for=settable
settable: true
{10 4}
```

A pointer to `p` gives a reflection value that refers to `p` itself. Its exported fields are settable.

- Verdict: native. The package `reflect` gives the dynamic type (`reflect.Type`) and the value (`reflect.Value`) of any interface value, and it can read and set fields and call methods.
- Only exported fields are settable. A value is settable only if it refers to the original variable, for example through a pointer.
- Mistakes in reflective code panic at run time. The compiler sees only calls on `reflect.Value`.
