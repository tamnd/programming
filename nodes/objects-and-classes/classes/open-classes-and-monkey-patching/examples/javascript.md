```javascript run id=patch-a-prototype-method
class Greeter {
  greet(name) {
    return `Hello, ${name}`;
  }
}

// Replace a method, keep the original, and return a function that undoes the patch.
function patchMethod(target, name, makeReplacement) {
  const original = target[name];
  if (typeof original !== "function") throw new Error(`no method ${name} to patch`);
  target[name] = makeReplacement(original);
  return () => {
    target[name] = original;
  };
}

const g = new Greeter(); // made before the patch
const undo = patchMethod(Greeter.prototype, "greet", (original) =>
  function (name) {
    return original.call(this, name.trim()) + "!";
  });
console.log(g.greet("  Ada "));
undo();
console.log(g.greet("Ada"));
console.log(Object.keys(Greeter.prototype));
```

```output for=patch-a-prototype-method
Hello, Ada!
Hello, Ada
[]
```

In JavaScript, every class keeps its methods on its prototype object, so an assignment to `Greeter.prototype.greet` changes the method for every instance, also for `g`, which existed before the patch. `patchMethod` keeps the original, so the replacement can call it and the returned function can undo the patch. The check for an existing function stops the patch from adding a misspelled name in silence. The assignment replaces an existing property, so the method keeps the attributes of a class method and `Object.keys` does not list it.
