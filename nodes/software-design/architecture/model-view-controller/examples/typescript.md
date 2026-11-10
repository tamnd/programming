```typescript run id=todo
interface Item {
  readonly title: string;
  readonly done: boolean;
}

type Listener = () => void;

class TodoModel {
  #items: Item[] = [];
  #listeners: Listener[] = [];

  subscribe(listener: Listener): void {
    this.#listeners.push(listener);
  }
  items(): readonly Item[] {
    return this.#items;
  }
  add(title: string): void {
    if (title === "") throw new RangeError("a title is required");
    this.#items = [...this.#items, { title, done: false }];
    this.#changed();
  }
  complete(number: number): void {
    if (number < 1 || number > this.#items.length) throw new RangeError(`no item ${number}`);
    this.#items = this.#items.map((item, i) => (i === number - 1 ? { ...item, done: true } : item));
    this.#changed();
  }
  #changed(): void {
    this.#listeners.forEach((listener) => listener());
  }
}

// A view is a function from the model's data to output.
const renderList = (items: readonly Item[]): string[] =>
  items.map((item, i) => `  [${item.done ? "x" : " "}] ${i + 1} ${item.title}`);

const renderSummary = (items: readonly Item[]): string[] => {
  const done = items.filter((item) => item.done).length;
  return [`  summary: ${items.length - done} open, ${done} done`];
};

class Controller {
  readonly #model: TodoModel;
  constructor(model: TodoModel) {
    this.#model = model;
  }

  handle(line: string): void {
    console.log(`> ${line}`);
    const space = line.indexOf(" ");
    const command = space < 0 ? line : line.slice(0, space);
    const argument = space < 0 ? "" : line.slice(space + 1);
    try {
      if (command === "add") this.#model.add(argument);
      else if (command === "done" && /^\d+$/.test(argument)) this.#model.complete(Number(argument));
      else console.log(`  error: cannot read '${line}'`);
    } catch (err) {
      if (!(err instanceof RangeError)) throw err;
      console.log(`  error: ${err.message}`);
    }
  }
}

const model = new TodoModel();
for (const view of [renderList, renderSummary]) {
  model.subscribe(() => view(model.items()).forEach((line) => console.log(line)));
}
const controller = new Controller(model);
for (const line of ["add buy milk", "add write report", "done 1", "done 7", "rename 2"]) controller.handle(line);
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. The model replaces its array on each change instead of changing it in place, and `items()` returns it as `readonly Item[]`, so the compiler rejects a view that tries to change it.
- Each view is a pure function from data to lines of text. The subscription connects it to the model. This is the shape that UI libraries built on render functions use.
- The model has no type for views. It knows only the `Listener` type, a function with no arguments.
