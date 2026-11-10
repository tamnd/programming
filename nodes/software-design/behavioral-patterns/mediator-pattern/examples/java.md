```java run id=checkout
import java.util.Map;

public class Main {
    interface Mediator {
        void changed(Widget widget);
    }

    /** A colleague. It knows its mediator and no other widget. */
    abstract static class Widget {
        final String name;
        final Mediator mediator;

        Widget(String name, Mediator mediator) {
            this.name = name;
            this.mediator = mediator;
        }
    }

    static final class Select extends Widget {
        String value = "";

        Select(String name, Mediator mediator) { super(name, mediator); }

        void choose(String value) {
            this.value = value;
            System.out.println(name + " = \"" + value + "\"");
            mediator.changed(this);
        }
    }

    static final class TextField extends Widget {
        String text = "";

        TextField(String name, Mediator mediator) { super(name, mediator); }

        void type(String text) { // the user edits the text
            setText(text);
            mediator.changed(this);
        }

        void setText(String text) { // the program sets the text: no notification
            this.text = text;
            System.out.println(name + " = \"" + text + "\"");
        }
    }

    static final class Button extends Widget {
        boolean enabled = false;

        Button(String name, Mediator mediator) { super(name, mediator); }

        void setEnabled(boolean enabled) {
            if (enabled != this.enabled) {
                this.enabled = enabled;
                System.out.println(name + (enabled ? " enabled" : " disabled"));
            }
        }

        void click() {
            if (enabled) mediator.changed(this);
        }
    }

    /** The mediator. All rules that join the widgets are here. */
    static final class CheckoutForm implements Mediator {
        private static final Map<String, Integer> POSTCODE_LENGTH = Map.of("US", 5, "NL", 6);
        final Select country = new Select("country", this);
        final TextField postcode = new TextField("postcode", this);
        final Button submit = new Button("submit", this);

        @Override
        public void changed(Widget widget) {
            if (widget == country) {
                postcode.setText("");
                submit.setEnabled(false);
            } else if (widget == postcode) {
                Integer needed = POSTCODE_LENGTH.get(country.value);
                submit.setEnabled(needed != null && postcode.text.length() == needed);
            } else if (widget == submit) {
                System.out.println("order sent to " + country.value + " " + postcode.text);
            }
        }
    }

    public static void main(String[] args) {
        CheckoutForm form = new CheckoutForm();
        form.country.choose("US");
        form.postcode.type("1234");
        form.postcode.type("12345");
        form.country.choose("NL");
        form.postcode.type("1234AB");
        form.submit.click();
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

- Verdict: idiom. A `Mediator` interface with one method, widgets that hold it in a `final` field, and a form that implements it.
- The form passes `this` to the widgets in its field initializers. This lets `this` escape before the constructor ends. It is safe here because no widget calls the mediator during construction.
- Swing's `ButtonGroup` is a small mediator from the JDK. Turning on one button in a group turns off the others, and no button knows the others.
- `needed != null` guards the lookup. A country with no entry leaves the button disabled, and it does not throw a `NullPointerException` on the unboxing of `needed`.
