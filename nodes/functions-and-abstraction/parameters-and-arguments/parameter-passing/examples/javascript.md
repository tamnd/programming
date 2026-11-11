```javascript run id=sharing
function update(car, year) {
  car.make = "Toyota";
  car = { make: "Honda" };
  year += 1;
  return [car.make, year];
}

const myCar = { make: "Ford" };
const myYear = 1999;
console.log(update(myCar, myYear));
console.log(myCar.make, myYear);

function collect(item, list = []) {
  list.push(item);
  return list;
}

console.log(collect(1), collect(2));

function connect({ host, port = 5432 } = {}, ...flags) {
  return `${host}:${port} ${flags.join(",")}`;
}

console.log(connect({ host: "db" }, "ssl", "ro"));
```

```output for=sharing
[ 'Honda', 2000 ]
Toyota 1999
[ 1 ] [ 2 ]
db:5432 ssl,ro
```

- Verdict: native. JavaScript passes each argument by value. When the value is an object, the parameter refers to the same object, so a change to a property is visible to the caller. A new assignment to the parameter is not.
- A default value is evaluated at each call in which the argument is missing or `undefined`. So each call to `collect` gets a new array. Python does the opposite.
- There are no named arguments. A destructured object parameter is the usual replacement. A rest parameter `...flags` collects the remaining arguments in an array.

```javascript wrong id=extra-arguments
console.log(["10", "10", "10"].map(parseInt));
```

```output for=extra-arguments
[ 10, NaN, 2 ]
```

`map` calls its callback with three arguments: the element, the index and the array. `parseInt` takes a second parameter, the radix. So the calls are `parseInt("10", 0)`, `parseInt("10", 1)` and `parseInt("10", 2)`. JavaScript does not check the number of arguments. Use `.map((s) => parseInt(s, 10))`.
