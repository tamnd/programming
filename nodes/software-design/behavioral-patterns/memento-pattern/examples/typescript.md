```typescript run id=undo
declare const mementoBrand: unique symbol;

// The narrow interface. The caretaker sees a type with no readable members.
type Memento = { readonly [mementoBrand]: "Editor" };

type State = { readonly text: string; readonly cursor: number };

// The originator. Only it turns a Memento back into a State.
class Editor {
  #text = "";
  #cursor = 0;

  type(chars: string): void {
    this.#text = this.#text.slice(0, this.#cursor) + chars + this.#text.slice(this.#cursor);
    this.#cursor += chars.length;
  }
  moveTo(cursor: number): void {
    this.#cursor = Math.max(0, Math.min(cursor, this.#text.length));
  }
  save(): Memento {
    const state: State = Object.freeze({ text: this.#text, cursor: this.#cursor });
    return state as unknown as Memento;
  }
  restore(memento: Memento): void {
    const state = memento as unknown as State; // the wide interface, used only here
    this.#text = state.text;
    this.#cursor = state.cursor;
  }
  show(): string {
    return this.#text.slice(0, this.#cursor) + "|" + this.#text.slice(this.#cursor);
  }
}

// The caretaker. It keeps mementos and cannot read them without a cast.
class History {
  readonly #undo: Memento[] = [];
  readonly #editor: Editor;
  constructor(editor: Editor) {
    this.#editor = editor;
  }
  save(): void {
    this.#undo.push(this.#editor.save());
  }
  undo(): boolean {
    const memento = this.#undo.pop();
    if (memento === undefined) return false;
    this.#editor.restore(memento);
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

- Verdict: idiom. A branded type is the narrow interface. `Memento` has one member, keyed by a `unique symbol` that no code can name outside its declaration, so the caretaker can store a memento but not read it.
- The brand exists only in the type system. At run time the memento is the frozen state object, and the casts in `save` and `restore` are the one place where the two views meet.
- `declare const` emits no code, so Node.js type stripping accepts it.
