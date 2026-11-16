```javascript run id=get-and-set-syntax
class Employee {
  #email; // a private field: code outside the class cannot read it

  constructor(name, email) {
    this.name = name; // a plain public field
    this.email = email; // calls the setter below
  }

  get email() {
    return this.#email;
  }

  set email(value) {
    const clean = value.trim().toLowerCase();
    if (!clean.includes("@")) {
      throw new RangeError(`not an email address: ${clean}`);
    }
    this.#email = clean;
  }
}

const e = new Employee("Ana", "ana@example.com");
e.email = "  Ana@Example.COM ";
console.log(e.name, e.email);
try {
  e.email = "not an address";
} catch (err) {
  console.log(err.message);
}
console.log(e.email, Object.keys(e));
```

```output for=get-and-set-syntax
Ana ana@example.com
not an email address: not an address
ana@example.com [ 'name' ]
```

The `get` and `set` syntax makes `email` look like a field to the caller. The value is in the private field `#email`, so code outside the class cannot skip the check. `Object.keys(e)` lists only `name`: the accessor property is on the prototype, not on the instance.
