```javascript run id=invoices
// The service receives its collaborators. It never creates them.
class ReminderService {
  #clock;
  #mailer;

  constructor({ clock, mailer }) {
    this.#clock = clock;
    this.#mailer = mailer;
  }

  remind(invoice) {
    const late = this.#clock.today() - invoice.due;
    if (late <= 0) return false;
    this.#mailer.send(invoice.customer, `invoice ${invoice.number} is ${late} days overdue`);
    return true;
  }
}

// Implementations are plain objects. Each one only needs the right methods.
const fixedClock = (day) => ({ today: () => day });
const consoleMailer = { send: (to, text) => console.log(`to ${to}: ${text}`) };

function recordingMailer() {
  const sent = [];
  return { sent, send: (to, text) => sent.push(`to ${to}: ${text}`) };
}

// The composition root.
const service = new ReminderService({ clock: fixedClock(40), mailer: consoleMailer });
const invoices = [
  { number: 17, customer: "ana@example.com", due: 31 },
  { number: 18, customer: "bo@example.com", due: 45 },
];
const sent = invoices.filter((i) => service.remind(i)).length;
console.log(`reminders sent: ${sent}`);

// A test wires the same class to a recording double.
const recorder = recordingMailer();
new ReminderService({ clock: fixedClock(50), mailer: recorder }).remind(invoices[1]);
console.log(`recorded: ${recorder.sent[0]}`);
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. The constructor takes one object with named properties, so the call site shows which collaborator is which and the order does not matter.
- The collaborators are object literals and closures. JavaScript checks nothing at the call: a mailer without `send` fails only when `remind` calls it.
- Private fields (`#clock`, `#mailer`) keep other code from replacing a collaborator after construction.
