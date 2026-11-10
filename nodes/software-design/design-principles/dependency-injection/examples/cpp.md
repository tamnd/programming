```cpp run id=invoices
#include <iostream>
#include <string>
#include <vector>

struct Clock {
    virtual ~Clock() = default;
    virtual int today() const = 0;
};

struct Mailer {
    virtual ~Mailer() = default;
    virtual void send(const std::string& to, const std::string& text) = 0;
};

struct Invoice {
    int number;
    std::string customer;
    int due;  // day number
};

// The service holds references. Whoever builds it owns the collaborators.
class ReminderService {
public:
    ReminderService(const Clock& clock, Mailer& mailer) : clock_(clock), mailer_(mailer) {}

    bool remind(const Invoice& invoice) {
        int late = clock_.today() - invoice.due;
        if (late <= 0) return false;
        mailer_.send(invoice.customer,
                     "invoice " + std::to_string(invoice.number) + " is " +
                         std::to_string(late) + " days overdue");
        return true;
    }

private:
    const Clock& clock_;
    Mailer& mailer_;
};

class FixedClock : public Clock {
public:
    explicit FixedClock(int day) : day_(day) {}
    int today() const override { return day_; }

private:
    int day_;
};

class ConsoleMailer : public Mailer {
public:
    void send(const std::string& to, const std::string& text) override {
        std::cout << "to " << to << ": " << text << "\n";
    }
};

class RecordingMailer : public Mailer {
public:
    std::vector<std::string> sent;
    void send(const std::string& to, const std::string& text) override {
        sent.push_back("to " + to + ": " + text);
    }
};

int main() {
    // The composition root owns every object, and they live until main returns.
    FixedClock clock(40);
    ConsoleMailer mailer;
    ReminderService service(clock, mailer);
    std::vector<Invoice> invoices{{17, "ana@example.com", 31}, {18, "bo@example.com", 45}};
    int sent = 0;
    for (const auto& invoice : invoices) sent += service.remind(invoice);
    std::cout << "reminders sent: " << sent << "\n";

    // A test wires the same class to a recording double.
    FixedClock later(50);
    RecordingMailer recorder;
    ReminderService(later, recorder).remind(invoices[1]);
    std::cout << "recorded: " << recorder.sent[0] << "\n";
}
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. Abstract classes with pure virtual functions describe the roles, and the constructor takes references to them.
- The service holds references, so it does not own its collaborators. They must outlive the service. Here `main` owns them all, so a reader can check that rule in one function. A service that must own a collaborator takes a `std::unique_ptr`, and one that shares it takes a `std::shared_ptr`.
- A template parameter instead of a virtual base gives static injection, as in the Rust example: no virtual calls, and one copy of the class for each set of types.
