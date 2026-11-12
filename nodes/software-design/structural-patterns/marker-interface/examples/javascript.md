```javascript run id=redacting-logger
const sensitive = Symbol("sensitive");

class Username {
  constructor(value) {
    this.value = value;
  }
}

class Password {
  static {
    this.prototype[sensitive] = true;
  }
  constructor(value) {
    this.value = value;
  }
}

class ApiKey {
  static {
    this.prototype[sensitive] = true;
  }
  constructor(value) {
    this.value = value;
  }
}

function log(label, item) {
  const shown = sensitive in item ? "[redacted]" : item.value;
  console.log(`${label}: ${shown}`);
}

log("user", new Username("ann"));
log("password", new Password("hunter2"));
log("api key", new ApiKey("k-123"));
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. JavaScript has no interfaces, so a property with a private `Symbol` key is the marker. The `in` operator also finds the property on the prototype.
