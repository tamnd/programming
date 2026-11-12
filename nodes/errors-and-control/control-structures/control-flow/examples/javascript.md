```javascript run id=switch-finally-and-event-order
// A switch case without break falls through into the next case.
function days(month) {
  switch (month) {
    case "feb":
      return 28;
    case "apr":
    case "jun":
      return 30;
    default:
      return 31;
  }
}
console.log(days("feb"), days("jun"), days("jan"));

// finally runs after the return value is computed and before the function returns.
function f() {
  try {
    return "try";
  } finally {
    console.log("finally runs first");
  }
}
console.log(f());

// Asynchronous code runs later: synchronous code first, then promise callbacks, then timers.
const order = [];
setTimeout(() => {
  order.push("timer");
  console.log(order.join(" "));
}, 0);
Promise.resolve().then(() => order.push("promise"));
order.push("sync");

// && and ?. also choose whether code runs.
const user = null;
console.log(user?.name ?? "no user", user && user.name);
```

```output for=switch-finally-and-event-order
28 30 31
finally runs first
try
no user null
sync promise timer
```

- Verdict: native. The cases `"apr"` and `"jun"` share one body, because a case without `break` or `return` falls through. `finally` runs before the function returns. Synchronous code runs first, then the promise callback, then the timer, so the order is `sync promise timer`. `?.`, `??` and `&&` decide whether the code to their right runs.
