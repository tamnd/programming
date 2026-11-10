```javascript run id=from-environment
function loadConfig(env) {
  const password = env.EXAMPLE_DB_PASSWORD;
  if (!password) {
    throw new Error("EXAMPLE_DB_PASSWORD is not set");
  }
  return { user: env.EXAMPLE_DB_USER ?? "app", password };
}

function describe(config) {
  // Logs show that a secret is present, never the secret itself.
  return `user=${config.user} password=<set>`;
}

try {
  loadConfig(process.env);
} catch (error) {
  console.log("refused to start:", error.message);
}

console.log(describe(loadConfig({ EXAMPLE_DB_PASSWORD: "example-not-a-real-password" })));
```

```output for=from-environment
refused to start: EXAMPLE_DB_PASSWORD is not set
user=app password=<set>
```

`process.env` is a plain object of strings. A variable that is not set is `undefined`, so the check refuses to start without it.

- Verdict: stdlib. Node.js reads the environment through `process.env`. The task [[read-an-environment-variable]] covers the details.
- Node.js 20.6 and later can load a `.env` file with the option `--env-file`. Keep that file out of version control.
- Code that runs in a browser cannot keep a secret. Every value in a bundle reaches the user, including values that a bundler copies from the environment at build time. Keep secrets on the server.
- `JSON.stringify` and `console.log` print every property of an object. Keep the secret out of objects that the program logs, or give the class a `toJSON` method that hides it.
