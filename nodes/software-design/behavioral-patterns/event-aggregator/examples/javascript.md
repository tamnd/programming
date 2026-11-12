```javascript run id=consultants
const { EventEmitter } = require("node:events");

class Consultant extends EventEmitter {
  constructor(name, home, availableAtHome, location) {
    super();
    Object.assign(this, { name, home, availableAtHome, location });
  }
  availableAt(place) {
    return place === this.home ? this.availableAtHome : true;
  }
  move(place) {
    console.log(`${this.name} moves to ${place}`);
    if (this.location !== place) {
      this.emit("locationChanged", this);
      if (this.availableAt(place) !== this.availableAt(this.location)) this.emit("availabilityChanged", this);
      this.location = place;
    }
  }
}

class EventAggregator extends EventEmitter {
  listen(consultant) {
    consultant.on("locationChanged", (c) => this.signal("location", c));
    consultant.on("availabilityChanged", (c) => this.signal("availability", c));
  }
  signal(specific, c) {
    this.emit(specific, c); // the specific event
    this.emit("changed", c); // and the general one
  }
}

const agg = new EventAggregator();
const alice = new Consultant("alice", "London", false, "London");
const bob = new Consultant("bob", "Paris", true, "Lyon");
for (const c of [alice, bob]) agg.listen(c);

agg.on("changed", (c) => console.log(`screen: ${c.name} changed`));
agg.on("availability", (c) => console.log(`phone: availability of ${c.name} changed`));

alice.move("Leeds");
bob.move("Paris");
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: library. Node's `EventEmitter` gives the events; the aggregator is an emitter that listens to other emitters.
