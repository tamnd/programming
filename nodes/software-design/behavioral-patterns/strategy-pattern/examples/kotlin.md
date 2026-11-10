```kotlin run timeout=30 id=checkout
data class Item(val name: String, val price: Int, val quantity: Int)

typealias Pricing = (List<Item>) -> Int

class Checkout(var pricing: Pricing) {
    fun total(items: List<Item>): Int = pricing(items)
}

fun regular(items: List<Item>): Int = items.sumOf { it.price * it.quantity }

fun tenPercentOff(items: List<Item>): Int = regular(items) * 90 / 100

// 50 cents off each unit of a line with 5 or more units.
fun bulk(items: List<Item>): Int =
    items.sumOf { (if (it.quantity >= 5) it.price - 50 else it.price) * it.quantity }

fun main() {
    val cart = listOf(Item("book", 1500, 2), Item("pen", 200, 5))
    val strategies: List<Pair<String, Pricing>> = listOf(
        "regular" to ::regular,
        "ten percent off" to ::tenPercentOff,
        "bulk" to ::bulk,
    )
    val checkout = Checkout(::regular)
    for ((name, pricing) in strategies) {
        checkout.pricing = pricing
        println("$name: ${checkout.total(cart)} cents")
    }
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. Kotlin has function types, so the strategy interface is `(List<Item>) -> Int` and `::bulk` is a reference to a top-level function of that type.
- `typealias` only names the function type. It does not make a type of its own.
- A `fun interface` gives the class form a single abstract method, so a lambda can still be passed where it is expected.
