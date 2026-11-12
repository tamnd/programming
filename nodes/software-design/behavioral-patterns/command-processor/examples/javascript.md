```javascript run id=processor
class Insert {
  constructor(words, word) {
    this.words = words;
    this.word = word;
  }
  name() {
    return `insert ${this.word}`;
  }
  execute() {
    this.words.push(this.word);
  }
  undo() {
    this.words.pop();
  }
}

class Processor {
  constructor() {
    this.done = [];
  }
  doIt(command) {
    console.log(`do: ${command.name()}`);
    command.execute();
    this.done.push(command);
  }
  undoIt() {
    const command = this.done.pop();
    console.log(`undo: ${command.name()}`);
    command.undo();
  }
}

const words = [];
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

- Verdict: idiom. A command is any object with `name`, `execute` and `undo`, and the processor keeps an array of executed commands.
