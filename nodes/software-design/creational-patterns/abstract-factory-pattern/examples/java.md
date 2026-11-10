```java run id=widgets
import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
    interface Widget {
        String render();
    }

    interface WidgetFactory {
        Widget button(String label);

        Widget checkbox(String label, boolean checked);
    }

    record PlainButton(String label) implements Widget {
        public String render() {
            return "[ " + label + " ]";
        }
    }

    record PlainCheckbox(String label, boolean checked) implements Widget {
        public String render() {
            return "[" + (checked ? "x" : " ") + "] " + label;
        }
    }

    record RoundButton(String label) implements Widget {
        public String render() {
            return "( " + label + " )";
        }
    }

    record RoundCheckbox(String label, boolean checked) implements Widget {
        public String render() {
            return "(" + (checked ? "*" : " ") + ") " + label;
        }
    }

    static final class PlainFactory implements WidgetFactory {
        public Widget button(String label) {
            return new PlainButton(label);
        }

        public Widget checkbox(String label, boolean checked) {
            return new PlainCheckbox(label, checked);
        }
    }

    static final class RoundFactory implements WidgetFactory {
        public Widget button(String label) {
            return new RoundButton(label);
        }

        public Widget checkbox(String label, boolean checked) {
            return new RoundCheckbox(label, checked);
        }
    }

    static String loginForm(WidgetFactory widgets) {
        return widgets.button("Save").render() + " " + widgets.checkbox("Remember me", true).render();
    }

    public static void main(String[] args) {
        Map<String, WidgetFactory> kits = new LinkedHashMap<>();
        kits.put("plain", new PlainFactory());
        kits.put("round", new RoundFactory());
        kits.forEach((name, kit) -> System.out.println(name + ": " + loginForm(kit)));
    }
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. The classic form: an interface for the factory, an interface for the products, and a class for each concrete product and each concrete factory.
- The client `loginForm` depends only on `WidgetFactory` and `Widget`. The program chooses the family once, when it puts a factory in the map.
- A `default` method in the factory interface can give a fallback for a product that is added later, so the existing factories still compile. The .NET class `DbProviderFactory` does the same with virtual methods (see the node).
