```bash run id=sensor
observers=()

subscribe() { observers+=("$1"); }

unsubscribe() {
    local kept=() name
    for name in "${observers[@]}"; do
        [[ $name == "$1" ]] || kept+=("$name")
    done
    observers=("${kept[@]}")
}

set_reading() {
    local observer
    for observer in "${observers[@]}"; do
        "$observer" "$1"
    done
}

display() { echo "display: $1"; }

alarm() {
    if (( $(echo "$1 > 30" | bc) )); then
        echo "alarm: $1 is above 30"
    fi
}

subscribe display
subscribe alarm
set_reading 21.5
set_reading 30.5
unsubscribe display
set_reading 31.0
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: idiom. Bash functions are not values, but a function can be called by its name. An array of function names is the observer list.
- Bash arithmetic has integers only, so the alarm compares the decimal reading with `bc`.
- The expansion `"${observers[@]}"` in `set_reading` is made before the loop starts, so an observer that unsubscribes during the loop does not change the current pass.
