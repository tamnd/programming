```javascript run id=closures-and-generators
function makeCounter() {
  let count = 0; // lives on after makeCounter returns, in the closure
  return () => ++count;
}

const a = makeCounter();
const b = makeCounter();
console.log(a(), a(), a(), "b:", b());

function* trafficLight() { // the position in the code is part of the state
  while (true) {
    yield "green";
    yield "yellow";
    yield "red";
  }
}
const light = trafficLight();
console.log(Array.from({ length: 5 }, () => light.next().value).join(" -> "));
```

```output for=closures-and-generators
1 2 3 b: 1
green -> yellow -> red -> green -> yellow
```

- Verdict: native. A closure keeps the variables of its enclosing call alive, so each counter has its own `count` that no other code can reach. A generator keeps its position in the code between calls: the traffic light has no variable for the colour, because the `yield` where it stopped is the state. Private class fields, written `#count`, give the same protection to object state.
