```javascript run id=acceptor
class StatusHandler {
  open(channel) {
    this.channel = channel;
    console.log("accept -> status handler open");
  }
  poll() {
    console.log("status: got", this.channel.shift());
  }
}

class CommandHandler {
  open(channel) {
    this.channel = channel;
    console.log("connect done -> command handler open");
  }
  send(text) {
    console.log("command: send", text);
    this.channel.push(text);
  }
}

const listeners = new Map(); // stands in for the network

class Acceptor {
  constructor(address, HandlerClass) {
    this.HandlerClass = HandlerClass;
    this.handlers = [];
    listeners.set(address, this);
    console.log("listen", address);
  }
  accept(channel) {
    const handler = new this.HandlerClass();
    handler.open(channel);
    this.handlers.push(handler);
  }
}

class Connector {
  connect(address, HandlerClass) {
    console.log("connect", address);
    const channel = [];
    listeners.get(address).accept(channel);
    return this.complete(channel, HandlerClass);
  }
  complete(channel, HandlerClass) {
    const handler = new HandlerClass();
    handler.open(channel);
    return handler;
  }
}

const acceptor = new Acceptor("gateway:9000", StatusHandler);
const command = new Connector().connect("gateway:9000", CommandHandler);
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

- Verdict: idiom. A class is a value, so the acceptor stores the handler class and calls `new` on it for each connection.
