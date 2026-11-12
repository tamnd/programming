```cpp run id=consultants
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Consultant;
using Handler = std::function<void(const Consultant&)>;

struct Signal {
    std::vector<Handler> handlers;
    void add(Handler h) { handlers.push_back(std::move(h)); }
    void fire(const Consultant& c) const { for (const auto& h : handlers) h(c); }
};

struct Consultant {
    std::string name, home;
    bool availableAtHome;
    std::string location;
    Signal locationChanged, availabilityChanged;

    bool availableAt(const std::string& place) const { return place == home ? availableAtHome : true; }

    void move(const std::string& place) {
        std::cout << name << " moves to " << place << "\n";
        if (location != place) {
            locationChanged.fire(*this);
            if (availableAt(place) != availableAt(location)) availabilityChanged.fire(*this);
            location = place;
        }
    }
};

struct EventAggregator {
    Signal changed, location, availability;

    void listen(Consultant& c) {
        c.locationChanged.add([this](const Consultant& x) { signal(location, x); });
        c.availabilityChanged.add([this](const Consultant& x) { signal(availability, x); });
    }
    void signal(const Signal& specific, const Consultant& c) const {
        specific.fire(c);  // the specific event
        changed.fire(c);   // and the general one
    }
};

int main() {
    EventAggregator agg;
    Consultant alice{"alice", "London", false, "London", {}, {}};
    Consultant bob{"bob", "Paris", true, "Lyon", {}, {}};
    agg.listen(alice);
    agg.listen(bob);

    agg.changed.add([](const Consultant& c) { std::cout << "screen: " << c.name << " changed\n"; });
    agg.availability.add([](const Consultant& c) { std::cout << "phone: availability of " << c.name << " changed\n"; });

    alice.move("Leeds");
    bob.move("Paris");
}
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: idiom. Vectors of `std::function` values are the events. The aggregator must live longer than the consultants that hold its lambdas.
