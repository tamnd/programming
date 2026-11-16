```javascript run id=function-takes-a-class
const registry = new Map();

function plugin(cls) { // a function that takes a class does the work of a metaclass
  if (typeof cls.prototype.run !== "function") {
    throw new TypeError(`${cls.name} must define run()`);
  }
  registry.set(cls.name.toLowerCase(), cls);
  return cls;
}

const Upper = plugin(class Upper {
  run(text) {
    return text.toUpperCase();
  }
});

class Even {
  static [Symbol.hasInstance](n) { // changes what instanceof does for this class
    return n % 2 === 0;
  }
}

console.log(typeof Upper, Object.getPrototypeOf(Upper) === Function.prototype);
console.log([...registry.keys()], new (registry.get("upper"))().run("abc"));
console.log(4 instanceof Even, 5 instanceof Even);
try {
  plugin(class Broken {});
} catch (e) {
  console.log(e.message);
}
```

```output for=function-takes-a-class
function true
[ 'upper' ] ABC
true false
Broken must define run()
```

- Verdict: absent. A class is a function object, and its prototype is `Function.prototype`; code cannot give a class its own metaclass, and no hook runs when a class is defined.
- `plugin` takes a class, checks it and registers it, which is the usual job of a Python metaclass. The call must be written at each class. `static [Symbol.hasInstance]` is one of the few hooks that change how the language treats a class: here `instanceof Even` tests a number.
