```python run id=fetch-metadata
SAFE_METHODS = {"GET", "HEAD", "OPTIONS"}
OWN_ORIGIN = "https://app.example"


def allowed(request: dict) -> bool:
    if request["method"] in SAFE_METHODS:
        return True  # safe methods must not change state
    site = request["headers"].get("Sec-Fetch-Site")
    if site is not None:
        return site in ("same-origin", "none")
    origin = request["headers"].get("Origin")
    if origin is not None:
        return origin == OWN_ORIGIN
    return False  # no signal at all: refuse on a sensitive endpoint


requests = [
    ("own page", {"method": "POST", "headers": {"Sec-Fetch-Site": "same-origin"}}),
    ("other site", {"method": "POST", "headers": {"Sec-Fetch-Site": "cross-site"}}),
    ("sibling subdomain", {"method": "POST", "headers": {"Sec-Fetch-Site": "same-site"}}),
    ("older browser, other origin", {"method": "POST", "headers": {"Origin": "https://attacker.example"}}),
    ("no headers", {"method": "POST", "headers": {}}),
]
for name, request in requests:
    print(f"{name}: {'allowed' if allowed(request) else 'refused'}")
```

```output for=fetch-metadata
own page: allowed
other site: refused
sibling subdomain: refused
older browser, other origin: refused
no headers: refused
```

The policy follows the OWASP cheat sheet. `Sec-Fetch-Site` is the main signal. A `same-site` request comes from a sibling subdomain, so the policy refuses it unless the application trusts every subdomain. When the header is absent, the policy falls back to `Origin`. With no signal at all, it fails safe and refuses.

- Verdict: idiom. The standard library has no web framework and no CSRF check. Django has one in `CsrfViewMiddleware`, and CodeQL query `py/csrf-protection-disabled` reports a Django project that removes it.
- In Django, keep `CsrfViewMiddleware` in the middleware list, put `{% csrf_token %}` in each POST form, and do not mark a view `csrf_exempt` without a reason.
- For a custom server, use `secrets.token_urlsafe` for tokens and `hmac.compare_digest` to compare them, as in the main example.
