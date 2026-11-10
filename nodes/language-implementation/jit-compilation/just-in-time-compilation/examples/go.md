```go run id=engine
package main

import (
	"fmt"
	"slices"
	"strconv"
	"strings"
)

// Values carry their type, as boxed values do in a dynamic language.
type Value interface{ Kind() string }
type IntV struct{ N int64 }
type FloatV struct{ X float64 }

func (IntV) Kind() string     { return "int" }
func (FloatV) Kind() string   { return "float" }
func (v IntV) String() string { return strconv.FormatInt(v.N, 10) }
func (v FloatV) String() string {
	return strconv.FormatFloat(v.X, 'g', -1, 64)
}

// The body of f(x) = x * x + 1 as a tree.
type Node interface{}
type Num struct{ Value int64 }
type Arg struct{}
type Bin struct {
	Op          byte
	Left, Right Node
}

func intOp(op byte) func(a, b int64) int64 {
	if op == '+' {
		return func(a, b int64) int64 { return a + b }
	}
	return func(a, b int64) int64 { return a * b }
}

func asFloat(v Value) float64 {
	if i, ok := v.(IntV); ok {
		return float64(i.N)
	}
	return v.(FloatV).X
}

// Generic arithmetic: check both tags on every operation.
func arith(op byte, a, b Value) Value {
	x, xok := a.(IntV)
	y, yok := b.(IntV)
	if xok && yok {
		return IntV{intOp(op)(x.N, y.N)}
	}
	p, q := asFloat(a), asFloat(b)
	if op == '+' {
		return FloatV{p + q}
	}
	return FloatV{p * q}
}

// The interpreter: walk the tree on every call.
func interpret(node Node, arg Value) Value {
	switch n := node.(type) {
	case Num:
		return IntV{n.Value}
	case Arg:
		return arg
	case Bin:
		return arith(n.Op, interpret(n.Left, arg), interpret(n.Right, arg))
	}
	panic("unknown node")
}

// Compiled code for int: closures over raw int64 values, no tag checks inside.
func compileInt(node Node) func(int64) int64 {
	switch n := node.(type) {
	case Num:
		v := n.Value
		return func(int64) int64 { return v }
	case Arg:
		return func(x int64) int64 { return x }
	case Bin:
		f, l, r := intOp(n.Op), compileInt(n.Left), compileInt(n.Right)
		return func(x int64) int64 { return f(l(x), r(x)) }
	}
	panic("unknown node")
}

// Compiled code for any argument: closures that still check tags.
func compileGeneric(node Node) func(Value) Value {
	switch n := node.(type) {
	case Num:
		boxed := IntV{n.Value}
		return func(Value) Value { return boxed }
	case Arg:
		return func(a Value) Value { return a }
	case Bin:
		op, l, r := n.Op, compileGeneric(n.Left), compileGeneric(n.Right)
		return func(a Value) Value { return arith(op, l(a), r(a)) }
	}
	panic("unknown node")
}

const threshold = 3

type JitFunction struct {
	name   string
	body   Node
	calls  int                       // calls in the interpreter since the last compilation
	seen   map[string]bool           // the profile: the kinds of the arguments
	code   func(Value) (Value, bool) // compiled code; false means that the guard failed
	kind   string
	counts [3]int // interpreter, compiled, deoptimizations
}

func (f *JitFunction) compile() {
	kinds := make([]string, 0, len(f.seen))
	for k := range f.seen {
		kinds = append(kinds, k)
	}
	slices.Sort(kinds)
	how := "generic"
	if len(kinds) == 1 && kinds[0] == "int" {
		raw := compileInt(f.body)
		f.code = func(a Value) (Value, bool) {
			i, ok := a.(IntV) // the guard
			if !ok {
				return nil, false
			}
			return IntV{raw(i.N)}, true
		}
		f.kind, how = "int", "specialized for int"
	} else {
		generic := compileGeneric(f.body)
		f.code = func(a Value) (Value, bool) { return generic(a), true }
		f.kind = "generic"
	}
	fmt.Printf("* compile %s: arguments seen: %s; %s\n", f.name, strings.Join(kinds, ", "), how)
}

func (f *JitFunction) Call(arg Value) (Value, string) {
	if f.code == nil && f.calls >= threshold {
		f.compile()
	}
	if f.code != nil {
		if result, ok := f.code(arg); ok {
			f.counts[1]++
			return result, "compiled, " + f.kind
		}
		fmt.Printf("* deoptimize %s: guard failed, %v is not an int\n", f.name, arg)
		f.counts[2]++
		f.code, f.calls = nil, 0
	}
	f.calls++
	f.counts[0]++
	f.seen[arg.Kind()] = true
	return interpret(f.body, arg), fmt.Sprintf("interpreter, call %d", f.calls)
}

func main() {
	f := &JitFunction{name: "f", body: Bin{'+', Bin{'*', Arg{}, Arg{}}, Num{1}}, seen: map[string]bool{}}
	for _, arg := range []Value{IntV{1}, IntV{2}, IntV{3}, IntV{4}, FloatV{2.5}, IntV{6}, IntV{7}, IntV{8}} {
		result, tier := f.Call(arg)
		fmt.Printf("f(%v) = %v [%s]\n", arg, result, tier)
	}
	fmt.Printf("interpreter: %d, compiled: %d, deoptimizations: %d\n", f.counts[0], f.counts[1], f.counts[2])
}
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: idiom. Go compiles ahead of time to machine code, so a JIT is something a program builds, as here.
- `i, ok := a.(IntV)` is the guard. The type assertion with two results tests the dynamic type without a panic.
- A Go map has no order, so `compile` sorts the kinds of the profile before it prints them.
- Go has no sum types. The interfaces `Value` and `Node` stand in for them, and each `switch` ends in a `panic` for a case that the compiler cannot rule out.
