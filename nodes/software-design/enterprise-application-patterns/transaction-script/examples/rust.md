```rust run id=transaction-script
use std::collections::HashMap;

struct Customer {
    #[allow(dead_code)]
    name: String,
    credit: u32,
}

struct Order {
    customer: u32,
    cents: u32,
    open: bool,
}

// A thin wrapper over the stored tables. The scripts call it directly.
struct Db {
    customers: HashMap<u32, Customer>,
    orders: HashMap<u32, Order>,
}

impl Db {
    fn find_customer(&mut self, customer_id: u32) -> &mut Customer {
        self.customers.get_mut(&customer_id).expect("no such customer")
    }

    fn insert_order(&mut self, customer: u32, cents: u32) -> u32 {
        let order_id = self.orders.len() as u32 + 1;
        self.orders.insert(order_id, Order { customer, cents, open: true });
        order_id
    }
}

fn money(cents: u32) -> String {
    format!("{}.{:02}", cents / 100, cents % 100)
}

// One transaction script for each request from the presentation.
fn place_order(db: &mut Db, customer_id: u32, cents: u32) -> String {
    let customer = db.find_customer(customer_id);
    if customer.credit < cents {
        return format!("refused, credit left {}", money(customer.credit));
    }
    customer.credit -= cents;
    format!("order {}", db.insert_order(customer_id, cents))
}

fn cancel_order(db: &mut Db, order_id: u32) -> String {
    let order = db.orders.get_mut(&order_id).expect("no such order");
    if !order.open {
        return "already cancelled".to_string();
    }
    order.open = false;
    let (customer_id, cents) = (order.customer, order.cents);
    db.find_customer(customer_id).credit += cents;
    format!("refunded {}", money(cents))
}

fn main() {
    let mut db = Db {
        customers: HashMap::from([(1, Customer { name: "ada".to_string(), credit: 5000 })]),
        orders: HashMap::new(),
    };
    println!("place_order(ada, 30.00): {}", place_order(&mut db, 1, 3000));
    println!("place_order(ada, 30.00): {}", place_order(&mut db, 1, 3000));
    println!("cancel_order(1): {}", cancel_order(&mut db, 1));
    println!("cancel_order(1): {}", cancel_order(&mut db, 1));
    println!("place_order(ada, 30.00): {}", place_order(&mut db, 1, 3000));
    println!("credit of ada: {}", money(db.find_customer(1).credit));
}
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. Each script is a function that takes `&mut Db`. `cancel_order` copies the two fields that it needs out of the order before it borrows the customer, because the borrow checker allows only one mutable borrow of `db` at a time.
