```rust run id=checkout
#[derive(Clone, Copy, PartialEq)]
enum Country {
    Us,
    Nl,
}

impl Country {
    fn code(self) -> &'static str {
        match self {
            Country::Us => "US",
            Country::Nl => "NL",
        }
    }
    fn postcode_length(self) -> usize {
        match self {
            Country::Us => 5,
            Country::Nl => 6,
        }
    }
}

/// What a widget tells the mediator. The widgets hold no reference to it.
enum Event {
    CountryChosen,
    PostcodeTyped,
    SubmitClicked,
}

struct Select {
    name: &'static str,
    value: Option<Country>,
}

impl Select {
    fn choose(&mut self, value: Country) -> Option<Event> {
        self.value = Some(value);
        println!("{} = \"{}\"", self.name, value.code());
        Some(Event::CountryChosen)
    }
}

struct TextField {
    name: &'static str,
    text: String,
}

impl TextField {
    /// A user edit: it returns an event for the mediator.
    fn type_text(&mut self, text: &str) -> Option<Event> {
        self.set_text(text);
        Some(Event::PostcodeTyped)
    }
    /// A program edit: no event.
    fn set_text(&mut self, text: &str) {
        self.text = text.to_string();
        println!("{} = \"{}\"", self.name, text);
    }
}

struct Button {
    name: &'static str,
    enabled: bool,
}

impl Button {
    fn set_enabled(&mut self, enabled: bool) {
        if enabled != self.enabled {
            self.enabled = enabled;
            println!("{} {}", self.name, if enabled { "enabled" } else { "disabled" });
        }
    }
    fn click(&self) -> Option<Event> {
        self.enabled.then_some(Event::SubmitClicked)
    }
}

/// The mediator. It owns the widgets, and all rules that join them are here.
struct CheckoutForm {
    country: Select,
    postcode: TextField,
    submit: Button,
}

impl CheckoutForm {
    fn new() -> Self {
        CheckoutForm {
            country: Select { name: "country", value: None },
            postcode: TextField { name: "postcode", text: String::new() },
            submit: Button { name: "submit", enabled: false },
        }
    }

    fn changed(&mut self, event: Event) {
        match event {
            Event::CountryChosen => {
                self.postcode.set_text("");
                self.submit.set_enabled(false);
            }
            Event::PostcodeTyped => {
                let valid = self
                    .country
                    .value
                    .is_some_and(|c| self.postcode.text.len() == c.postcode_length());
                self.submit.set_enabled(valid);
            }
            Event::SubmitClicked => {
                let code = self.country.value.map_or("", Country::code);
                println!("order sent to {} {}", code, self.postcode.text);
            }
        }
    }
}

/// The steps of the user, in order.
enum Action {
    Choose(Country),
    Type(&'static str),
    Click,
}

fn main() {
    let mut form = CheckoutForm::new();
    let actions = [
        Action::Choose(Country::Us),
        Action::Type("1234"),
        Action::Type("12345"),
        Action::Choose(Country::Nl),
        Action::Type("1234AB"),
        Action::Click,
    ];
    for action in actions {
        let event = match action {
            Action::Choose(country) => form.country.choose(country),
            Action::Type(text) => form.postcode.type_text(text),
            Action::Click => form.submit.click(),
        };
        if let Some(event) = event {
            form.changed(event);
        }
    }
}
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

- Verdict: idiom. The mediator owns the widgets, each widget method returns an `Event`, and the caller passes the event to `changed`. No widget holds a reference to the form.
- A widget that holds `&mut CheckoutForm` while the form owns the widget does not compile. Two `&mut` paths to one value break the borrow rules. `Rc<RefCell<...>>` and `Weak` back references can model the classic form, but the borrow is then checked at run time.
- `match` on the `Event` enum is exhaustive. An event kind added to the enum and not handled in `changed` is a compile error.
