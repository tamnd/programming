---
tag: M5W08S
kind: pattern
slug: state-pattern
name: State pattern
aliases: [State, Objects for States, state object, state machine]
area: software-design/behavioral-patterns
level: core
summary: >-
  An object keeps its current state in a separate state object and passes each request to it. A change of state replaces that object, so the object behaves as if its class had changed.
family: behavioral-design
polarity: positive
intent: "Let an object change its behaviour when its internal state changes, so that the object appears to change its class."
also_known_as: [Objects for States]
refactorings: [replace-state-altering-conditionals-with-state, replace-conditional-with-polymorphism]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Objects for States as another name, and a TCP connection as the motivating example.
see_also: [strategy-pattern, singleton-pattern, command-pattern, python/match-statement, closures]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, State (Intent, Also Known As, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (State is one of the six patterns affected by first-class types)
  - url: https://www.rfc-editor.org/rfc/rfc9293.html#section-3.3.2
    title: RFC 9293, Transmission Control Protocol (TCP), IETF, 2022
    locator: section 3.3.2, State Machine Overview
  - url: https://doc.rust-lang.org/book/ch18-03-oo-design-patterns.html
    title: The Rust Programming Language, chapter 18.3, Implementing an Object-Oriented Design Pattern
    locator: sections on the trait object design, its trade-offs, the note on enums, and encoding states and behaviour as types
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
    title: The Rust Programming Language, chapter 6.2, The match Control Flow Construct
    locator: Matches Are Exhaustive
  - url: https://github.com/golang/go/blob/go1.27.3/src/text/template/parse/lex.go#L109-L110
    title: Go go1.27.3 source, src/text/template/parse/lex.go
    locator: type stateFn, lines 109 to 110
  - url: https://github.com/golang/go/blob/go1.27.3/src/text/template/parse/lex.go#L226-L238
    title: Go go1.27.3 source, src/text/template/parse/lex.go
    locator: nextItem, lines 226 to 238
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.9.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.9.1 Enum Constants
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 7, Simplification, Replace State-Altering Conditionals with State
  - url: https://refactoring.com/catalog/replaceConditionalWithPolymorphism.html
    title: Martin Fowler, Refactoring catalog, Replace Conditional with Polymorphism
    locator: catalog entry
---

## Intent

Let an object change its behaviour when its internal state changes. To its clients, the object appears to change its class.[^1] The object is called the context. It keeps a reference to a state object and passes each request to it. A change of state replaces the state object.

## Problem

A TCP connection reacts to the same request in different ways. An `open` on a closed connection starts a handshake. An `open` on an established connection is an error. Design Patterns starts from this example.[^1] The TCP specification lists the states of a connection, from LISTEN and SYN-SENT to TIME-WAIT, and a fictional state CLOSED.[^3]

The first design keeps the state in a field and puts a conditional in each operation. Every method of the connection then starts with the same `if` or `switch` on the field. The code for one state is spread over all the methods. To add a state, a programmer must find and change each of them. Design Patterns names this case: operations with large conditional statements that depend on the state of the object, often held in enumerated constants.[^1]

The forces are these. The behaviour for one state should be in one place. The transitions should be visible, not hidden in assignments to a field. And the code that uses the object should not change when a state is added.

## Solution

Design Patterns names three participants.[^1]

- **Context** is the object that clients use. It holds the current state object and passes each request to it.
- **State** declares one operation for each request that depends on the state.
- **ConcreteState** implements the behaviour for one state.

A request to the context goes to the current state. The state does its work, and then it either tells the context the next state or returns it. The context does not test the state. The class of the current state object selects the behaviour.

The examples follow a media player with three states and three events. Each line shows the event, the action and the transition:

```python run id=enum-match
from enum import Enum


class State(Enum):
    STOPPED = "stopped"
    PLAYING = "playing"
    PAUSED = "paused"


def step(state, event):
    match state, event:
        case State.STOPPED, "play":
            return State.PLAYING, "start at 0:00"
        case State.PLAYING, "pause":
            return State.PAUSED, "hold the position"
        case State.PAUSED, "play":
            return State.PLAYING, "resume"
        case State.PLAYING | State.PAUSED, "stop":
            return State.STOPPED, "rewind"
        case _:
            return state, "ignored"


state = State.STOPPED
for event in ["play", "pause", "play", "stop", "pause"]:
    after, action = step(state, event)
    print(f"{event}: {action} ({state.value} -> {after.value})")
    state = after
```

```output for=enum-match
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

This block is not the State pattern. It is an enum and one [[python/match-statement|match statement]], and it is the form that the pattern replaces. The Python example file has the same machine with a class for each state. Both forms are correct. The choice depends on what changes more often.

### State, an enum and a match

| Question | State objects | Enum and match |
|---|---|---|
| Where is the code for one state | in one class | in one arm of each match |
| Where are the transitions | spread over the state classes | in one function, as a table |
| Add a state | add one class | add arms to each match |
| Add an event | add a method to each state class | add arms to one function |
| Can the compiler find a missing case | yes, when the event methods are abstract | yes, when the match must be exhaustive |
| Can a state hold its own data | yes, in its fields | yes, in an enum variant with fields |

An enum and a match fit a machine with few states, small actions and many readers of the whole table. State objects fit a machine where each state has much code of its own. The Rust book says that an enum design needs a `match` in each place that checks the state, and that its trait object design avoids this repetition.[^4]

In Rust, a `match` must cover every value. When the enum gets a variant, each `match` without a wildcard arm stops compiling.[^5] This program adds `Buffering` and forgets its arms:

```rust fails expect=compile id=missing-arm
#[derive(Clone, Copy)]
enum State {
    Stopped,
    Playing,
    Paused,
    Buffering,
}

fn name(state: State) -> &'static str {
    match state {
        State::Stopped => "stopped",
        State::Playing => "playing",
        State::Paused => "paused",
    }
}

