```cpp run id=simple-factory
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

struct Notifier {
    virtual ~Notifier() = default;
    virtual std::string name() const = 0;
    virtual std::string send(const std::string& user, const std::string& text) const = 0;
};

struct EmailNotifier : Notifier {
    std::string name() const override { return "EmailNotifier"; }
    std::string send(const std::string& user, const std::string& text) const override {
        return "mail to " + user + "@example.com: " + text;
    }
};

struct SmsNotifier : Notifier {
    std::string name() const override { return "SmsNotifier"; }
    std::string send(const std::string& user, const std::string& text) const override {
        return "text to the phone of " + user + ": " + text;
    }
};

struct PushNotifier : Notifier {
    std::string name() const override { return "PushNotifier"; }
    std::string send(const std::string& user, const std::string& text) const override {
        return "push to the app of " + user + ": " + text;
    }
};

// The one place that knows the concrete classes.
std::unique_ptr<Notifier> make_notifier(const std::string& channel) {
    if (channel == "email") return std::make_unique<EmailNotifier>();
    if (channel == "sms") return std::make_unique<SmsNotifier>();
    if (channel == "push") return std::make_unique<PushNotifier>();
    throw std::invalid_argument("unknown channel: " + channel);
}

int main() {
    for (const std::string channel : {"email", "sms", "push", "fax"}) {
        std::unique_ptr<Notifier> notifier;
        try {
            notifier = make_notifier(channel);
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\n";
            continue;
        }
        std::cout << notifier->name() << ": " << notifier->send("ann", "order shipped") << "\n";
    }
}
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A free function returns `std::unique_ptr` to the base class. The caller owns the object, and only the file with the factory needs the headers of the concrete classes.
- The base class needs a virtual destructor, so that deleting through the base pointer runs the destructor of the concrete class.
