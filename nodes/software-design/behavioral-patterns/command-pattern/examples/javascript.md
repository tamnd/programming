```javascript run id=editor
class History {
  #undo = [];
  #redo = [];

  run(command) {
    command.execute();
    this.#undo.push(command);
    this.#redo.length = 0;
  }

  undo() {
    const command = this.#undo.pop();
    if (!command) return false;
    command.undo();
    this.#redo.push(command);
    return true;
  }

  redo() {
    const command = this.#redo.pop();
    if (!command) return false;
    command.execute();
    this.#undo.push(command);
    return true;
  }
}

const buffer = { text: "" };

const append = (text) => ({
  execute: () => {
    buffer.text += text;
  },
  undo: () => {
    buffer.text = buffer.text.slice(0, buffer.text.length - text.length);
  },
});

const remove = (count) => {
  let removed = "";
  return {
    execute: () => {
      removed = buffer.text.slice(buffer.text.length - count);
      buffer.text = buffer.text.slice(0, buffer.text.length - count);
    },
    undo: () => {
      buffer.text += removed;
    },
  };
};

const history = new History();
const show = (label) => console.log(`${label}: "${buffer.text}"`);
history.run(append("hello")); show("append");
history.run(append(" world")); show("append");
history.run(remove(5)); show("delete");
history.undo(); show("undo");
history.undo(); show("undo");
history.redo(); show("redo");
history.run(append("!")); show("append");
if (!history.redo()) console.log("redo: nothing to redo");
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
```

- Verdict: idiom. A command is an object literal with two functions, `execute` and `undo`. The two functions are closures over the same variables, so `undo` sees what `execute` saved.
- `remove` keeps the removed text in a variable of the factory call. Each command has its own variable, because each call of `remove` makes a fresh scope.
- `delete` is a reserved word in JavaScript, so the factory is named `remove`.
- Private fields (`#undo`, `#redo`) keep the stacks inside `History`, so other code cannot push a command without running it.
