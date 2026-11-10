```javascript run id=checkout
// The colleagues. Each widget knows its mediator and no other widget.
class Select {
  value = "";
  constructor(name, mediator) {
    this.name = name;
    this.mediator = mediator;
  }
  choose(value) {
    this.value = value;
    console.log(`${this.name} = "${value}"`);
    this.mediator.changed(this);
  }
}

class TextField {
  text = "";
  constructor(name, mediator) {
    this.name = name;
    this.mediator = mediator;
  }
  type(text) {
    this.setText(text); // the user edits the text
    this.mediator.changed(this);
  }
  setText(text) {
    this.text = text; // the program sets the text: no notification
    console.log(`${this.name} = "${text}"`);
  }
}

class Button {
  enabled = false;
  constructor(name, mediator) {
    this.name = name;
    this.mediator = mediator;
  }
  setEnabled(enabled) {
    if (enabled !== this.enabled) {
      this.enabled = enabled;
      console.log(`${this.name} ${enabled ? "enabled" : "disabled"}`);
    }
  }
  click() {
    if (this.enabled) this.mediator.changed(this);
  }
}

// The mediator. All rules that join the widgets are here.
class CheckoutForm {
  static postcodeLength = { US: 5, NL: 6 };
  country = new Select("country", this);
  postcode = new TextField("postcode", this);
  submit = new Button("submit", this);

  changed(widget) {
    if (widget === this.country) {
      this.postcode.setText("");
      this.submit.setEnabled(false);
    } else if (widget === this.postcode) {
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

- Verdict: idiom. The widgets call `mediator.changed(this)`, and the form holds all the rules.
- Class fields run in order, and `this` in a field initializer is the instance under construction, so the form can pass itself to each widget as it makes it.
- In a browser the DOM gives the other form. Each element is an `EventTarget`, and the form adds listeners to the widgets. The widgets then know nothing of the mediator, and the mediator is an observer of each widget. The node page has that version.
