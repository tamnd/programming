```rust run id=interpreter
use std::cell::RefCell;
use std::collections::HashMap;
use std::rc::Rc;

enum Expr {
    Num(i64),
    Var(&'static str),
    Bin(char, Box<Expr>, Box<Expr>),
}

enum Stmt {
    Let(&'static str, Expr),
    Assign(&'static str, Expr),
    Block(Vec<Stmt>),
    While(Expr, Box<Stmt>),
    Print(Expr),
}

struct Env {
    vars: RefCell<HashMap<&'static str, i64>>,
    parent: Option<Rc<Env>>,
}

impl Env {
    fn new(parent: Option<Rc<Env>>) -> Rc<Env> {
        Rc::new(Env { vars: RefCell::new(HashMap::new()), parent })
    }
}

#[derive(Default)]
struct Interpreter {
    nodes: u32,
    lookups: u32,
    links: u32,
}

impl Interpreter {
    fn find(&mut self, env: &Rc<Env>, name: &str) -> Rc<Env> {
        self.lookups += 1;
        let mut e = Rc::clone(env);
        loop {
            if e.vars.borrow().contains_key(name) {
                return e;
            }
            self.links += 1;
            e = match &e.parent {
                Some(p) => Rc::clone(p),
                None => panic!("undefined variable '{}'", name),
            };
        }
    }

    fn evaluate(&mut self, node: &Expr, env: &Rc<Env>) -> i64 {
        self.nodes += 1;
        match node {
            Expr::Num(value) => *value,
            Expr::Var(name) => {
                let found = self.find(env, name);
                let value = found.vars.borrow()[name];
                value
            }
            Expr::Bin(op, left, right) => {
                let a = self.evaluate(left, env);
                let b = self.evaluate(right, env);
                match op {
                    '+' => a + b,
                    '-' => a - b,
                    '*' => a * b,
                    '<' => (a < b) as i64,
                    _ => panic!("unknown operator {}", op),
                }
            }
        }
    }

    fn execute(&mut self, node: &Stmt, env: &Rc<Env>) {
        self.nodes += 1;
        match node {
            Stmt::Let(name, init) => {
                let value = self.evaluate(init, env);
                env.vars.borrow_mut().insert(*name, value);
            }
            Stmt::Assign(name, expr) => {
                let value = self.evaluate(expr, env);
                self.find(env, name).vars.borrow_mut().insert(*name, value);
            }
            Stmt::Block(body) => {
                let inner = Env::new(Some(Rc::clone(env)));
                for stmt in body {
                    self.execute(stmt, &inner);
                }
            }
            Stmt::While(cond, body) => {
                while self.evaluate(cond, env) != 0 {
                    self.execute(body, env);
                }
            }
            Stmt::Print(expr) => println!("{}", self.evaluate(expr, env)),
        }
    }
}

fn main() {
    use Expr::*;
    use Stmt::*;
    let program = Block(vec![
        Let("n", Num(5)),
        Let("acc", Num(1)),
        While(
            Bin('<', Box::new(Num(0)), Box::new(Var("n"))),
            Box::new(Block(vec![
                Let("step", Var("n")),
                Assign("acc", Bin('*', Box::new(Var("acc")), Box::new(Var("step")))),
                Assign("n", Bin('-', Box::new(Var("n")), Box::new(Num(1)))),
            ])),
        ),
        Print(Var("acc")),
    ]);

    let mut interp = Interpreter::default();
    interp.execute(&program, &Env::new(None));
    println!("nodes visited: {}", interp.nodes);
    println!("variable lookups: {}, parent links followed: {}", interp.lookups, interp.links);
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. Two enums model the tree, and `match` must cover every variant, so a new node type is a compile error until the interpreter handles it.
- An environment has several owners: the block that runs in it and every inner environment that points to it. `Rc` counts those owners and frees the environment when the last one goes. `RefCell` allows the map to change through a shared reference, and checks the borrows at run time.
- A cycle of `Rc` pointers is never freed. Closures that capture their own environment create such cycles, so an interpreter with closures needs a weak pointer or a tracing [[garbage-collection|garbage collector]].
