```typescript run id=invoices
interface Clock {
  today(): number;
}

interface Mailer {
  send(to: string, text: string): void;
}

interface Invoice {
  readonly number: number;
  readonly customer: string;
  readonly due: number;
}

class ReminderService {
  readonly #clock: Clock;
  readonly #mailer: Mailer;

  constructor(clock: Clock, mailer: Mailer) {
    this.#clock = clock;
    this.#mailer = mailer;
  }

  remind(invoice: Invoice): boolean {
    const late = this.#clock.today() - invoice.due;
    if (late <= 0) return false;
    this.#mailer.send(invoice.customer, `invoice ${invoice.number} is ${late} days overdue`);
    return true;
  }
}

class FixedClock implements Clock {
  readonly #day: number;
  constructor(day: number) {
    this.#day = day;
  }
  today(): number {
    return this.#day;
  }
}

class ConsoleMailer implements Mailer {
  send(to: string, text: string): void {
    console.log(`to ${to}: ${text}`);
  }
}

class RecordingMailer implements Mailer {
  readonly sent: string[] = [];
  send(to: string, text: string): void {
    this.sent.push(`to ${to}: ${text}`);
  }
}

// The composition root.
const service = new ReminderService(new FixedClock(40), new ConsoleMailer());
const invoices: Invoice[] = [
  { number: 17, customer: "ana@example.com", due: 31 },
  { number: 18, customer: "bo@example.com", due: 45 },
];
const sent = invoices.filter((i) => service.remind(i)).length;
console.log(`reminders sent: ${sent}`);

// A test double needs no class at all: an object literal has the right shape.
const recorded: string[] = [];
const recorder: Mailer = { send: (to, text) => recorded.push(`to ${to}: ${text}`) };
new ReminderService(new FixedClock(50), recorder).remind(invoices[1]);
console.log(`recorded: ${recorded[0]}`);
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. Interfaces describe the roles, and the constructor parameters have those types. The compiler rejects a call with a collaborator of the wrong shape.
- TypeScript types are structural. `FixedClock` names `Clock` in `implements`, but the object literal `recorder` also counts as a `Mailer`, because it has a `send` method with the right type.
- The interfaces exist only at compile time. A container that reads constructor parameter types at run time needs decorators and metadata from a library, which this page does not use.
