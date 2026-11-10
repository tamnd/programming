```rust run id=widgets
trait Widget {
    fn render(&self) -> String;
}

trait WidgetFactory {
    type Button: Widget;
    type Checkbox: Widget;
    fn button(&self, label: &str) -> Self::Button;
    fn checkbox(&self, label: &str, checked: bool) -> Self::Checkbox;
}

struct PlainButton(String);
struct PlainCheckbox(String, bool);
struct RoundButton(String);
struct RoundCheckbox(String, bool);

impl Widget for PlainButton {
    fn render(&self) -> String {
        format!("[ {} ]", self.0)
    }
}
impl Widget for PlainCheckbox {
    fn render(&self) -> String {
        format!("[{}] {}", if self.1 { "x" } else { " " }, self.0)
    }
}
impl Widget for RoundButton {
    fn render(&self) -> String {
        format!("( {} )", self.0)
    }
}
impl Widget for RoundCheckbox {
    fn render(&self) -> String {
        format!("({}) {}", if self.1 { "*" } else { " " }, self.0)
    }
}

struct Plain;
struct Round;

impl WidgetFactory for Plain {
    type Button = PlainButton;
    type Checkbox = PlainCheckbox;
    fn button(&self, label: &str) -> PlainButton {
        PlainButton(label.into())
    }
    fn checkbox(&self, label: &str, checked: bool) -> PlainCheckbox {
        PlainCheckbox(label.into(), checked)
    }
}

impl WidgetFactory for Round {
    type Button = RoundButton;
    type Checkbox = RoundCheckbox;
    fn button(&self, label: &str) -> RoundButton {
        RoundButton(label.into())
    }
    fn checkbox(&self, label: &str, checked: bool) -> RoundCheckbox {
        RoundCheckbox(label.into(), checked)
    }
}

// Generic over one factory type F: every widget comes from the same family.
fn login_form<F: WidgetFactory>(widgets: &F) -> String {
    format!("{} {}", widgets.button("Save").render(), widgets.checkbox("Remember me", true).render())
}

fn main() {
    println!("plain: {}", login_form(&Plain));
    println!("round: {}", login_form(&Round));
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. A trait with associated types names the product types of each family. Each `impl` fixes them once, so `F::Button` and `F::Checkbox` always belong to the same `F`.
- `login_form` is generic, so the compiler makes one copy for each factory and the calls are static. A `&dyn` factory would need the products boxed as `Box<dyn Widget>`, with the family chosen at run time.
- The concrete factories are unit structs with no data. The factory is a type more than a value, which is close to Norvig's point about first-class types (see the node).
