```typescript run id=consultants
type Handler = (c: Consultant) => void;

class Signal {
  private handlers: Handler[] = [];
  add(h: Handler): void { this.handlers.push(h); }
  fire(c: Consultant): void { for (const h of this.handlers) h(c); }
}

class Consultant {
  readonly locationChanged = new Signal();
  readonly availabilityChanged = new Signal();
  name: string;
  home: string;
  availableAtHome: boolean;
  location: string;
  constructor(name: string, home: string, availableAtHome: boolean, location: string) {
    this.name = name;
    this.home = home;
    this.availableAtHome = availableAtHome;
    this.location = location;
  }
  availableAt(place: string): boolean {
    return place === this.home ? this.availableAtHome : true;
  }
  move(place: string): void {
    console.log(`${this.name} moves to ${place}`);
    if (this.location !== place) {
      this.locationChanged.fire(this);
      if (this.availableAt(place) !== this.availableAt(this.location)) this.availabilityChanged.fire(this);
      this.location = place;
    }
  }
}

class EventAggregator {
  readonly changed = new Signal();
  readonly location = new Signal();
  readonly availability = new Signal();
  listen(c: Consultant): void {
    c.locationChanged.add((x) => this.signal(this.location, x));
    c.availabilityChanged.add((x) => this.signal(this.availability, x));
  }
  private signal(specific: Signal, c: Consultant): void {
    specific.fire(c); // the specific event
    this.changed.fire(c); // and the general one
  }
}

const agg = new EventAggregator();
const alice = new Consultant("alice", "London", false, "London");
const bob = new Consultant("bob", "Paris", true, "Lyon");
for (const c of [alice, bob]) agg.listen(c);

agg.changed.add((c) => console.log(`screen: ${c.name} changed`));
agg.availability.add((c) => console.log(`phone: availability of ${c.name} changed`));

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

- Verdict: idiom. A small typed `Signal` class holds the handlers of one event.
