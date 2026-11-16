---
tag: 41PBH1
kind: task
slug: use-a-stack
name: Use a stack
aliases: [push and pop, stack push pop peek, lifo stack, check balanced brackets, matching parentheses]
area: data-and-values/stacks-queues-and-trees
level: intro
summary: Check that the brackets of a text match and nest, with push, pop and peek on the standard stack type of each tier 1 language.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/tutorial/datastructures.html
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1 More on Lists; 5.1.1 Using Lists as Stacks
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/push
    title: Array.prototype.push(), MDN Web Docs
    locator: the introduction; Return value
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/vec/struct.Vec.html
    title: Struct Vec, The Rust Standard Library
    locator: the description; pop
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Index expressions; Appending to and copying slices
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Stack.html
    title: Stack (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/ArrayDeque.html
    title: ArrayDeque (Java SE 27 and JDK 27)
    locator: the class description; poll; pop
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/stack
    title: std::stack, cppreference.com
    locator: the introduction
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.stack-1?view=net-11.0
    title: Stack<T> Class, .NET API reference
    locator: the summary; Methods
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: 6.7 Arrays
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/pop
    title: Array.prototype.pop(), MDN Web Docs
    locator: Return value; Description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/stack/pop
    title: std::stack::pop, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/deque/pop_back
    title: std::deque::pop_back, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.stack-1.pop?view=net-11.0
    title: Stack<T>.Pop Method, .NET API reference
    locator: Exceptions
    accessed: 2026-11-16
---

## Task

Use a stack to check the brackets of a text. The function `balanced(text)` returns true if each opening bracket `(`, `[` or `{` has a closing bracket of the same kind after it, and the pairs nest without crossing. The other characters do not count. For example, `balanced("f(a[0], {b: 1})")` is true, and `balanced("([)]")` is false.

The solution reads the text from left to right. It pushes each opening bracket. At a closing bracket, it pops the top and compares the two brackets. At the end, the stack must be empty. The most recent open bracket must close first, and that is the order of a stack. For the idea, see [[stack-data-structure]].

## Solutions

| Language | Stack | Push, pop and top |
|---|---|---|
| Python | `list` | `append(x)`, `pop()`, `stack[-1]` |
| JavaScript | `Array` | `push(x)`, `pop()`, `stack[stack.length - 1]` |
| TypeScript | `string[]` | `push(x)`, `pop()` with the type `string \| undefined` |
| Java | `ArrayDeque<Character>` as a `Deque` | `push(x)`, `pop()` or `poll()`, `peek()` |
| C | a `char` array and a count | `stack[count++] = x`, `count--`, `stack[count - 1]` |
| C++ | `std::stack<char>` | `push(x)`, `pop()`, `top()` |
| C# | `Stack<char>` | `Push(x)`, `Pop()` or `TryPop(out x)`, `Peek()` |
| Go | a slice `[]rune` | `append(s, x)`, `s = s[:n-1]`, `s[n-1]` |
| Rust | `Vec<char>` | `push(x)`, `pop()` that gives an `Option`, `last()` |
| Bash | an indexed array | `stack+=("$x")`, `unset 'stack[-1]'`, `${stack[-1]}` |

- **A list or an array is often the stack.** The Python tutorial says: "The list methods make it very easy to use a list as a stack, where the last element added is the first element retrieved"[^1]. A JavaScript array works in the same way. `push()` "adds the specified elements to the end of an array and returns the new length of the array."[^2] The Rust documentation shows the same use: "Use a Vec<T> as an efficient stack"[^3]. Go has no stack type. The solution uses a slice. The function `append` "appends zero or more values x to a slice s of type S and returns the resulting slice"[^4], and a shorter slice removes the top.
- **Java uses `ArrayDeque`, not `Stack`.** The documentation of the old `Stack` class says: "A more complete and consistent set of LIFO stack operations is provided by the Deque interface and its implementations, which should be used in preference to this class."[^5] `ArrayDeque` "is likely to be faster than Stack when used as a stack"[^6]. The solution uses `poll()`, because it "returns null if this deque is empty"[^6]. `pop()` throws an exception in that case.
- **C++ and C# have a stack type.** `std::stack` is "a container adaptor that gives the programmer the functionality of a stack - specifically, a LIFO (last-in, first-out) data structure."[^7] In .NET, `Stack<T>` "Represents a variable size last-in-first-out (LIFO) collection of instances of the same specified type."[^8] The C# solution uses `TryPop`, which "Returns a value that indicates whether there is an object at the top of the Stack<T>"[^8].
- **C makes a stack from an array.** The text has `n` characters, so the stack never holds more than `n` brackets. The solution gets `n + 1` bytes with `malloc`, keeps a count, and frees the array before it returns.
- **Bash uses an indexed array.** The index `-1` is the last element: "an index of -1 references the last element."[^9] `unset 'stack[-1]'` removes it.
- **Rust and TypeScript show the empty case in the type.** Rust `pop` "Removes the last element from a vector and returns it, or None if it is empty."[^3] The Rust solution compares `stack.pop()` with `Some(want)`, so an empty stack gives false with no extra test.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `no-brackets`: a text with no brackets is balanced.
- `unclosed`: `((` leaves two brackets on the stack. A solution that does not test the stack at the end returns true.
- `extra-close`: `())` pops from an empty stack at the third character.
- `close-first`: `)(` has one bracket of each kind, but the closing bracket comes first. A count of open brackets that goes below 0 finds this too, but a count does not find `wrong-kind`.
- `wrong-kind`: `(]` pops `(` for `]`.
- `deep`: 1000 opening brackets and then 1000 closing brackets. The stack holds 1000 items. The solutions use no recursion, so the depth of the call stack does not change.

## Pitfalls

- **Python `pop()` on an empty list raises an exception.** "It raises an IndexError if the list is empty or the index is outside the list range."[^1] `stack[-1]` on an empty list also raises `IndexError`. Test the stack before you pop or read the top:

```python run id=python-empty-pop
stack = []
try:
    stack.pop()
except IndexError:
    print("pop: IndexError")
try:
    stack[-1]
except IndexError:
    print("top: IndexError")
print("top:", stack[-1] if stack else None)
```

```output for=python-empty-pop
pop: IndexError
top: IndexError
top: None
```

- **Do not forget the test at the end.** A function that only checks each closing bracket accepts a text with open brackets that are left. Return `not stack` (or `len(stack) == 0`) in place of `True`:

```python wrong id=python-no-final-test
def balanced(text):
    stack = []
    for c in text:
        if c == "(":
            stack.append(c)
        elif c == ")":
            if not stack:
                return False
            stack.pop()
    return True

print(balanced("(("))
```

```output for=python-no-final-test
True
```

- **JavaScript `push` returns the length, not the array.** The return value is "The new length property of the object upon which the method was called."[^2] If you assign it back to the stack, the stack becomes a number. And `pop()` on an empty array does not throw: "If you call pop() on an empty array, it returns undefined"[^10].

```javascript run id=javascript-push-result
let stack = [];
stack = stack.push("a");
console.log(stack);
try {
  stack.push("b");
} catch (e) {
  console.log(e.constructor.name);
}
console.log([].pop());
```

```output for=javascript-push-result
1
TypeError
undefined
```

- **Java `ArrayDeque` refuses null, and `pop()` throws on an empty stack.** "Null elements are prohibited."[^6] `pop()` throws "NoSuchElementException - if this deque is empty"[^6], and `poll()` returns null:

```java run id=java-deque-null
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.NoSuchElementException;

public class Main {
  public static void main(String[] args) {
    Deque<String> stack = new ArrayDeque<>();
    try {
      stack.push(null);
    } catch (NullPointerException e) {
      System.out.println("push(null): NullPointerException");
    }
    try {
      stack.pop();
    } catch (NoSuchElementException e) {
      System.out.println("pop(): NoSuchElementException");
    }
    System.out.println("poll(): " + stack.poll());
  }
}
```

```output for=java-deque-null
push(null): NullPointerException
pop(): NoSuchElementException
poll(): null
```

- **C++ `pop()` returns nothing, and an empty stack is not checked.** `std::stack::pop` "Removes the top element from the stack. Effectively calls c.pop_back()"[^11]. It returns `void`, so read `top()` first. The default container is `std::deque`, and for its `pop_back` on an empty container, "the behavior is undefined"[^12] (until C++26). Test `empty()` before `top()` and `pop()`.
- **C# `Pop` throws on an empty stack.** The `Pop` page gives `InvalidOperationException` for the case "The Stack<T> is empty."[^13] Use `TryPop` when the stack can be empty:

```csharp run id=csharp-empty-pop
using System;
using System.Collections.Generic;

var stack = new Stack<char>();
try
{
    stack.Pop();
}
catch (InvalidOperationException)
{
    Console.WriteLine("Pop: InvalidOperationException");
}
Console.WriteLine(stack.TryPop(out char top) ? "top: " + top : "TryPop: empty");
```

```output for=csharp-empty-pop
Pop: InvalidOperationException
TryPop: empty
```

- **Go `s[len(s)-1]` on an empty slice panics.** For a slice, "if x is out of range at run time, a run-time panic occurs"[^4]. Test `len(s)` first:

```go run id=go-empty-top
package main

import "fmt"

func top(stack []rune) (r rune, ok bool) {
	defer func() {
		if recover() != nil {
			ok = false
		}
	}()
	return stack[len(stack)-1], true
}

func main() {
	_, ok := top(nil)
	if !ok {
		fmt.Println("top of an empty slice: panic")
	}
	r, _ := top([]rune{'(', '['})
	fmt.Println("top:", string(r))
}
```

```output for=go-empty-top
top of an empty slice: panic
top: [
```

- **Bash `unset stack[-1]` without quotes can match a file.** An argument such as `a[4]` goes through filename expansion, and the manual says: "Quote the argument if pathname expansion is not desired"[^9]. In a directory with a file named `stack1`, the word `stack[-1]` becomes `stack1`, and `unset` removes a variable with that name. The stack does not change:

```bash run id=bash-unset-glob
dir=$(mktemp -d)
cd "$dir" || exit 1
touch stack1
stack=(a b c)
unset stack[-1]
echo "${stack[@]}"
unset 'stack[-1]'
echo "${stack[@]}"
cd / && rm -r -- "${dir:?}"
```

```output for=bash-unset-glob
a b c
a b
```

## Related tasks

- [[use-a-queue]]: the first item in is the first item out.
- [[use-a-priority-queue]]: the item with the best priority is the next item out.
- [[stack-data-structure]]: the idea behind this task and the operations.
- [[deque]]: one type that is a stack and a queue.

## References
