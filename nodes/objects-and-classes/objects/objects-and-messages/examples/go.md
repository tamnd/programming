```go run id=values-with-methods
package main

import "fmt"

type Celsius float64

func (c Celsius) String() string { return fmt.Sprintf("%.1f C", float64(c)) }

type Counter struct{ n int }

func (c *Counter) Inc() { c.n++ }

func newGreeter(greeting string) func(string) string {
	return func(name string) string { return greeting + ", " + name } // state in a closure
}

func main() {
	things := []any{42, "hi", []int{1, 2}, Celsius(21.5), &Counter{}}
	for _, t := range things {
		fmt.Printf("%T ", t)
	}
	fmt.Println()

	var s fmt.Stringer = Celsius(21.5) // any type with String answers this message
	fmt.Println(s.String())

	a := &Counter{}
	b := a // a second pointer to the same struct
	b.Inc()
	c := *a // a copy of the struct value
	c.Inc()
	fmt.Println(a.n, c.n, a == b)

	hi := newGreeter("Hi")
	fmt.Println(hi("ana"))
}
```

```output for=values-with-methods
int string []int main.Celsius *main.Counter 
21.5 C
1 2 true
Hi, ana
```

- Verdict: idiom. Go does not use the word object, but any named type can have methods, even a type based on `float64` such as `Celsius`. A value of any type can go in an interface value, which holds the value and its dynamic type, and `%T` prints that type. An interface such as `fmt.Stringer` names a message, and every type with a matching method answers it.
- Struct values copy on assignment, so `c := *a` makes a separate counter, while pointers share one. A closure that holds its own variables is a small object with one method, as `newGreeter` shows.
