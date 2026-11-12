```rust run id=orders
use std::collections::BTreeMap;

#[derive(Clone, Debug, PartialEq)]
enum Status {
    Open,
    Shipped,
}

#[derive(Clone, Debug)]
struct Order {
    id: u32,
    customer: String,
    total: u32,
    status: Status,
}

impl Order {
    fn new(id: u32, customer: &str, total: u32) -> Self {
        Order { id, customer: customer.into(), total, status: Status::Open }
    }

    fn ship(&mut self) -> Result<(), String> {
        if self.status != Status::Open {
            return Err(format!("order {} is {:?}", self.id, self.status));
        }
        self.status = Status::Shipped;
        Ok(())
    }
}

trait OrderRepository {
    fn add(&mut self, order: Order) -> Result<(), String>;
    fn get(&self, id: u32) -> Option<Order>;
    fn by_customer(&self, customer: &str) -> Vec<Order>;
    fn update(&mut self, order: Order) -> Result<(), String>;
}

#[derive(Default)]
struct InMemoryOrders {
    rows: BTreeMap<u32, Order>,
}

impl OrderRepository for InMemoryOrders {
    fn add(&mut self, order: Order) -> Result<(), String> {
        if self.rows.contains_key(&order.id) {
            return Err(format!("order {} exists", order.id));
        }
        self.rows.insert(order.id, order);
        Ok(())
    }

    fn get(&self, id: u32) -> Option<Order> {
        self.rows.get(&id).cloned()
    }

    fn by_customer(&self, customer: &str) -> Vec<Order> {
        self.rows.values().filter(|o| o.customer == customer).cloned().collect()
    }

    fn update(&mut self, order: Order) -> Result<(), String> {
        match self.rows.get_mut(&order.id) {
            Some(row) => {
                *row = order;
                Ok(())
            }
            None => Err(format!("order {} does not exist", order.id)),
        }
    }
}

fn ship_order(orders: &mut impl OrderRepository, id: u32) -> String {
    let Some(mut order) = orders.get(id) else {
        return format!("order {id}: not found");
    };
    if let Err(e) = order.ship().and_then(|()| orders.update(order.clone())) {
        return e;
    }
    format!("order {id} is {}", format!("{:?}", order.status).to_lowercase())
}

fn main() {
    let mut repo = InMemoryOrders::default();
    for o in [Order::new(1, "ana", 30), Order::new(2, "bo", 12), Order::new(3, "ana", 5)] {
        repo.add(o).unwrap();
    }
    let ana: Vec<String> = repo
        .by_customer("ana")
        .iter()
        .map(|o| format!("order {} ({})", o.id, o.total))
        .collect();
    println!("ana: {}", ana.join(", "));
    println!("{}", ship_order(&mut repo, 1));
    println!("{}", ship_order(&mut repo, 9));
    let stored = repo.get(1).unwrap();
    println!("stored: order {} is {}", stored.id, format!("{:?}", stored.status).to_lowercase());
}
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. A trait gives the collection-like contract. `get` returns `Option<Order>`, so the compiler makes the caller handle a missing order.
- `get` returns an owned clone. The repository keeps the only stored copy, and the borrow rules make the caller give a changed order back through `update`, which takes `&mut self`.
- `BTreeMap` keeps the keys in order, so `by_customer` returns the orders sorted by id with no extra sort.
