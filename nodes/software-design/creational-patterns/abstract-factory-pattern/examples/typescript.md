```typescript run id=widgets
interface Widget {
  render(): string;
}

interface WidgetFactory {
  button(label: string): Widget;
  checkbox(label: string, checked: boolean): Widget;
}

class PlainFactory implements WidgetFactory {
  button(label: string): Widget {
    return { render: () => `[ ${label} ]` };
  }

  checkbox(label: string, checked: boolean): Widget {
    return { render: () => `[${checked ? "x" : " "}] ${label}` };
  }
}

class RoundFactory implements WidgetFactory {
  button(label: string): Widget {
    return { render: () => `( ${label} )` };
  }

  checkbox(label: string, checked: boolean): Widget {
    return { render: () => `(${checked ? "*" : " "}) ${label}` };
  }
}

function loginForm(widgets: WidgetFactory): string {
  return [widgets.button("Save").render(), widgets.checkbox("Remember me", true).render()].join(" ");
}

const kits: [string, WidgetFactory][] = [
  ["plain", new PlainFactory()],
  ["round", new RoundFactory()],
];
for (const [name, kit] of kits) {
  console.log(`${name}: ${loginForm(kit)}`);
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. An interface declares the factory methods, and `implements` makes the compiler check that each concrete factory has all of them.
- The type system of TypeScript is structural. An object literal with the two methods is a `WidgetFactory` with no `implements` clause, so the JavaScript form also type-checks.
- When a method is added to `WidgetFactory`, each class that implements it is a compile error until it gets the method. This is the liability of a new kind of product, found by the compiler.
