```typescript run id=editor
interface Command {
  execute(): void;
  undo(): void;
}

class Buffer {
  text = "";
}

class Append implements Command {
  private readonly buffer: Buffer;
  private readonly text: string;

  constructor(buffer: Buffer, text: string) {
    this.buffer = buffer;
    this.text = text;
  }

  execute(): void {
    this.buffer.text += this.text;
  }

  undo(): void {
    this.buffer.text = this.buffer.text.slice(0, this.buffer.text.length - this.text.length);
  }
}

class Delete implements Command {
  private readonly buffer: Buffer;
  private readonly count: number;
  private removed = "";

  constructor(buffer: Buffer, count: number) {
    this.buffer = buffer;
    this.count = count;
  }

  execute(): void {
    const text = this.buffer.text;
    this.removed = text.slice(text.length - this.count);
    this.buffer.text = text.slice(0, text.length - this.count);
  }

  undo(): void {
    this.buffer.text += this.removed;
  }
}

class History {
  private readonly undoStack: Command[] = [];
  private readonly redoStack: Command[] = [];

  run(command: Command): void {
    command.execute();
    this.undoStack.push(command);
    this.redoStack.length = 0;
  }

  undo(): boolean {
    const command = this.undoStack.pop();
    if (command === undefined) return false;
    command.undo();
    this.redoStack.push(command);
    return true;
  }

  redo(): boolean {
    const command = this.redoStack.pop();
    if (command === undefined) return false;
    command.execute();
    this.undoStack.push(command);
    return true;
  }
}

const buffer = new Buffer();
const history = new History();
const show = (label: string): void => console.log(`${label}: "${buffer.text}"`);
history.run(new Append(buffer, "hello")); show("append");
history.run(new Append(buffer, " world")); show("append");
history.run(new Delete(buffer, 5)); show("delete");
history.undo(); show("undo");
history.undo(); show("undo");
history.redo(); show("redo");
history.run(new Append(buffer, "!")); show("append");
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

- Verdict: idiom. The classic form: an interface for the command, and one class for each kind of command. The receiver, the buffer, is a constructor parameter.
- The fields are declared and set in the constructor. Parameter properties, such as `constructor(private readonly buffer: Buffer)`, are shorter, but Node.js rejects them when it strips types, because they generate code.
- `pop` returns `Command | undefined`, so the compiler makes `undo` and `redo` handle an empty stack.
