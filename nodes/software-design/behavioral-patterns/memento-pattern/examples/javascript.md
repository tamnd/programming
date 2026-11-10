```javascript run id=undo
// The originator. Only it can read the state behind a memento.
class Editor {
  static #states = new WeakMap(); // memento -> saved state, visible only inside Editor
  #text = "";
  #cursor = 0;

  type(chars) {
    this.#text = this.#text.slice(0, this.#cursor) + chars + this.#text.slice(this.#cursor);
    this.#cursor += chars.length;
  }
  moveTo(cursor) {
    this.#cursor = Math.max(0, Math.min(cursor, this.#text.length));
  }
  save() {
    const memento = Object.freeze({}); // an opaque token for the caretaker
    Editor.#states.set(memento, { text: this.#text, cursor: this.#cursor });
    return memento;
  }
  restore(memento) {
    const state = Editor.#states.get(memento);
    if (state === undefined) throw new TypeError("not a memento of an Editor");
    this.#text = state.text;
    this.#cursor = state.cursor;
  }
  show() {
    return this.#text.slice(0, this.#cursor) + "|" + this.#text.slice(this.#cursor);
  }
}

// The caretaker. It keeps mementos and cannot look inside them.
class History {
  #undo = [];
  constructor(editor) {
    this.editor = editor;
  }
  save() {
    this.#undo.push(this.editor.save());
  }
  undo() {
    if (this.#undo.length === 0) return false;
    this.editor.restore(this.#undo.pop());
    return true;
  }
}

const editor = new Editor();
const history = new History(editor);

history.save();
editor.type("Hello");
console.log(editor.show());
history.save();
editor.type(" world");
console.log(editor.show());
history.save();
editor.moveTo(5);
editor.type(",");
console.log(editor.show());

for (let i = 0; i < 4; i++) {
  console.log(history.undo() ? `undo: ${editor.show()}` : "nothing to undo");
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

- Verdict: idiom. The memento is an empty frozen object, and the editor keeps the saved state in a private static `WeakMap` keyed by that object. Code outside `Editor` cannot reach the state, even by reflection.
- A `WeakMap` does not keep its keys alive. When the caretaker drops a memento, the saved state can be collected with it.
- The shorter form returns `Object.freeze({ text, cursor })`. The caretaker can then read the fields, so the encapsulation rests on a convention, as in Python.
- `structuredClone` makes a deep copy of a plain state object, which suits an originator whose state holds arrays or nested objects.
