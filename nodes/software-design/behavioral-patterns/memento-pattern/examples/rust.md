```rust run id=undo
mod editor {
    /// The memento. Its fields are private to this module, so code outside
    /// the module can hold a Memento but cannot read or build one.
    pub struct Memento {
        text: String,
        cursor: usize,
    }

    /// The originator.
    #[derive(Default)]
    pub struct Editor {
        text: String,
        cursor: usize,
    }

    impl Editor {
        pub fn type_text(&mut self, chars: &str) {
            self.text.insert_str(self.cursor, chars);
            self.cursor += chars.len();
        }

        pub fn move_to(&mut self, position: usize) {
            self.cursor = position.min(self.text.len());
        }

        pub fn save(&self) -> Memento {
            Memento { text: self.text.clone(), cursor: self.cursor }
        }

        /// Takes the memento by value: a restored memento is used up.
        pub fn restore(&mut self, memento: Memento) {
            self.text = memento.text;
            self.cursor = memento.cursor;
        }

        pub fn show(&self) -> String {
            format!("{}|{}", &self.text[..self.cursor], &self.text[self.cursor..])
        }
    }
}

use editor::{Editor, Memento};

/// The caretaker. It keeps mementos and cannot look inside them.
struct History {
    undo: Vec<Memento>,
}

impl History {
    fn save(&mut self, editor: &Editor) {
        self.undo.push(editor.save());
    }

    fn undo(&mut self, editor: &mut Editor) -> bool {
        match self.undo.pop() {
            Some(memento) => {
                editor.restore(memento);
                true
            }
            None => false,
        }
    }
}

fn main() {
    let mut editor = Editor::default();
    let mut history = History { undo: Vec::new() };

    history.save(&editor);
    editor.type_text("Hello");
    println!("{}", editor.show());
    history.save(&editor);
    editor.type_text(" world");
    println!("{}", editor.show());
    history.save(&editor);
    editor.move_to(5);
    editor.type_text(",");
    println!("{}", editor.show());

    for _ in 0..4 {
        if history.undo(&mut editor) {
            println!("undo: {}", editor.show());
        } else {
            println!("nothing to undo");
        }
    }
}
```

```output for=undo
Hello|
Hello world|
Hello,| world
undo: Hello world|
undo: Hello|
undo: |
nothing to undo
```

- Verdict: idiom. Module privacy gives the narrow interface. A private field is visible in its module and the modules below it, so `History` in the parent module cannot read `memento.text`, and the compiler rejects any attempt.
- `save` must clone the `String`, because the editor keeps its own. `restore` takes the memento by value and moves the string back in, with no second copy.
- The history passes `&Editor` and `&mut Editor` in each call and holds no reference to the editor. A caretaker that held `&mut Editor` would lock the editor for as long as the caretaker lives.
- `insert_str` and the slices index by bytes, and they panic at a position that is not on a character boundary. The text here is ASCII.
