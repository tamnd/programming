```python run id=checkout
from abc import ABC, abstractmethod


class Mediator(ABC):
    @abstractmethod
    def changed(self, widget: "Widget") -> None: ...


class Widget:
    """A colleague. It knows its mediator and no other widget."""

    def __init__(self, name: str, mediator: Mediator):
        self.name = name
        self.mediator = mediator


class Select(Widget):
    def __init__(self, name: str, mediator: Mediator):
        super().__init__(name, mediator)
        self.value = ""

    def choose(self, value: str) -> None:  # the user picks a value
        self.value = value
        print(f'{self.name} = "{value}"')
        self.mediator.changed(self)


class TextField(Widget):
    def __init__(self, name: str, mediator: Mediator):
        super().__init__(name, mediator)
        self.text = ""

    def type(self, text: str) -> None:  # the user edits the text
        self.set_text(text)
        self.mediator.changed(self)

    def set_text(self, text: str) -> None:  # the program sets the text: no notification
        self.text = text
        print(f'{self.name} = "{text}"')


class Button(Widget):
    def __init__(self, name: str, mediator: Mediator):
        super().__init__(name, mediator)
        self.enabled = False

    def set_enabled(self, enabled: bool) -> None:
        if enabled != self.enabled:
            self.enabled = enabled
            print(f"{self.name} {'enabled' if enabled else 'disabled'}")

    def click(self) -> None:
        if self.enabled:
            self.mediator.changed(self)


class CheckoutForm(Mediator):
    """The mediator. All rules that join the widgets are here."""

    POSTCODE_LENGTH = {"US": 5, "NL": 6}

    def __init__(self):
        self.country = Select("country", self)
        self.postcode = TextField("postcode", self)
        self.submit = Button("submit", self)

    def changed(self, widget: Widget) -> None:
        if widget is self.country:
            self.postcode.set_text("")
            self.submit.set_enabled(False)
        elif widget is self.postcode:
            needed = self.POSTCODE_LENGTH.get(self.country.value)
            self.submit.set_enabled(len(self.postcode.text) == needed)
        elif widget is self.submit:
            print(f"order sent to {self.country.value} {self.postcode.text}")


form = CheckoutForm()
form.country.choose("US")
form.postcode.type("1234")
form.postcode.type("12345")
form.country.choose("NL")
form.postcode.type("1234AB")
form.submit.click()
```

```output for=checkout
country = "US"
postcode = ""
postcode = "1234"
postcode = "12345"
submit enabled
country = "NL"
postcode = ""
submit disabled
postcode = "1234AB"
submit enabled
order sent to NL 1234AB
```

- Verdict: idiom. Each widget holds a reference to its mediator and calls `changed(self)`. The form tests which widget changed and applies the rules.
- The widgets are reusable. `TextField` and `Button` have no code about countries or postcodes, so another form can use them with other rules.
- `type` notifies the mediator and `set_text` does not. The mediator calls `set_text`, so its own changes do not come back to it as events. The node page shows the loop that this split prevents.
- In Python a mediator can also be a set of callbacks: `field.on_change = form.postcode_changed`. A bound method is a first-class value, so the widget needs no `Mediator` base class.
