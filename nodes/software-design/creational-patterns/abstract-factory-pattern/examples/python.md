```python run id=widgets
from abc import ABC, abstractmethod


class Button(ABC):
    @abstractmethod
    def render(self) -> str: ...


class Checkbox(ABC):
    @abstractmethod
    def render(self) -> str: ...


class WidgetFactory(ABC):
    @abstractmethod
    def button(self, label: str) -> Button: ...

    @abstractmethod
    def checkbox(self, label: str, checked: bool) -> Checkbox: ...


class PlainButton(Button):
    def __init__(self, label: str) -> None:
        self.label = label

    def render(self) -> str:
        return f"[ {self.label} ]"


class PlainCheckbox(Checkbox):
    def __init__(self, label: str, checked: bool) -> None:
        self.label, self.checked = label, checked

    def render(self) -> str:
        return f"[{'x' if self.checked else ' '}] {self.label}"


class RoundButton(Button):
    def __init__(self, label: str) -> None:
        self.label = label

    def render(self) -> str:
        return f"( {self.label} )"


class RoundCheckbox(Checkbox):
    def __init__(self, label: str, checked: bool) -> None:
        self.label, self.checked = label, checked

    def render(self) -> str:
        return f"({'*' if self.checked else ' '}) {self.label}"


class PlainFactory(WidgetFactory):
    def button(self, label):
        return PlainButton(label)

    def checkbox(self, label, checked):
        return PlainCheckbox(label, checked)


class RoundFactory(WidgetFactory):
    def button(self, label):
        return RoundButton(label)

    def checkbox(self, label, checked):
        return RoundCheckbox(label, checked)


def login_form(widgets: WidgetFactory) -> str:
    """The client. It names no concrete widget class."""
    return " ".join([widgets.button("Save").render(), widgets.checkbox("Remember me", True).render()])


for name, factory in [("plain", PlainFactory()), ("round", RoundFactory())]:
    print(f"{name}: {login_form(factory)}")
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. Abstract base classes give the classic form. `abc.abstractmethod` stops a concrete factory from being created until it has every factory method.
- A class is a callable object in Python, so a concrete factory can be any object whose attributes are classes, such as a module or a `types.SimpleNamespace(button=PlainButton, checkbox=PlainCheckbox)`. The node shows this variant.
- The DB-API of PEP 249 has the same shape at the level of modules. Each driver module has a `connect` constructor, and the connection makes cursors of the same driver.
