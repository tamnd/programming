```typescript run id=acceptor
interface ServiceHandler {
  open(channel: string[]): void;
}

class StatusHandler implements ServiceHandler {
  private channel: string[] = [];
  open(channel: string[]): void {
    this.channel = channel;
    console.log("accept -> status handler open");
  }
  poll(): void {
    console.log("status: got", this.channel.shift());
  }
}

class CommandHandler implements ServiceHandler {
  private channel: string[] = [];
  open(channel: string[]): void {
    this.channel = channel;
    console.log("connect done -> command handler open");
  }
  send(text: string): void {
    console.log("command: send", text);
    this.channel.push(text);
  }
}

const listeners = new Map<string, Acceptor<ServiceHandler>>();

class Acceptor<H extends ServiceHandler> {
  readonly handlers: H[] = [];
  private make: () => H;
  constructor(address: string, make: () => H) {
    this.make = make;
    listeners.set(address, this);
    console.log("listen", address);
  }
  accept(channel: string[]): void {
    const handler = this.make();
    handler.open(channel);
    this.handlers.push(handler);
  }
}

class Connector {
  connect<H extends ServiceHandler>(address: string, make: () => H): H {
    console.log("connect", address);
    const channel: string[] = [];
    listeners.get(address)?.accept(channel);
    return this.complete(channel, make);
  }
  private complete<H extends ServiceHandler>(channel: string[], make: () => H): H {
    const handler = make();
    handler.open(channel);
    return handler;
  }
}

const acceptor = new Acceptor("gateway:9000", () => new StatusHandler());
const command = new Connector().connect("gateway:9000", () => new CommandHandler());
command.send("reset");
acceptor.handlers[0].poll();
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. The acceptor and the connector are generic over the handler type, so `connect` returns a `CommandHandler` and the caller can call `send` without a cast.
