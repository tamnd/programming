```typescript run id=processor
interface Command {
  name(): string;
  execute(): void;
  undo(): void;
}

class Insert implements Command {
  private words: string[];
  private word: string;

  constructor(words: string[], word: string) {
    this.words = words;
    this.word = word;
  }
  name(): string {
    return `insert ${this.word}`;
  }
  execute(): void {
    this.words.push(this.word);
  }
  undo(): void {
    this.words.pop();
  }
}

class Processor {
  private done: Command[] = [];

  doIt(command: Command): void {
    console.log(`do: ${command.name()}`);
    command.execute();
    this.done.push(command);
  }
  undoIt(): void {
    const command = this.done.pop();
    if (command === undefined) return;
    console.log(`undo: ${command.name()}`);
    command.undo();
  }
}

const words: string[] = [];
const processor = new Processor();
processor.doIt(new Insert(words, "hello"));
processor.doIt(new Insert(words, "world"));
console.log("text:", words.join(" "));
processor.undoIt();
console.log("text:", words.join(" "));
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. The interface `Command` is the abstract command, and the processor knows only that interface.
