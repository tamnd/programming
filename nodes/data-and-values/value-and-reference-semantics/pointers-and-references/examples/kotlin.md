```kotlin run timeout=30 id=change
class Box(var value: Int)

fun change(box: Box) {
    box.value = 42
}

fun main() {
    val box = Box(1)
    change(box)
    println("after change: ${box.value}")

    val alias = box
    alias.value = 100
    println("box.value: ${box.value}")
    println("alias === box: ${alias === box}")

    val numbers = mutableListOf(1, 2, 3)
    val copy = numbers.toMutableList()
    copy[0] = 100
    println("numbers: $numbers")
    println("copy === numbers: ${copy === numbers}")
    println("copy == listOf(100, 2, 3): ${copy == listOf(100, 2, 3)}")
}
```

```output for=change
after change: 42
box.value: 100
alias === box: true
numbers: [1, 2, 3]
copy === numbers: false
copy == listOf(100, 2, 3): true
```

- Verdict: native. A variable of a class type holds a reference to an object.
- A call passes the reference. `change` sets a property of the shared object, and the caller sees it.
- `===` is referential equality: both sides point to the same object. `==` is structural equality and calls `equals`.
- `val` stops a second assignment to the variable. It does not make the object immutable, so `alias.value = 100` is valid.
- Kotlin has no pointer types and no call by reference.
