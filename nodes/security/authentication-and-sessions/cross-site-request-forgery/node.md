---
tag: NEW-cross-site-request-forgery
kind: weakness
slug: cross-site-request-forgery
name: Cross-site request forgery
aliases: [CSRF, XSRF, sea surf, session riding, one-click attack, cwe-352]
area: security/authentication-and-sessions
level: core
summary: >-
  Cross-site request forgery happens when a web application trusts a request because it carries the user's cookies, so a page on another site can make the browser act for the user.
cwe: 352
abstraction: compound
languages_affected: [any-web-server]
owasp_top10_2025: A01
cwe_top25_2025: 3
history:
  year: 2001
  people: [Peter W]
  note: In a Bugtraq post on June 15, 2001, Peter W named the problem Cross-Site Request Forgeries (CSRF, pronounced "sea surf") and wrote that many web applications are good at identifying users but terrible at verifying origins and intent.
same_as:
  wikidata: Q15401472
  wikipedia: Cross-site_request_forgery
sources:
  - url: https://cwe.mitre.org/data/definitions/352.html
    title: "CWE-352: Cross-Site Request Forgery (CSRF)"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Composite Components; Demonstrative Examples, Example 1; Detection Methods; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 3, CVEs in KEV, Rank Last Year
  - url: https://owasp.org/Top10/2025/A01_2025-Broken_Access_Control/
    title: "OWASP Top 10:2025, A01:2025 Broken Access Control"
    locator: Background; List of Mapped CWEs
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Cross-Site_Request_Forgery_Prevention_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Cross-Site Request Forgery Prevention Cheat Sheet
    locator: Introduction; Cross-Site Scripting (XSS) will defeat all CSRF mitigation techniques; Built-In Or Existing CSRF Implementations; Synchronizer Token Pattern; Fetch Metadata headers; Using Standard Headers to Verify Origin; SameSite (Cookie Attribute), Limitations of SameSite
  - url: https://marc.info/?l=bugtraq&m=99263135911884&w=2
    title: "Peter W, Cross-Site Request Forgeries (Re: The Dangers of Allowing Users to Post Images), Bugtraq, 2001-06-15"
    locator: the opening paragraphs; The difference between XSS and CSRF
  - url: https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Sec-Fetch-Site
    title: MDN Web Docs, Sec-Fetch-Site
    locator: Directives
  - url: https://pkg.go.dev/net/http#CrossOriginProtection
    title: Go standard library, net/http, CrossOriginProtection
    locator: the type description
  - url: https://docs.djangoproject.com/en/stable/ref/csrf/
    title: Django documentation, Cross Site Request Forgery protection
    locator: How it works
  - url: https://learn.microsoft.com/en-us/aspnet/core/security/anti-request-forgery
    title: ASP.NET Core documentation, Prevent Cross-Site Request Forgery (XSRF/CSRF) attacks
    locator: the opening section, an example of a CSRF attack
  - url: https://docs.python.org/3/library/secrets.html
    title: The Python Standard Library, secrets
    locator: the opening section; Generating tokens
  - url: https://docs.python.org/3/library/hmac.html#hmac.compare_digest
    title: The Python Standard Library, hmac
    locator: hmac.compare_digest
  - url: https://codeql.github.com/codeql-query-help/javascript/js-missing-token-validation/
    title: CodeQL query help, Missing CSRF middleware (js/missing-token-validation)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/python/py-csrf-protection-disabled/
    title: CodeQL query help, CSRF protection weakened or disabled (py/csrf-protection-disabled)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/java/java-spring-disabled-csrf-protection/
    title: CodeQL query help, Disabled Spring CSRF protection (java/spring-disabled-csrf-protection)
    locator: the query description
  - url: https://codeql.github.com/codeql-query-help/csharp/cs-web-missing-token-validation/
    title: CodeQL query help, Missing cross-site request forgery token validation (cs/web/missing-token-validation)
    locator: the query description; Recommendation
  - url: https://www.cve.org/CVERecord?id=CVE-2004-1703
    title: CVE-2004-1703, Fusion News
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2009-3022
    title: CVE-2009-3022, bingo!CMS
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2009-3759
    title: CVE-2009-3759, Citrix XenCenterWeb
    locator: the description and the publication date
---

## Description

A browser adds the cookies of a site to every request that it sends to that site. It does this when the user clicks a button on the site, and also when a page on another site holds a form, an image or a script that points to the site.[^4][^9] A web application that identifies the user by a session cookie therefore sees two kinds of request that look the same. One comes from its own page, because the user wants the action. The other comes from a page that an attacker wrote, and the user does not know about it. Cross-site request forgery (CSRF) happens when the application cannot tell the two apart and carries out both.[^1]

