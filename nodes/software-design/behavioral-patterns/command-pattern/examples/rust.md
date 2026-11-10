```rust run id=editor
// Commands are data. Undo needs the text that a delete removed, so Delete has a slot for it.
#[derive(Debug)]
enum Command {
    Append(String),
    Delete { count: usize, removed: String },
}

impl Command {
    fn execute(&mut self, text: &mut String) {
        match self {
            Command::Append(s) => text.push_str(s),
            Command::Delete { count, removed } => {
                let start = text.len() - *count;
                *removed = text.split_off(start);
            }
        }
    }

    fn undo(&self, text: &mut String) {
        match self {
            Command::Append(s) => text.truncate(text.len() - s.len()),
            Command::Delete { removed, .. } => text.push_str(removed),
        }
    }
}

#[derive(Default)]
struct Editor {
    text: String,
    undo: Vec<Command>,
    redo: Vec<Command>,
}

impl Editor {
    fn run(&mut self, mut command: Command) {
        command.execute(&mut self.text);
        self.undo.push(command);
        self.redo.clear();
    }

    fn undo(&mut self) -> bool {
        let Some(command) = self.undo.pop() else { return false };
        command.undo(&mut self.text);
        self.redo.push(command);
        true
    }

    fn redo(&mut self) -> bool {
        let Some(mut command) = self.redo.pop() else { return false };
        command.execute(&mut self.text);
        self.undo.push(command);
        true
    }

    fn show(&self, label: &str) {
        println!("{label}: \"{}\"", self.text);
    }
}

fn delete(count: usize) -> Command {
    Command::Delete { count, removed: String::new() }
}

fn main() {
    let mut editor = Editor::default();
    editor.run(Command::Append("hello".into()));
    editor.show("append");
    editor.run(Command::Append(" world".into()));
    editor.show("append");
    editor.run(delete(5));
    editor.show("delete");
    editor.undo();
    editor.show("undo");
    editor.undo();
    editor.show("undo");
    editor.redo();
    editor.show("redo");
    editor.run(Command::Append("!".into()));
    editor.show("append");
    if !editor.redo() {
        println!("redo: nothing to redo");
    }
    println!("{:?}", editor.undo.last());
}
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
Some(Append("!"))
```

- Verdict: idiom. A command is a value of an enum. It holds no reference to the buffer. The editor passes `&mut String` to `execute` and `undo`, so the borrow checker sees one owner of the text.
- A closure that captured `&mut String` would hold a mutable borrow for as long as the command lives, and the history keeps commands for a long time. Commands as data avoid that problem.
- An enum can be printed, compared, stored and sent to another thread with derives. The last line prints the command on top of the undo stack.
- `std::process::Command` in the standard library is not this pattern. It is a builder for a child process.
