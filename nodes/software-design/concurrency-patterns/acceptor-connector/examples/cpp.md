```cpp run id=acceptor
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using Channel = std::shared_ptr<std::deque<std::string>>;

struct StatusHandler {
    Channel channel;
    void open(Channel c) { channel = c; std::cout << "accept -> status handler open\n"; }
    void poll() { std::cout << "status: got " << channel->front() << "\n"; channel->pop_front(); }
};

struct CommandHandler {
    Channel channel;
    void open(Channel c) { channel = c; std::cout << "connect done -> command handler open\n"; }
    void send(const std::string& text) {
        std::cout << "command: send " << text << "\n";
        channel->push_back(text);
    }
};

struct AcceptorBase {
    virtual ~AcceptorBase() = default;
    virtual void accept(Channel channel) = 0;
};

std::map<std::string, AcceptorBase*> listeners;  // stands in for the network

template <typename Handler>
struct Acceptor : AcceptorBase {
    std::vector<Handler> handlers;
    explicit Acceptor(const std::string& address) {
        listeners[address] = this;
        std::cout << "listen " << address << "\n";
    }
    void accept(Channel channel) override {
        Handler handler;
        handler.open(channel);
        handlers.push_back(handler);
    }
};

template <typename Handler>
Handler connect(const std::string& address) {
    std::cout << "connect " << address << "\n";
    auto channel = std::make_shared<std::deque<std::string>>();
    listeners.at(address)->accept(channel);
    Handler handler;  // complete: activate when the connection is up
    handler.open(channel);
    return handler;
}

int main() {
    Acceptor<StatusHandler> acceptor("gateway:9000");
    auto command = connect<CommandHandler>("gateway:9000");
    command.send("reset");
    acceptor.handlers[0].poll();
}
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. The acceptor and the connector are templates over the handler type, as in the paper's C++ code.
