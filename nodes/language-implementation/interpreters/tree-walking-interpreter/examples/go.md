```go run id=interpreter
package main

import "fmt"

type Expr interface{ isExpr() }
type Num struct{ Value int }
type Var struct{ Name string }
type Bin struct {
	Op          byte
	Left, Right Expr
}

func (Num) isExpr() {}
func (Var) isExpr() {}
func (Bin) isExpr() {}

type Stmt interface{ isStmt() }
type Let struct {
	Name string
	Init Expr
}
type Assign struct {
	Name  string
	Value Expr
}
type Block struct{ Body []Stmt }
type While struct {
	Cond Expr
	Body Stmt
}
type Print struct{ Value Expr }

func (Let) isStmt()    {}
func (Assign) isStmt() {}
func (Block) isStmt()  {}
func (While) isStmt()  {}
func (Print) isStmt()  {}

type Env struct {
	vars   map[string]int
	parent *Env
}

func newEnv(parent *Env) *Env { return &Env{vars: map[string]int{}, parent: parent} }

type Interpreter struct{ nodes, lookups, links int }

func (in *Interpreter) find(env *Env, name string) *Env {
	in.lookups++
	for e := env; e != nil; e = e.parent {
		if _, ok := e.vars[name]; ok {
			return e
		}
		in.links++
	}
	panic(fmt.Sprintf("undefined variable '%s'", name))
}

func (in *Interpreter) evaluate(node Expr, env *Env) int {
	in.nodes++
	switch n := node.(type) {
	case Num:
		return n.Value
	case Var:
		return in.find(env, n.Name).vars[n.Name]
	case Bin:
		a := in.evaluate(n.Left, env)
		b := in.evaluate(n.Right, env)
		switch n.Op {
		case '+':
			return a + b
		case '-':
			return a - b
		case '*':
			return a * b
		case '<':
			if a < b {
				return 1
			}
			return 0
		}
	}
	panic(fmt.Sprintf("unknown expression %#v", node))
}

func (in *Interpreter) execute(node Stmt, env *Env) {
	in.nodes++
	switch n := node.(type) {
	case Let:
		env.vars[n.Name] = in.evaluate(n.Init, env)
	case Assign:
		result := in.evaluate(n.Value, env)
		in.find(env, n.Name).vars[n.Name] = result
	case Block:
		inner := newEnv(env)
		for _, stmt := range n.Body {
			in.execute(stmt, inner)
		}
	case While:
		for in.evaluate(n.Cond, env) != 0 {
			in.execute(n.Body, env)
		}
	case Print:
		fmt.Println(in.evaluate(n.Value, env))
	default:
		panic(fmt.Sprintf("unknown statement %#v", node))
	}
}

func main() {
	program := Block{[]Stmt{
		Let{"n", Num{5}},
		Let{"acc", Num{1}},
		While{Bin{'<', Num{0}, Var{"n"}}, Block{[]Stmt{
			Let{"step", Var{"n"}},
			Assign{"acc", Bin{'*', Var{"acc"}, Var{"step"}}},
			Assign{"n", Bin{'-', Var{"n"}, Num{1}}},
		}}},
		Print{Var{"acc"}},
	}}

	in := &Interpreter{}
	in.execute(program, newEnv(nil))
	fmt.Println("nodes visited:", in.nodes)
	fmt.Printf("variable lookups: %d, parent links followed: %d\n", in.lookups, in.links)
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. Each node is a struct, and an interface with an unexported marker method (`isExpr`, `isStmt`) closes the set of node types to this package.
- A type switch selects the case and binds `n` to the concrete struct. The compiler does not check that the cases are complete, so each switch ends with a `panic`.
- `Env` holds a map and a pointer to its parent. The garbage collector frees the environment of a block after the block ends.
