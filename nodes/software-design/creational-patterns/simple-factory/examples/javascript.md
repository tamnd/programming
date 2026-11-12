```javascript run id=simple-factory
class EmailNotifier {
  send(user, text) {
    return `mail to ${user}@example.com: ${text}`;
  }
}

class SmsNotifier {
  send(user, text) {
    return `text to the phone of ${user}: ${text}`;
  }
}

class PushNotifier {
  send(user, text) {
    return `push to the app of ${user}: ${text}`;
  }
}

// The one place that knows the concrete classes.
const NOTIFIERS = new Map([
  ["email", EmailNotifier],
  ["sms", SmsNotifier],
  ["push", PushNotifier],
]);

function makeNotifier(channel) {
  const Kind = NOTIFIERS.get(channel);
  if (Kind === undefined) throw new RangeError(`unknown channel: ${channel}`);
  return new Kind();
}

for (const channel of ["email", "sms", "push", "fax"]) {
  let notifier;
  try {
    notifier = makeNotifier(channel);
  } catch (e) {
    console.log(e.message);
    continue;
  }
  console.log(`${notifier.constructor.name}: ${notifier.send("ann", "order shipped")}`);
}
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A class is a value in JavaScript, so a `Map` from the parameter to the class replaces a `switch`.
- A `Map` is safer than a plain object for the table, because a lookup in a plain object also finds inherited keys such as `"toString"`.
