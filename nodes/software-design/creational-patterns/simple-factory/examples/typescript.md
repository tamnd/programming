```typescript run id=simple-factory
interface Notifier {
  send(user: string, text: string): string;
}

class EmailNotifier implements Notifier {
  send(user: string, text: string): string {
    return `mail to ${user}@example.com: ${text}`;
  }
}

class SmsNotifier implements Notifier {
  send(user: string, text: string): string {
    return `text to the phone of ${user}: ${text}`;
  }
}

class PushNotifier implements Notifier {
  send(user: string, text: string): string {
    return `push to the app of ${user}: ${text}`;
  }
}

type Channel = "email" | "sms" | "push";

// The one place that knows the concrete classes.
function makeNotifier(channel: Channel): Notifier {
  switch (channel) {
    case "email":
      return new EmailNotifier();
    case "sms":
      return new SmsNotifier();
    case "push":
      return new PushNotifier();
    default: {
      const unknown: never = channel; // a compile error if a case is missing
      throw new RangeError(`unknown channel: ${String(unknown)}`);
    }
  }
}

function isChannel(text: string): text is Channel {
  return text === "email" || text === "sms" || text === "push";
}

for (const channel of ["email", "sms", "push", "fax"]) {
  if (!isChannel(channel)) {
    console.log(`unknown channel: ${channel}`);
    continue;
  }
  const notifier = makeNotifier(channel);
  console.log(`${notifier.constructor.name}: ${notifier.send("ann", "order shipped")}`);
}
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A union type for the parameter and a `never` check in the `default` branch make the compiler report a channel that the factory does not handle.
- Text from outside, such as a configuration value, goes through a type guard before it reaches the factory.
