```javascript wrong id=template-literal
function commentHtml(author, text) {
  return `<p class="comment"><b>${author}</b>: ${text}</p>`;
}

console.log(commentHtml("alice", "Nice post."));
console.log(commentHtml("mallory", "<script>alert(1)</script>"));
```

```output for=template-literal
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: <script>alert(1)</script></p>
```

In a browser, code like `element.innerHTML = commentHtml(...)` parses this string as markup. The same happens on a server that sends the string as the body of an HTML response.

```javascript run id=tagged-template
const ENTITIES = { "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#x27;" };

function escapeHtml(value) {
  return String(value).replace(/[&<>"']/g, (ch) => ENTITIES[ch]);
}

// A tag function gets the fixed parts of the template apart from the values.
// It escapes each value and keeps each fixed part as it is.
function html(strings, ...values) {
  let out = strings[0];
  values.forEach((value, i) => {
    out += escapeHtml(value) + strings[i + 1];
  });
  return out;
}

function commentHtml(author, text) {
  return html`<p class="comment"><b>${author}</b>: ${text}</p>`;
}

console.log(commentHtml("alice", "Nice post."));
console.log(commentHtml("mallory", "<script>alert(1)</script>"));
```

```output for=tagged-template
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: &lt;script&gt;alert(1)&lt;/script&gt;</p>
```

- Verdict: stdlib. In a browser, the DOM has sinks that take text and never parse it as markup: `textContent`, `createTextNode`, `insertAdjacentText` and the `value` of a form field. `innerHTML` parses its value as markup. Node.js has no HTML escape function.
- The safe form in the browser is `element.textContent = text`. It makes a text node and does no encoding, so do not escape the value first.
- A tagged template sees the fixed parts and the values apart, as a parameterised query does. The second example uses this to escape only the values.
- Many frameworks escape by default and have a way to turn the escape off, such as `dangerouslySetInnerHTML` in React. Review each use. HTML that users write needs a maintained sanitizer, such as DOMPurify, and not an escape.
- `eval`, `setTimeout` and `setInterval` can run a string as code. Do not pass input to them.
- CodeQL queries `js/xss` (client side) and `js/reflected-xss` (server) follow input to these sinks.
