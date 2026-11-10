```typescript run id=checkout
type Country = "US" | "NL";

interface Mediator {
  changed(widget: Widget): void;
}

// A colleague. It knows its mediator and no other widget.
abstract class Widget {
  readonly name: string;
  protected readonly mediator: Mediator;
  constructor(name: string, mediator: Mediator) {
    this.name = name;
    this.mediator = mediator;
  }
}

class Select<T extends string> extends Widget {
  value: T | undefined;
  choose(value: T): void {
    this.value = value;
    console.log(`${this.name} = "${value}"`);
    this.mediator.changed(this);
  }
}

class TextField extends Widget {
  text = "";
  type(text: string): void {
    this.setText(text); // the user edits the text
    this.mediator.changed(this);
  }
  setText(text: string): void {
    this.text = text; // the program sets the text: no notification
    console.log(`${this.name} = "${text}"`);
  }
}

class Button extends Widget {
  enabled = false;
  setEnabled(enabled: boolean): void {
    if (enabled !== this.enabled) {
      this.enabled = enabled;
      console.log(`${this.name} ${enabled ? "enabled" : "disabled"}`);
    }
  }
  click(): void {
    if (this.enabled) this.mediator.changed(this);
  }
}

// The mediator. All rules that join the widgets are here.
class CheckoutForm implements Mediator {
  static readonly postcodeLength: Readonly<Record<Country, number>> = { US: 5, NL: 6 };
  readonly country = new Select<Country>("country", this);
  readonly postcode = new TextField("postcode", this);
  readonly submit = new Button("submit", this);

  changed(widget: Widget): void {
    if (widget === this.country) {
      this.postcode.setText("");
      this.submit.setEnabled(false);
    } else if (widget === this.postcode && this.country.value !== undefined) {
      const needed = CheckoutForm.postcodeLength[this.country.value];
      this.submit.setEnabled(this.postcode.text.length === needed);
    } else if (widget === this.submit) {
      console.log(`order sent to ${this.country.value} ${this.postcode.text}`);
    }
  }
}

const form = new CheckoutForm();
form.country.choose("US");
form.postcode.type("1234");
form.postcode.type("12345");
form.country.choose("NL");
form.postcode.type("1234AB");
form.submit.click();
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

- Verdict: idiom. A `Mediator` interface, an abstract `Widget` that holds it, and a form that implements it.
- `Select<Country>` lets the compiler reject `form.country.choose("FR")`, and `Record<Country, number>` makes the compiler check that each country has a postcode length.
- `changed` takes a `Widget` and tests it with `===`. A discriminated union of events, such as `{ kind: "country"; value: Country }`, would let the compiler check that the form handles each kind. The Rust example takes that approach.
