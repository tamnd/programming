```javascript run id=widgets
const plain = Object.freeze({
  button: (label) => ({ render: () => `[ ${label} ]` }),
  checkbox: (label, checked) => ({ render: () => `[${checked ? "x" : " "}] ${label}` }),
});

const round = Object.freeze({
  button: (label) => ({ render: () => `( ${label} )` }),
  checkbox: (label, checked) => ({ render: () => `(${checked ? "*" : " "}) ${label}` }),
});

// The client. It knows only the two factory functions of a kit.
function loginForm(widgets) {
  return [widgets.button("Save").render(), widgets.checkbox("Remember me", true).render()].join(" ");
}

for (const [name, kit] of Object.entries({ plain, round })) {
  console.log(`${name}: ${loginForm(kit)}`);
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. A concrete factory is an object literal whose properties are functions, and each product is an object with a `render` method. No class or interface is needed.
- Nothing checks that a kit has every factory function. A kit that lacks `checkbox` fails only when the client calls it, with a `TypeError`. TypeScript moves this check to compile time.
- `Object.freeze` stops code from replacing one function of a kit, which would mix families.