The root cause is that a cookie answers the wrong question. It answers "who is this user?". It does not answer "did this user ask for this action?". CWE-352 is a composite weakness. It requires several weaknesses at the same time, among them CWE-346, an origin validation error, and CWE-441, an unintended proxy or intermediary. The browser of the user is that intermediary. Removing one of the weaknesses removes or sharply reduces the risk.[^1] Peter W wrote in 2001 that many web applications are good at identifying users and understanding requests, but terrible at verifying origins and intent.[^5] The check of the session in CWE-352 Example 1 is correct, and the application is still vulnerable, because the forged request carries a valid session.[^1]

The weakness is there by default, because a server that works for its users already has it. A form, a handler and a session check are all that a normal feature needs. Nothing fails in a test, because the test never sends a request from another site. HTTPS does not help, since the forged request uses HTTPS too.[^9]

The consequences depend on what the request can do. CWE-352 says that the server treats the forged request as authentic, so the attacker performs operations as the victim. If the victim is an administrator, the attacker may get complete control over the web application.[^1] Cross-site scripting defeats most CSRF defences, because a script on the site can read the tokens and send requests from the right origin.[^1][^4] The page [[cross-site-scripting]] covers that weakness.

CWE-352 is rank 3 in the 2025 CWE Top 25, with no CVEs in the CISA catalogue of known exploited vulnerabilities, up from rank 4 the year before.[^2] The OWASP Top 10:2025 maps it to A01:2025 Broken Access Control.[^3]

### History

On June 15, 2001, Peter W replied on the Bugtraq list to a post about images on message boards. The post had shown that an `[img]` tag on a forum makes each reader's browser send a GET request to any URL. Peter W named the general problem Cross-Site Request Forgeries, CSRF, pronounced "sea surf".[^5] He wrote that the problem is not the image tag. It is the backend application that the tag reaches. He also set CSRF apart from cross-site scripting. Cross-site scripting puts active code into a page. CSRF uses tags such as `IMG` to forge requests with the user's credentials, to the site of the page or to any other site.[^5] Early records in CWE-352 match his description, such as CVE-2004-1703, where an image tag in a comment added user accounts.[^1][^16]

## Vulnerable code

The handler below changes the email address of the logged-in user. It runs in the same process, with requests as dictionaries, so the page needs no network. The second request is the classic demonstration: a form on another site posts to the handler, and Alice's browser adds her session cookie.

```python wrong id=cookie-only
SESSIONS = {"c0ffee": "alice"}
EMAILS = {"alice": "alice@example.com"}


def change_email(request: dict) -> str:
    user = SESSIONS.get(request["cookies"].get("session"))
    if user is None:
        return "401 not logged in"
    EMAILS[user] = request["form"]["email"]
    return f"200 email of {user} is {EMAILS[user]}"


# Alice submits the form on the site.
print(change_email({
    "headers": {"Sec-Fetch-Site": "same-origin"},
    "cookies": {"session": "c0ffee"},
    "form": {"email": "alice@example.net"},
}))

# A page on another site submits a form to the same URL.
print(change_email({
    "headers": {"Sec-Fetch-Site": "cross-site", "Origin": "https://attacker.example"},
    "cookies": {"session": "c0ffee"},
    "form": {"email": "mallory@attacker.example"},
}))
```

```output for=cookie-only
200 email of alice is alice@example.net
200 email of alice is mallory@attacker.example
```

Both requests succeed. The handler checks the session, and the session is valid in both. The headers show where the second request came from, but the handler does not read them. If the site sends password resets to this address, the attacker can now take over the account.

## Fixed code

The fix adds a second secret that only the site's own pages have: a synchronizer token. The server makes a random token for each session, puts it in each form as a hidden field, and compares it on each request that changes state. The OWASP cheat sheet says that the token must be unique per session, secret and unpredictable.[^4] The `secrets` module is the source for security tokens in Python, and `hmac.compare_digest` compares without the timing leak of `==`.[^10][^11]

```python run id=synchronizer-token
import hmac
import secrets

SESSIONS = {"c0ffee": {"user": "alice", "csrf": secrets.token_urlsafe(32)}}
EMAILS = {"alice": "alice@example.com"}


def form_token(session_id: str) -> str:
    # The server puts this value in a hidden field of its own form.
    return SESSIONS[session_id]["csrf"]


def change_email(request: dict) -> str:
    session = SESSIONS.get(request["cookies"].get("session"))
    if session is None:
        return "401 not logged in"
    sent = request["form"].get("csrf_token", "")
    if not hmac.compare_digest(sent, session["csrf"]):
        return "403 missing or wrong CSRF token"
    EMAILS[session["user"]] = request["form"]["email"]
    return f"200 email of {session['user']} is {EMAILS[session['user']]}"


print(change_email({
    "cookies": {"session": "c0ffee"},
    "form": {"email": "alice@example.net", "csrf_token": form_token("c0ffee")},
}))

# The other site cannot read the token from the site's pages.
print(change_email({
    "cookies": {"session": "c0ffee"},
    "form": {"email": "mallory@attacker.example"},
}))
```

