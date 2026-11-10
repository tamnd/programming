```javascript run id=todo
// The model extends EventTarget, so views can listen for "change" events.
class TodoModel extends EventTarget {
  #items = [];

  items() {
    return this.#items.map((item) => ({ ...item }));
  }
  add(title) {
    if (!title) throw new RangeError("a title is required");
    this.#items.push({ title, done: false });
    this.dispatchEvent(new Event("change"));
  }
  complete(number) {
    if (number < 1 || number > this.#items.length) throw new RangeError(`no item ${number}`);
    this.#items[number - 1].done = true;
    this.dispatchEvent(new Event("change"));
  }
}

function listView(model) {
  model.addEventListener("change", () => {
    model.items().forEach((item, i) => console.log(`  [${item.done ? "x" : " "}] ${i + 1} ${item.title}`));
  });
}

function summaryView(model) {
  model.addEventListener("change", () => {
    const items = model.items();
    const done = items.filter((item) => item.done).length;
    console.log(`  summary: ${items.length - done} open, ${done} done`);
  });
}

class Controller {
  #model;
  constructor(model) { this.#model = model; }

  handle(line) {
    console.log(`> ${line}`);
    const [command, ...rest] = line.split(" ");
    const argument = rest.join(" ");
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
listView(model);
summaryView(model);
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

- Verdict: idiom. `EventTarget` is part of the platform in browsers and in Node, so the model gets its observer list from the runtime.
- In a browser, the views would write to the DOM, and the controller would be the event handlers of the form. The three roles stay the same.
- `items()` returns copies, so a view cannot change the model while it renders.