fn main() {
    println!("{}", name(State::Buffering));
}
```

A wildcard arm `_` turns off this check. The Rust example file lists every pair of state and event for this reason.

### State and Strategy

The two patterns have the same class diagram. A context holds an object behind an interface and calls it. The difference is who changes the object and how often.

- A client chooses a strategy, usually once, and the strategies do not know each other.
- The states change the context's state object while the program runs. A state names the next state, so the state classes depend on each other.

The [[strategy-pattern]] page covers strategies. A rule of thumb: if the object is replaced from inside, as a result of its own requests, it is a state.

### Who defines the transitions

Design Patterns gives two choices.[^1] The context can hold the transitions, so that the states do not know each other. Or each state can return or set its successor. The second is more flexible, and it is the form that the example files use. It also makes each state class depend on the others. A third choice is a table that maps a state and an event to the next state. The TypeScript example uses a table. Design Patterns credits this form to Cargill. It says that a table lets a programmer change the transitions by changing data and not code. It also says that a lookup is often slower than a function call, that the uniform table makes the transition criteria harder to understand, and that it is hard to add actions to the transitions.[^1]

### Typestate

In a language with generics, each state can be a type. A method that is wrong for a state does not exist on that type, so the wrong call is a compile error. The Rust book calls this encoding states and behaviour as types.[^4] Each method consumes the player and returns the player in the next state:

```rust fails expect=compile id=typestate
use std::marker::PhantomData;

struct Stopped;
struct Playing;
struct Paused;

struct Player<S> {
    position: u32,
    state: PhantomData<S>,
}

impl Player<Stopped> {
    fn new() -> Self {
        Player { position: 0, state: PhantomData }
    }
    fn play(self) -> Player<Playing> {
        Player { position: 0, state: PhantomData }
    }
}

impl Player<Playing> {
    fn pause(self) -> Player<Paused> {
        Player { position: self.position, state: PhantomData }
    }
}

