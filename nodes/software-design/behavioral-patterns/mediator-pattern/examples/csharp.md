```csharp run id=checkout
using System;
using System.Collections.Generic;

var form = new CheckoutForm();
form.Country.Choose("US");
form.Postcode.Type("1234");
form.Postcode.Type("12345");
form.Country.Choose("NL");
form.Postcode.Type("1234AB");
form.Submit.Click();

interface IMediator
{
    void Changed(Widget widget);
}

// A colleague. It knows its mediator and no other widget.
abstract class Widget(string name, IMediator mediator)
{
    public string Name { get; } = name;
    protected IMediator Mediator { get; } = mediator;
}

sealed class Select(string name, IMediator mediator) : Widget(name, mediator)
{
    public string Value { get; private set; } = "";

    public void Choose(string value)
    {
        Value = value;
        Console.WriteLine($"{Name} = \"{value}\"");
        Mediator.Changed(this);
    }
}

sealed class TextField(string name, IMediator mediator) : Widget(name, mediator)
{
    public string Text { get; private set; } = "";

    public void Type(string text) // the user edits the text
    {
        SetText(text);
        Mediator.Changed(this);
    }

    public void SetText(string text) // the program sets the text: no notification
    {
        Text = text;
        Console.WriteLine($"{Name} = \"{text}\"");
    }
}

sealed class Button(string name, IMediator mediator) : Widget(name, mediator)
{
    public bool Enabled { get; private set; }

    public void SetEnabled(bool enabled)
    {
        if (enabled == Enabled) return;
        Enabled = enabled;
        Console.WriteLine($"{Name} {(enabled ? "enabled" : "disabled")}");
    }

    public void Click()
    {
        if (Enabled) Mediator.Changed(this);
    }
}

// The mediator. All rules that join the widgets are here.
sealed class CheckoutForm : IMediator
{
    private static readonly Dictionary<string, int> PostcodeLength = new() { ["US"] = 5, ["NL"] = 6 };

    public Select Country { get; }
    public TextField Postcode { get; }
    public Button Submit { get; }

    public CheckoutForm()
    {
        Country = new Select("country", this);
        Postcode = new TextField("postcode", this);
        Submit = new Button("submit", this);
    }

    public void Changed(Widget widget)
    {
        if (widget == Country)
        {
            Postcode.SetText("");
            Submit.SetEnabled(false);
        }
        else if (widget == Postcode)
        {
            Submit.SetEnabled(PostcodeLength.TryGetValue(Country.Value, out int needed) && Postcode.Text.Length == needed);
        }
        else if (widget == Submit)
        {
            Console.WriteLine($"order sent to {Country.Value} {Postcode.Text}");
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

- Verdict: idiom. An `IMediator` interface, widgets with primary constructors that take it, and a form that implements it.
- The form makes its widgets in its constructor, because a field initializer in C# cannot refer to `this`.
- The usual .NET form of a mediator is events. Each widget exposes an `event EventHandler Changed`, and the form subscribes with `+=`. The widgets then hold no reference of their own to the form, but the event's delegate list does.
