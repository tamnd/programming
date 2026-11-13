```typescript run id=transaction-script
// A thin wrapper over the stored tables. The scripts call it directly.
interface Customer {
  name: string;
  credit: number;
}

interface Order {
  customer: number;
  cents: number;
  state: "open" | "cancelled";
}

const db = {
  customers: new Map<number, Customer>([[1, { name: "ada", credit: 5000 }]]),
  orders: new Map<number, Order>(),
};

function findCustomer(customerId: number): Customer {
  const customer = db.customers.get(customerId);
  if (!customer) throw new Error(`no customer ${customerId}`);
  return customer;
}

function insertOrder(customerId: number, cents: number): number {
  const orderId = db.orders.size + 1;
  db.orders.set(orderId, { customer: customerId, cents, state: "open" });
  return orderId;
}

function money(cents: number): string {
  return `${Math.floor(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
}

// One transaction script for each request from the presentation.
function placeOrder(customerId: number, cents: number): string {
  const customer = findCustomer(customerId);
  if (customer.credit < cents) return `refused, credit left ${money(customer.credit)}`;
  customer.credit -= cents;
  return `order ${insertOrder(customerId, cents)}`;
}

function cancelOrder(orderId: number): string {
  const order = db.orders.get(orderId);
  if (!order) throw new Error(`no order ${orderId}`);
  if (order.state !== "open") return "already cancelled";
  order.state = "cancelled";
  findCustomer(order.customer).credit += order.cents;
  return `refunded ${money(order.cents)}`;
}

console.log("place_order(ada, 30.00):", placeOrder(1, 3000));
console.log("place_order(ada, 30.00):", placeOrder(1, 3000));
console.log("cancel_order(1):", cancelOrder(1));
console.log("cancel_order(1):", cancelOrder(1));
console.log("place_order(ada, 30.00):", placeOrder(1, 3000));
console.log("credit of ada:", money(findCustomer(1).credit));
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. The same functions as JavaScript, with interfaces for the rows and a union type for the state of an order.
