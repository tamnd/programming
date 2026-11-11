```javascript run id=construction-order
class Base {
  name = "base field";  // a field initializer runs as part of construction

  constructor(id) {
    console.log("Base constructor", id);
    this.id = id;
  }
}

class Item extends Base {
  price = 0;  // initialized after super() returns

  constructor(id, price) {
    super(id);  // must come before any use of this
    console.log("Item constructor, price field is", this.price);
    this.price = price;
  }

  static free(id) { return new Item(id, 0); }  // a named constructor
}

const i = new Item(7, 250);
console.log(i);
console.log(Item.free(8).price);
```

```output for=construction-order
Base constructor 7
Item constructor, price field is 0
Item { name: 'base field', id: 7, price: 250 }
Base constructor 8
Item constructor, price field is 0
0
```

```javascript wrong id=virtual-call-in-constructor
class Widget {
  constructor() {
    this.render();  // calls the subclass method before the subclass is ready
  }

  render() { console.log("widget"); }
}

class Label extends Widget {
  text = "hello";  // set only after Widget's constructor returns

  render() { console.log("label:", this.text); }
}

new Label();
```

```output for=virtual-call-in-constructor
label: undefined
```

- Verdict: native. `new` creates an object whose prototype is the class's `prototype`, and runs the `constructor`. In a derived class, the constructor must call `super(...)` before it uses `this`, and the field initializers of the derived class run right after `super` returns, before the rest of the constructor body. That is why `Item`'s constructor sees `price` as 0, the value of the initializer, before it sets the real price. A static method is the usual named constructor.
- The wrong block calls an overridden method from the base constructor. The call reaches `Label.render`, but `Label`'s field `text` gets its value only after `Widget`'s constructor returns, so the method sees `undefined`. Keep constructors free of calls to methods that subclasses can override, or start the work in a separate method after construction.
