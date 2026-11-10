```bash run id=checkout
# Each line of the cart: name price-in-cents quantity.
cart=("book 1500 2" "pen 200 5")

regular() {
    local total=0 name price quantity
    for line in "${cart[@]}"; do
        read -r name price quantity <<<"$line"
        total=$((total + price * quantity))
    done
    echo "$total"
}

ten_percent_off() {
    echo $(($(regular) * 90 / 100))
}

# 50 cents off each unit of a line with 5 or more units.
bulk() {
    local total=0 name price quantity
    for line in "${cart[@]}"; do
        read -r name price quantity <<<"$line"
        if ((quantity >= 5)); then
            price=$((price - 50))
        fi
        total=$((total + price * quantity))
    done
    echo "$total"
}

# The context holds the name of the strategy function.
pricing=regular
checkout_total() {
    "$pricing"
}

for strategy in regular ten_percent_off bulk; do
    pricing=$strategy
    echo "${strategy//_/ }: $(checkout_total) cents"
done
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. Bash functions are not values, but a variable can hold the name of a function, and `"$pricing"` calls it.
- A function returns its result on standard output, so `$(checkout_total)` runs it in a subshell.
- The cart is a global array, because Bash cannot pass an array to a function by value. A name reference (`local -n`) is the other choice.
- The name in the variable is not checked before the call. An unknown name fails with "command not found" at run time.
