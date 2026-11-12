```rust run id=todo
use std::cell::RefCell;
use std::rc::Rc;

#[derive(Clone)]
struct Item {
    title: String,
    done: bool,
}

#[derive(Default)]
struct TodoModel {
    items: Vec<Item>,
    observers: Vec<Box<dyn Fn(&[Item])>>,
}

impl TodoModel {
    fn subscribe(&mut self, f: impl Fn(&[Item]) + 'static) {
        self.observers.push(Box::new(f));
    }

    fn add(&mut self, title: &str) -> Result<(), String> {
        if title.is_empty() {
            return Err("a title is required".into());
        }
        self.items.push(Item { title: title.into(), done: false });
        self.changed();
        Ok(())
    }

    fn complete(&mut self, n: usize) -> Result<(), String> {
        let item = n.checked_sub(1).and_then(|i| self.items.get_mut(i)).ok_or(format!("no item {n}"))?;
        item.done = true;
        self.changed();
        Ok(())
    }

    // The model passes a read-only view of its items to each observer.
    fn changed(&self) {
        for f in &self.observers {
            f(&self.items);
        }
    }
}

fn list_view(items: &[Item]) {
    for (i, item) in items.iter().enumerate() {
        println!("  [{}] {} {}", if item.done { "x" } else { " " }, i + 1, item.title);
    }
}

fn summary_view(items: &[Item]) {
    let done = items.iter().filter(|item| item.done).count();
    println!("  summary: {} open, {done} done", items.len() - done);
}

struct Controller {
    model: Rc<RefCell<TodoModel>>,
}

impl Controller {
    fn handle(&self, line: &str) {
        println!("> {line}");
        let (command, argument) = line.split_once(' ').unwrap_or((line, ""));
        let mut model = self.model.borrow_mut();
        let result = match (command, argument.parse::<usize>()) {
            ("add", _) => model.add(argument),
            ("done", Ok(n)) => model.complete(n),
            _ => Err(format!("cannot read '{line}'")),
        };
        if let Err(e) = result {
            println!("  error: {e}");
        }
    }
}

fn main() {
    let model = Rc::new(RefCell::new(TodoModel::default()));
    model.borrow_mut().subscribe(list_view);
    model.borrow_mut().subscribe(summary_view);
    let controller = Controller { model: Rc::clone(&model) };
    for line in ["add buy milk", "add write report", "done 1", "done 7", "rename 2"] {
        controller.handle(line);
    }
}
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. The model pushes a shared slice `&[Item]` to each observer, instead of each view holding a reference back to the model. A view that held `Rc<RefCell<TodoModel>>` would try to borrow the model while the controller holds it mutably, and `RefCell` would panic at run time.
- The views are plain functions, so `subscribe(list_view)` needs no closure.
- The controller shares the model through `Rc<RefCell<...>>` because `main` and the controller both refer to it.