fn main() {
    let stopped = Player::new();
    let playing = stopped.play();
    let _paused = playing.pause();
    let again = Player::new();
    again.pause(); // a stopped player has no pause method
}
```

The cost is that the state is no longer a value. A program that reads events from a user does not know the next state at compile time. It needs an enum of the typestates, and so it needs a match again.

## Example

All the example files run one script: play, pause, play, stop, and then pause on a stopped player, which is ignored.

| Language | Form |
|---|---|
| Python | a class for each state; one shared object of each class |
| JavaScript | an object for each state, with a default prototype |
| TypeScript | a transition table typed by unions of string literals |
| Java | an enum whose constants have their own method bodies |
| C# | an enum and a `switch` expression on the pair of state and event |
| Go | an empty struct for each state and an embedded struct of defaults |
| Rust | an enum and a `match` that lists every pair |
| C++ | an abstract class, a subclass for each state, and `const` global state objects |
| Bash | a `case` on the string `"state:event"` |

## Consequences

Design Patterns lists three consequences.[^1] The pattern puts the behaviour for one state in one class, and it separates the behaviour of different states. It makes the transitions explicit: a change of state is one assignment of the state object, so the context cannot be in a state between two states. And a state object with no fields can be shared by all contexts.

The costs are many small classes and coupling between them. Design Patterns says that the pattern increases the number of classes, and that this is good when there are many states.[^1] The Rust book names two downsides of its state objects. The states are coupled to each other, so a state added between two states changes the code of one of them. And some logic is duplicated across the states.[^4]

**Do not put the data of one context in a shared state object.** The examples share one object for each state, as Design Patterns suggests for states without fields.[^1] A field on a shared state is shared by every context:

```python wrong id=shared-data
class Playing:
    def __init__(self):
        self.position = 0

    def listen(self, seconds):
        self.position += seconds


PLAYING = Playing()  # one object for every player


class Player:
    def __init__(self, title):
        self.title = title
        self.state = PLAYING

    def listen(self, seconds):
        self.state.listen(seconds)


song, podcast = Player("song"), Player("podcast")
song.listen(20)
podcast.listen(15)
print(f"song at {song.state.position} s, podcast at {podcast.state.position} s")
```

```output for=shared-data
song at 35 s, podcast at 35 s
```

Each player should be at its own position. There are two fixes. Keep the position in the context and pass the context to the state. Or make a state object for each context when the state is entered. Design Patterns discusses the second choice as the question of when to create and destroy state objects.[^1]

## When not to use it

Do not use the pattern for two or three states with one line of code each. A field and a conditional are shorter, and the reader sees all of the behaviour in one place.

Do not use state classes when the transitions are the main thing to read and test. A table, or an enum with a match, shows the whole machine in one function. A test can call that function with a state and an event, and no context object.

Do not use state classes when a state machine is generated or loaded as data. Parsers and protocol engines often hold their machine as a table that a tool makes from a grammar or a specification.

## Variants and related patterns

**Functions as states.** A state can be a function that does the work and returns the next state. Norvig lists State among the six patterns that first-class types make invisible or less complex.[^2] The lexer of Go's `text/template` declares `type stateFn func(*lexer) stateFn`.[^6] Its loop is `state = state(l)`, and it stops when a state returns `nil`.[^7] See [[closures]].

**Enums with behaviour.** In Java, an enum constant can have a class body that overrides the methods of the enum, so each constant is a state object.[^8] The Java example uses this.

**Flyweight and Singleton.** Design Patterns relates State to Flyweight, for sharing state objects, and notes that state objects are often singletons.[^1] See [[singleton-pattern]]. A module constant, an enum constant or a `const` global does this without a singleton class.

**Strategy.** The section above compares them. See [[strategy-pattern]].

**Command.** A state machine often receives its events as command objects, and a command can be the action of a transition. See [[command-pattern]].

**Refactorings.** Kerievsky's Replace State-Altering Conditionals with State moves the conditionals that change a state field into state classes.[^9] Fowler's Replace Conditional with Polymorphism is the general form.[^10]

## Known uses

- Go: the template lexer in [src/text/template/parse/lex.go at go1.27.3](https://github.com/golang/go/blob/go1.27.3/src/text/template/parse/lex.go#L109-L110) holds each state as a function of type `stateFn`, and [nextItem](https://github.com/golang/go/blob/go1.27.3/src/text/template/parse/lex.go#L226-L238) runs states until one returns `nil`.[^6][^7]
- TCP: the connection of RFC 9293 is a state machine with eleven states, and it is the motivating example of Design Patterns.[^3][^1]

## References
