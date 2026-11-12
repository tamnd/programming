```rust run id=processor
trait Command {
    fn name(&self) -> String;
    fn execute(&self, words: &mut Vec<String>);
    fn undo(&self, words: &mut Vec<String>);
}

struct Insert(String);

impl Command for Insert {
    fn name(&self) -> String {
        format!("insert {}", self.0)
    }
    fn execute(&self, words: &mut Vec<String>) {
        words.push(self.0.clone());
    }
    fn undo(&self, words: &mut Vec<String>) {
        words.pop();
    }
}

struct Processor {
    words: Vec<String>, // the supplier
    done: Vec<Box<dyn Command>>,
}

impl Processor {
    fn do_it(&mut self, command: Box<dyn Command>) {
        println!("do: {}", command.name());
        command.execute(&mut self.words);
        self.done.push(command);
    }
    fn undo_it(&mut self) {
        if let Some(command) = self.done.pop() {
            println!("undo: {}", command.name());
            command.undo(&mut self.words);
        }
    }
}

fn main() {
    let mut p = Processor { words: Vec::new(), done: Vec::new() };
    p.do_it(Box::new(Insert("hello".to_string())));
    p.do_it(Box::new(Insert("world".to_string())));
    println!("text: {}", p.words.join(" "));
    p.undo_it();
    println!("text: {}", p.words.join(" "));
}
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. `Command` is a trait, and the processor keeps `Box<dyn Command>` values. The commands get the supplier as a `&mut` parameter, so no command holds a mutable reference to it.
