```javascript run id=token
const { randomBytes, timingSafeEqual } = require("node:crypto");

const sessions = new Map([["c0ffee", { user: "alice", csrf: randomBytes(32).toString("hex") }]]);

function changeEmail(cookie, formToken, email) {
  const s = sessions.get(cookie);
  if (s === undefined) {
    return "401 not logged in";
  }
  const sent = Buffer.from(formToken);
  const expected = Buffer.from(s.csrf);
  // timingSafeEqual needs buffers of the same length.
  if (sent.length !== expected.length || !timingSafeEqual(sent, expected)) {
    return "403 missing or wrong CSRF token";
  }
  return `200 email of ${s.user} is ${email}`;
}

console.log(changeEmail("c0ffee", sessions.get("c0ffee").csrf, "alice@example.net"));
console.log(changeEmail("c0ffee", "", "mallory@attacker.example"));
```

```output for=token
200 email of alice is alice@example.net
403 missing or wrong CSRF token
```

`crypto.randomBytes` makes the token. `crypto.timingSafeEqual` compares in constant time, and it throws when the lengths differ, so the code checks the length first. The length of the token is not a secret.

- Verdict: idiom. Node.js has no CSRF check in its HTTP server. Express needs a middleware, and CodeQL query `js/missing-token-validation` reports a cookie-based Express app without one.
- Send the token in the body or in a custom request header, never in the URL.
