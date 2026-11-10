```python wrong id=fstring
def comment_html(author, text):
    return f'<p class="comment"><b>{author}</b>: {text}</p>'


print(comment_html("alice", "Nice post."))
print(comment_html("mallory", "<script>alert(1)</script>"))
```

```output for=fstring
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: <script>alert(1)</script></p>
```

The second comment is now a `script` element in the page. A browser that loads the page runs it with the rights of the site.

```python run id=escape
from html import escape


def comment_html(author, text):
    return f'<p class="comment"><b>{escape(author)}</b>: {escape(text)}</p>'


print(comment_html("alice", "Nice post."))
print(comment_html("mallory", "<script>alert(1)</script>"))
print(comment_html("bob", "I <3 \"quotes\" & 'apostrophes'"))
```

```output for=escape
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: &lt;script&gt;alert(1)&lt;/script&gt;</p>
<p class="comment"><b>bob</b>: I &lt;3 &quot;quotes&quot; &amp; &#x27;apostrophes&#x27;</p>
```

```python wrong id=href
from html import escape


def profile_link(url):
    return f'<a href="{escape(url)}">home page</a>'


print(profile_link("https://example.com/alice"))
print(profile_link("javascript:alert(1)"))
```

```output for=href
<a href="https://example.com/alice">home page</a>
<a href="javascript:alert(1)">home page</a>
```

`escape` keeps the value inside the quotes of the attribute, but the attribute is a URL. The browser runs a `javascript:` URL when the user clicks the link. The context needs a second check.

```python run id=scheme
from html import escape
from urllib.parse import urlsplit

ALLOWED_SCHEMES = {"http", "https"}


def profile_link(url):
    if urlsplit(url).scheme.lower() not in ALLOWED_SCHEMES:
        url = "#"
    return f'<a href="{escape(url)}">home page</a>'


print(profile_link("https://example.com/alice"))
print(profile_link("javascript:alert(1)"))
```

```output for=scheme
<a href="https://example.com/alice">home page</a>
<a href="#">home page</a>
```

- Verdict: stdlib. `html.escape` converts `&`, `<` and `>`, and with `quote=True`, the default, also `"` and `'`.
- `html.escape` is correct for text between tags and for the value of a quoted attribute. It is not enough for a URL, for JavaScript or for CSS. The third and fourth examples show the URL case: the fix is an allow-list of schemes, applied before the escape.
- Template engines do the escape for each value. Jinja2 escapes values only when the environment has `autoescape` on, and Ruff rule S701 reports `autoescape=False`. Django escapes by default, and `mark_safe` turns the escape off for one value, which Ruff rule S308 reports.
- Do not build HTML with `+`, `%`, `format` or f-strings in a web handler. The examples here use f-strings only to show what the escape does.
