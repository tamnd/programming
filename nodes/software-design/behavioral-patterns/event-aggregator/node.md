---
tag: GM2N7C
kind: pattern
slug: event-aggregator
name: Event Aggregator
aliases: [event aggregator pattern, EventAggregator]
area: software-design/behavioral-patterns
level: advanced
summary: >-
  An event aggregator is a single source of events for many objects. It registers for the events of the source objects, and clients register only with it.
family: behavioral-design
polarity: positive
intent: Channel events from multiple objects into a single object to simplify registration for clients.
also_known_as: []
history:
  year: 2004
  people: [Martin Fowler]
  note: Fowler described Event Aggregator on 29 September 2004, as part of his draft writing for Further Enterprise Application Architecture. Microsoft's Composite Application Library gives an EventAggregator service, and the Prism library has a class EventAggregator.
see_also: [observer-pattern, mediator-pattern, facade-pattern, adapter-pattern, event-driven-programming, garbage-collection]
sources:
  - url: https://martinfowler.com/eaaDev/EventAggregator.html
    title: Martin Fowler, Event Aggregator
    locator: summary; How it Works; When to Use It; Further Reading; Example, Watching Our Consultants (C#)
  - url: https://learn.microsoft.com/en-us/previous-versions/msp-n-p/ff921122(v=pandp.20)
    title: Event Aggregator, Composite Application Guidance, Microsoft Learn
    locator: IEventAggregator; CompositePresentationEvent; Subscribing to an Event
  - url: https://github.com/PrismLibrary/Prism/blob/v8.1.97/src/Prism.Core/Events/EventAggregator.cs#L12-L44
    title: Prism v8.1.97 source, src/Prism.Core/Events/EventAggregator.cs
    locator: class EventAggregator and GetEvent, lines 12 to 44
---

## Intent

Channel events from multiple objects into a single object to simplify registration for clients.[^1]

## Problem

A system with lots of objects can lead to complexities when a client wants to subscribe to events. The client has to find and register for each object individually. If each object has multiple events, then each event requires a separate subscription.[^1]

## Solution

An Event Aggregator acts as a single source of events for many objects. It registers for all the events of the many objects, and clients register with just the aggregator.[^1] It is a simple element of indirection. When an event comes from a source object, the aggregator propagates that event to the target objects.[^1]

```text
   consultant A --\                       /--> screen
   consultant B ----> EventAggregator ---+
   consultant C --/   (registers once      \--> phone
                       with each source)
         sources                             clients register only here
```

An aggregator can also generalize the event. It converts events that are specific to a source object into a more generic event, so the observers need to register for fewer individual event types.[^1]

## Example

Fowler's example tracks the consultants of a company. When a consultant moves, the consultant object signals one event for the change of location, and one more event if the availability changes.[^1] The aggregator listens to all consultants. For each incoming event it signals two events: a specific one for the type of change, and a general one that only says that a change occurred. Clients can then register to the granularity they wish.[^1]

In the example files, a screen registers for the general event and a phone registers for the specific availability event. Neither of them knows the consultants. Alice is not available at home, so her move away from home also changes her availability. The program prints:

```text
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

## Consequences

The benefits:

- The registration logic is in one place, not in each observer.[^1]
- An aggregator also simplifies the memory management issues in using observers.[^1]
- Publishers and subscribers are decoupled, so they can evolve independently. New modules can be added that respond to events that other modules define.[^2]

The costs:

- A generic event simplifies registration, but observers are notified of events that may not have any material effect on them.[^1] In the example, the screen gets two notifications for one move of Alice.
- An Event Aggregator is based on Observer, so all the danger areas of Observer apply.[^1]

Without an aggregator, each client registers with each source. When a new source is added, each client must register with it, or the client misses its events. Here the screen did not register with Carol, who joined later:

```python wrong id=missed
class Consultant:
    def __init__(self, name):
        self.name, self.handlers = name, []

    def move(self, place):
        print(f"{self.name} moves to {place}")
        for h in self.handlers:
            h(self)


def screen(c):
    print(f"screen: {c.name} changed")


staff = [Consultant("alice"), Consultant("bob")]
for c in staff:
    c.handlers.append(screen)  # the screen registers with each consultant
staff.append(Consultant("carol"))  # joins later; nobody registers

staff[0].move("Leeds")
staff[2].move("Rome")  # the screen is not told
```

```output for=missed
alice moves to Leeds
screen: alice changed
carol moves to Rome
```

## When not to use it

- There are few event sources, and the client can register with each of them directly.[^1]
- Each client needs the full detail of the specific events of one source. Fowler suggests a single coarse-grained event for most cases.[^1]

## Variants and related patterns

- **Facade and Adapter.** An Event Aggregator is a particular form of Facade that focuses only on observer relationships.[^1] When it converts events into other events, it also acts as an Adapter.[^1]
- **Events by type.** In the Composite Application Library, the aggregator keeps a collection of the events in the system, and publishers and subscribers use it to locate an event. The aggregator constructs an event on its first access, so the publisher or subscriber does not need to determine whether the event is available.[^2] Here the publisher and the subscriber get the same event from the aggregator:

```python run id=by-type
class Event:
    def __init__(self):
        self.subscribers = []

    def subscribe(self, action):
        self.subscribers.append(action)

    def publish(self, payload):
        for action in self.subscribers:
            action(payload)


class OrderSaved(Event):
    pass


class EventAggregator:
    def __init__(self):
        self.events = {}

    def get_event(self, event_type):
        if event_type not in self.events:  # built on first access
            self.events[event_type] = event_type()
        return self.events[event_type]


bus = EventAggregator()
bus.get_event(OrderSaved).subscribe(lambda n: print("mail: order", n))
bus.get_event(OrderSaved).subscribe(lambda n: print("audit: order", n))
bus.get_event(OrderSaved).publish(42)  # the publisher knows no subscriber
```

```output for=by-type
mail: order 42
audit: order 42
```

- **Weak references.** A Prism event can hold a weak delegate reference to the subscriber. This reference does not prevent garbage collection of the subscriber, so the subscriber does not need to unsubscribe.[^2]
- **Observer.** An Event Aggregator is based on Observer. It observes the source objects, and the clients observe it.[^1]

## Known uses

- **Prism.** The class `EventAggregator` keeps a dictionary from event types to events. Its method `GetEvent<TEventType>()` gets the single instance of the event managed by this aggregator: multiple calls with the same type return the same event instance.[^3] The `CompositePresentationEvent` class of the earlier Composite Application Library keeps the list of subscribers and dispatches events to them.[^2]
- **Fowler's consultant screen.** Fowler's C# example registers for the `LocationChanged` and `AvailabilityChanged` events of each consultant, and signals `ConsultantChanged`, `ConsultantLocationChanged` and `ConsultantAvailabilityChanged`.[^1]

## References
