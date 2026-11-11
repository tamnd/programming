---
tag: DPXJHB
kind: pattern
slug: mediator-pattern
name: Mediator pattern
aliases: [Mediator, director, coordinator, controller of a form]
area: software-design/behavioral-patterns
level: core
summary: >-
  Put the rules that join a group of objects into one object, the mediator. The objects in the group talk only to the mediator, so none of them needs a reference to the others.
family: behavioral-design
polarity: positive
intent: "Define an object that holds the way a set of objects interact, so that the objects do not refer to each other and the interaction can change on its own."
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with the widgets of a dialog box as the motivating example.
see_also: [observer-pattern, facade-pattern, command-pattern, chain-of-responsibility-pattern, state-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Mediator (Intent, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://martinfowler.com/eaaDev/EventAggregator.html
    title: Martin Fowler, Event Aggregator
    locator: How it Works; When to Use It
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, 1996
    locator: slide 10, Design Patterns in Dylan or Lisp
  - url: https://dom.spec.whatwg.org/#interface-eventtarget
    title: DOM Living Standard, WHATWG
    locator: 2.7 Interface EventTarget
  - url: https://docs.python.org/3.14/library/exceptions.html#RecursionError
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: RecursionError
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.desktop/javax/swing/ButtonGroup.html
    title: Java SE 25 API, javax.swing.ButtonGroup
    locator: class description
  - url: https://html.spec.whatwg.org/multipage/input.html#radio-button-state-%28type=radio%29
    title: HTML Living Standard, WHATWG
    locator: 4.10.5.1.16 Radio Button state (type=radio)
---

## Intent

Define an object that holds the way a set of objects interact. The objects then do not refer to each other, and the way they interact can change without a change to them.[^1] Design Patterns calls the objects in the set **colleagues**. Each colleague knows only the mediator. When something happens to it, it tells the mediator, and the mediator decides what the other colleagues must do.

## Problem

Design Patterns starts with a dialog box. A font dialog has a list of fonts, a text field, and buttons. The widgets depend on each other. A choice in the list changes the text in the field. A change in the field can change the choice in the list. Some buttons are enabled only when the field holds text.[^1]

The examples use a checkout form with three widgets: a country selector, a postcode field and a submit button. The rules are these. A change of country clears the postcode, because a postcode from one country is not valid in another, and it disables the submit button. A postcode is valid when its length is the length that the country needs: 5 for the US and 6 for the Netherlands in this example. The submit button is enabled only when the postcode is valid.

The direct design puts each rule in a widget. The country selector holds a reference to the postcode field and to the button, and the postcode field holds a reference to the selector and to the button. With `n` widgets that is up to `n(n-1)` references. Each widget then knows the form that it is in, and none of them can go into another form. A rule that joins three widgets is split across three classes, and a reader must open all three to see it.

The forces are these. The widgets must be reusable, so they must not know each other. The rules of one form must be in one place, so that a reader can see them and a change has one place to go. And the rules change more often than the widgets do.

## Solution

Design Patterns names three participants.[^1]

- **Mediator** defines the interface that colleagues use to talk to it. Often it is one method, such as `changed(colleague)`.
- **ConcreteMediator** knows its colleagues, and it holds the rules that join them. Here it is `CheckoutForm`.
- **Colleague** classes each know their mediator. A colleague tells the mediator when its state changes, and it does not talk to other colleagues. Here they are `Select`, `TextField` and `Button`.

```text
             before                                 after

   country <------> postcode               country    postcode    submit
      ^  \          /  ^                       \         |          /
      |   \        /   |                        v        v         v
      |    v      v    |                     CheckoutForm (mediator)
      +---> submit <---+                       all rules are here
```

The references go from each widget to the mediator, and from the mediator to each widget. The many-to-many links between widgets become one-to-many links between the mediator and the widgets.[^1]

The examples follow one sequence:

1. The user chooses the US. The form clears the postcode.
2. The user types `1234`. It is too short, so the button stays disabled.
3. The user types `12345`. The form enables the button.
4. The user chooses the Netherlands. The form clears the postcode and disables the button.
5. The user types `1234AB`, and the form enables the button.
6. The user clicks the button, and the form sends the order.

### Feedback loops

A mediator changes colleagues as a result of a change to a colleague. When each change to a colleague notifies the mediator, the mediator's own changes come back to it as events. Two fields that the mediator keeps equal show the problem. A change to the first sets the second, which notifies the mediator, which sets the first, and so on:

```python fails expect=exception:RecursionError id=echo
class Field:
    def __init__(self, name, form):
        self.name, self.form, self.text = name, form, ""

    def set_text(self, text):
        self.text = text
        self.form.changed(self)  # notifies on each set, also when the form sets it


class AddressForm:
    """Keeps the billing address the same as the shipping address."""

    def __init__(self):
        self.shipping = Field("shipping", self)
        self.billing = Field("billing", self)

    def changed(self, field):
        other = self.billing if field is self.shipping else self.shipping
        other.set_text(field.text)


form = AddressForm()
form.shipping.set_text("1 Main Street")
print(form.billing.text)
```

Python raises `RecursionError` when the interpreter passes the maximum recursion depth.[^5] Other languages overflow the stack or loop until the process stops. There are two fixes. A colleague notifies only when its value really changes. Or a colleague tells a change by the user apart from a change by the program, and only the first notifies. With the first fix:

```python run id=echo-fixed
class Field:
    def __init__(self, name, form):
        self.name, self.form, self.text = name, form, ""

    def set_text(self, text):
        if text == self.text:
            return  # no change, so no notification
        self.text = text
        self.form.changed(self)


class AddressForm:
    def __init__(self):
        self.shipping = Field("shipping", self)
        self.billing = Field("billing", self)

    def changed(self, field):
        other = self.billing if field is self.shipping else self.shipping
        other.set_text(field.text)


form = AddressForm()
form.shipping.set_text("1 Main Street")
print(form.billing.text)
```

```output for=echo-fixed
1 Main Street
```

The examples use both fixes. The postcode field has a `type` method for the user, which notifies, and a `set_text` method for the mediator, which does not. The button's `set_enabled` does nothing when the state does not change.

### A mediator built on observers

Design Patterns describes two ways for colleagues to tell the mediator about a change. A colleague can call a notification method of the mediator, as in the examples. Or the mediator can be an observer of each colleague.[^1] In the second form, the colleagues do not know the mediator at all. They raise events, and the mediator subscribes to them. In a browser, each DOM element is an `EventTarget`, an object to which events are dispatched and which keeps a list of event listeners.[^4] Node.js has the same class. Here the form observes two password fields:

```javascript run id=event-target
class Field extends EventTarget {
  #text = "";
  constructor(name) {
    super();
    this.name = name;
  }
  get text() {
    return this.#text;
  }
  type(text) {
    this.#text = text;
    console.log(`${this.name} = "${text}"`);
    this.dispatchEvent(new Event("change")); // the field does not know who listens
  }
}

class Button {
  enabled = false;
  constructor(name) {
    this.name = name;
  }
  setEnabled(enabled) {
    if (enabled !== this.enabled) {
      this.enabled = enabled;
      console.log(`${this.name} ${enabled ? "enabled" : "disabled"}`);
    }
  }
}

// The mediator observes the fields and holds the rule that joins them.
class SignupForm {
  password = new Field("password");
  confirm = new Field("confirm");
  submit = new Button("submit");

  constructor() {
    const check = () => {
      const { text } = this.password;
      this.submit.setEnabled(text.length >= 8 && text === this.confirm.text);
    };
    this.password.addEventListener("change", check);
    this.confirm.addEventListener("change", check);
  }
}

const form = new SignupForm();
form.password.type("correct horse");
form.confirm.type("correct");
form.confirm.type("correct horse");
form.password.type("short");
```

```output for=event-target
password = "correct horse"
confirm = "correct"
confirm = "correct horse"
submit enabled
password = "short"
submit disabled
```

This is the usual form in user interface toolkits. The widget is a subject in the sense of [[observer-pattern]], and the form is its observer. What makes the form a mediator is not the way it receives the events. It is that the form holds the rules that join several widgets, and that it changes the other widgets in reply.

## Example

Each example file builds the checkout form and runs the six steps above.

| Language | Colleague to mediator | Mediator to colleague |
|---|---|---|
| Python | `self.mediator.changed(self)` | method calls on attributes |
| JavaScript | `this.mediator.changed(this)` | method calls on fields |
| TypeScript | a `Mediator` interface | method calls on `readonly` fields |
| Java | a `Mediator` interface in a `final` field | method calls on `final` fields |
| C# | an `IMediator` in a primary constructor | method calls on properties |
| Go | a `Mediator` interface in an unexported field | method calls on struct fields |
| Rust | each widget method returns an `Event`; no back reference | the mediator owns the widgets |
| C++ | a `Mediator&` member | method calls on member objects |

## Consequences

Design Patterns lists five consequences.[^1]

- **It limits subclassing.** The behaviour that would be spread across the colleagues is in the mediator. A change of behaviour needs a subclass of the mediator, or a different mediator, and the colleague classes stay as they are.
- **It decouples colleagues.** Colleague classes can change and be reused on their own.
- **It simplifies object protocols.** One-to-many links between a mediator and its colleagues replace many-to-many links between colleagues. One-to-many links are less work to understand, maintain and extend.
- **It abstracts how objects cooperate.** The way objects interact becomes a concept of its own, in an object of its own.
- **It centralizes control.** The complexity of the interaction moves into the mediator. Design Patterns warns that this can make the mediator more complex than any of the colleagues, and a monolith that is hard to maintain.

The last point is the main liability. A mediator for a large dialog collects every rule of the dialog. It knows the concrete type of each widget, and each rule added to it makes it larger. A mediator that grows this way is a god object, and splitting it means finding groups of widgets that interact mostly with each other and giving each group its own mediator.

The links between the mediator and its colleagues form a cycle. In a language with garbage collection, that is no problem. In Rust and in C++ it needs a decision about ownership. The Rust example gives the mediator ownership of the widgets and lets the widgets return events in place of a back reference.

## When not to use it

Do not use it for two objects. A mediator between two colleagues is a third class with no gain. A direct reference, or one observer, is clearer.

Do not use it when the objects do not interact with each other. Objects that only report to one consumer need an observer or a callback. The mediator is for rules that read one colleague and change another.

Do not use a global event bus as a mediator by default. A bus removes the references between objects, but it also removes the place where a reader can see the rules. Each subscriber holds a part of the logic, and the coupling is still there, through the names of the events. Fowler's Event Aggregator is a single source of events for many objects, and it fits the case where many objects want to hear about many sources.[^2] It does not hold the rules, so it is not a mediator in the sense of Design Patterns.

## Variants and related patterns

**Observer.** Observer sends one subject's changes to any number of observers, and the subject does not know what they do. A mediator holds the rules between several objects and acts on them. Design Patterns says that colleagues can talk to the mediator through Observer.[^1] Peter Norvig groups Mediator and Observer together, as patterns that method combination makes invisible or simpler in a dynamic language such as Lisp or Dylan.[^3] See [[observer-pattern]].

**Facade.** Design Patterns compares the two.[^1] A facade gives a more convenient interface to a subsystem, and its protocol goes one way: the facade calls the subsystem, and the subsystem does not know the facade. A mediator's colleagues know the mediator and call it, so its protocol goes both ways. See [[facade-pattern]].

**Event aggregator and event bus.** An event aggregator collects the events of many objects and gives them to subscribers in one place.[^2] It removes references between objects as a mediator does, but it moves no rules. A mediator can be built on top of one: the mediator subscribes to the bus, and the bus delivers the events.

**Command.** Colleagues can send commands to the mediator in place of method calls, and the mediator can run, log or queue them. See [[command-pattern]].

**Chain of Responsibility.** A chain passes a request along a list of handlers, and each one decides whether to handle it. A mediator receives the request and decides itself who acts. See [[chain-of-responsibility-pattern]].

**Controllers and view models.** The controller of a form in a model-view-controller design and the view model of a form often play the role of a mediator. They hold the rules that join the widgets of one view.

## Known uses

- Swing `ButtonGroup`: a set of buttons with the same `ButtonGroup` forms a multiple-exclusion scope. Turning one button on turns off all the others in the group.[^6] No button knows the others.
- HTML radio buttons: inputs of type radio in the same form with the same `name` are one radio button group. When one becomes checked, the browser sets all the others in the group to unchecked.[^7]
- DOM event handling: a form script that adds event listeners to its fields and changes other fields in reply is a mediator built on `EventTarget`.[^4]

## References