```output for=synchronizer-token
200 email of alice is alice@example.net
403 missing or wrong CSRF token
```

The attacker's page can make the browser send the cookie, but it cannot read the site's pages, so it does not have the token. The same-origin policy is what keeps the token secret. A token in a GET URL can leak through logs, history and the `Referer` header, so it belongs in the body or in a request header.[^4]

### Checking where the request came from

Browsers also tell the server where a request came from. The `Sec-Fetch-Site` header has the value `same-origin`, `same-site`, `cross-site` or `none`.[^6] The OWASP cheat sheet accepts this header as a defence, with a fallback to the `Origin` header for browsers that do not send it.[^4] The Go standard library has the check as `http.CrossOriginProtection`. It allows GET, HEAD and OPTIONS, rejects cross-origin requests with other methods, and compares the `Origin` header with the `Host` header when `Sec-Fetch-Site` is absent.[^7]

```go run since=1.25 id=cross-origin-protection
package main

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"strings"
)

func main() {
	mux := http.NewServeMux()
	mux.HandleFunc("POST /email", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "email changed")
	})
	handler := http.NewCrossOriginProtection().Handler(mux)

	cases := []struct{ name, header, value string }{
		{"own page", "Sec-Fetch-Site", "same-origin"},
		{"other site", "Sec-Fetch-Site", "cross-site"},
		{"other origin, older browser", "Origin", "https://attacker.example"},
	}
	for _, c := range cases {
		req := httptest.NewRequest("POST", "https://app.example/email", strings.NewReader("email=x"))
		req.Header.Set(c.header, c.value)
		rec := httptest.NewRecorder()
		handler.ServeHTTP(rec, req)
		fmt.Printf("%s: %d\n", c.name, rec.Code)
	}
}
```

```output for=cross-origin-protection
own page: 200
other site: 403
other origin, older browser: 403
```

The check works only if no GET request changes state. The Go documentation and the cheat sheet both say so.[^7][^4] Requests with neither header pass, because they come from the same origin or from a client that is not a browser.[^7]

## How to detect it

- **Manual testing.** CWE-352 rates manual analysis as highly effective. For each request that changes state, send it again from a page on another origin and see if it succeeds.[^1]
- **Static analysis.** CWE-352 rates automated static analysis as limited, because each application has its own rules about which requests an outsider may cause.[^1] Tools find a missing or a disabled defence instead. CodeQL queries `js/missing-token-validation`, `py/csrf-protection-disabled`, `java/spring-disabled-csrf-protection` and `cs/web/missing-token-validation` report handlers or settings without the framework's CSRF check.[^12][^13][^14][^15]
- **Code review.** List every route that changes state. Check that none of them accepts GET, that each one is behind the framework's CSRF check, and that no route is exempt without a reason. Look for login forms too, because a forged login can put the user in the attacker's account.[^15]

## How to prevent it

- **Use the framework's defence.** The OWASP cheat sheet says to check for built-in protection first.[^4] Django keeps a random secret in a cookie and requires a matching `csrfmiddlewaretoken` field on every request that is not GET, HEAD, OPTIONS or TRACE, and returns 403 if it is wrong.[^8] ASP.NET Core and Go have their own mechanisms.[^9][^7] CodeQL says that in many frameworks the default configuration is sufficient against most CSRF attacks.[^13]
- **Use tokens or Fetch Metadata.** Stateful applications use the synchronizer token pattern. Stateless applications use the double-submit cookie pattern in its signed form, and the cheat sheet discourages the naive form.[^4] For browsers that send `Sec-Fetch-Site`, a check of that header with an `Origin` fallback is a defence too.[^4]
- **Keep state changes off GET.** CWE-352 and the cheat sheet both say not to use GET for any request that changes state.[^1][^4]
- **Fix cross-site scripting.** CWE-352 says that most CSRF defences can be bypassed by script that the attacker controls.[^1]
- **Set SameSite on cookies as defence in depth.** `SameSite=Lax` or `Strict` keeps the browser from sending the cookie on many cross-site requests. The cheat sheet says that it does not replace a CSRF defence: `Lax` still sends the cookie on top-level GET navigations, and "site" covers every subdomain of the registrable domain.[^4]
- **Ask again for dangerous actions.** CWE-352 suggests a separate confirmation for an especially dangerous operation, such as a change of password.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2004-1703 | published 2005-02-26 | In Fusion News, a comment with an `img` bbcode tag pointed at the signup action. When a logged-in administrator viewed the page, the browser called it and added a user account.[^16][^1] |
| CVE-2009-3022 | published 2009-08-31 | A CSRF weakness in bingo!CMS let remote attackers hijack the authentication of other users for requests that changed configuration or content.[^17] |
| CVE-2009-3759 | published 2009-10-22 | Sample code in the XenServer Resource Kit in Citrix XenCenterWeb let remote attackers hijack the authentication of administrators to change a password or stop a virtual machine.[^18] |

## References
