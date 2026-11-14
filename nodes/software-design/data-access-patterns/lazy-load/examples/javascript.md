```javascript run id=four-kinds-of-lazy-load
// A stand-in for the database, because Node has no SQL driver in its standard library.
const customerTable = new Map([[1, ["Ann", "Oslo"]], [2, ["Bo", "Rome"]]]);
const orderTable = [[10, 1], [11, 2], [12, 1]];
let selects = 0;

function selectCustomer(id) {
  selects++;
  return customerTable.get(id);
}

class Customer {
  constructor(id, name, city) {
    Object.assign(this, { id, name, city });
  }
}

// 1. Lazy initialization: a marker value (null) means "not loaded".
class Order {
  #customer = null;
  constructor(id, customerId) {
    Object.assign(this, { id, customerId });
  }
  get customer() {
    this.#customer ??= new Customer(this.customerId, ...selectCustomer(this.customerId));
    return this.#customer;
  }
}

// 2. Virtual proxy: a Proxy object that loads the real customer on first use.
function customerProxy(id) {
  let real = null;
  return new Proxy({ id }, {
    get(target, prop) {
      if (prop === "id") return target.id;
      real ??= new Customer(id, ...selectCustomer(id));
      return real[prop];
    },
  });
}

// 3. Value holder: the client asks the holder for the value.
class ValueHolder {
  #loader;
  #loaded = false;
  #value;
  constructor(loader) {
    this.#loader = loader;
  }
  getValue() {
    if (!this.#loaded) {
      this.#value = this.#loader();
      this.#loaded = true;
    }
    return this.#value;
  }
}

// 4. Ghost: the real class, with only the key filled in until first use.
class GhostCustomer {
  #name;
  #city;
  #loaded = false;
  constructor(id) {
    this.id = id;
  }
  #load() {
    if (!this.#loaded) {
      [this.#name, this.#city] = selectCustomer(this.id);
      this.#loaded = true;
    }
  }
  get name() {
    this.#load();
    return this.#name;
  }
  get city() {
    this.#load();
    return this.#city;
  }
}

selects = 0;
const order = new Order(10, 1);
let line = `lazy initialization: selects ${selects} | ${order.customer.name}`;
console.log(`${line} | selects ${selects} | ${order.customer.name} | selects ${selects}`);

selects = 0;
const proxy = customerProxy(2);
line = `virtual proxy: id ${proxy.id} | selects ${selects} | ${proxy.name}`;
console.log(`${line} | selects ${selects} | ${proxy.city} | selects ${selects}`);

selects = 0;
const holder = new ValueHolder(() => new Customer(1, ...selectCustomer(1)));
console.log(`value holder: selects ${selects} | ${holder.getValue().name} | selects ${selects}`);

selects = 0;
const ghost = new GhostCustomer(1);
console.log(`ghost: id ${ghost.id} | selects ${selects} | ${ghost.name} ${ghost.city} | selects ${selects}`);

selects = 1; // One SELECT reads the orders.
const orders = orderTable.map(([id, customerId]) => new Order(id, customerId));
console.log(`lazy: ${orders.map((o) => o.customer.name).join(", ")} | selects ${selects}`);

selects = 1; // One SELECT with a JOIN reads the orders and their customers.
const names = orderTable.map(([, customerId]) => customerTable.get(customerId)[0]);
console.log(`eager: ${names.join(", ")} | selects ${selects}`);
```

```output
lazy initialization: selects 0 | Ann | selects 1 | Ann | selects 1
virtual proxy: id 2 | selects 0 | Bo | selects 1 | Rome | selects 1
value holder: selects 0 | Ann | selects 1
ghost: id 1 | selects 0 | Ann Oslo | selects 1
lazy: Ann, Bo, Ann | selects 4
eager: Ann, Bo, Ann | selects 1
```

- Verdict: idiom. Lazy initialization is a getter over a private field with `??=`. The virtual proxy is a built-in `Proxy` with a `get` trap, so the caller cannot tell it from a `Customer`.
- Node has no SQL driver in its standard library, so a `Map` stands in for the customer table and `selectCustomer` counts the statements.
