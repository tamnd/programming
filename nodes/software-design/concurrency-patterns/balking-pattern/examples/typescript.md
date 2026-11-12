```typescript run id=balking-pattern
// The tasks share one thread. The await in save lets other calls run.
class Document {
  write: () => Promise<void>;
  changed: boolean;
  saving: boolean;
  saves: number;

  constructor(write: () => Promise<void>) {
    this.write = write; // the slow part of a save
    this.changed = false;
    this.saving = false;
    this.saves = 0;
  }

  edit(): void {
    this.changed = true;
  }

  // Saves the document. Returns false at once if a save cannot start now.
  async save(): Promise<boolean> {
    if (this.saving || !this.changed) return false; // balk: the object is not in the right state
    this.saving = true;
    this.changed = false;
    await this.write();
    this.saving = false;
    this.saves++;
    return true;
  }
}

async function main(): Promise<void> {
  let release: () => void = () => {};
  const released = new Promise<void>((resolve) => (release = resolve));
  const doc = new Document(() => released); // the first save stays in progress
  doc.edit();
  const first = doc.save();

  const results = await Promise.all(Array.from({ length: 7 }, () => doc.save()));
  release();
  await first;

  const word = (ran: boolean): string => (ran ? "ran" : "balked");
  console.log("calls during a save:", results.length);
  console.log("calls that balked:", results.filter((r) => !r).length);
  console.log("saves run:", doc.saves);
  console.log("save with no changes:", word(await doc.save()));
  doc.edit();
  console.log("save after an edit:", word(await doc.save()));
  console.log("saves run:", doc.saves);
}

main();
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: idiom. The tasks share one thread, so the test and the change of `saving` run with no await between them.
