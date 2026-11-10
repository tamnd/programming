---
tag: NEW-missing-authentication
kind: weakness
slug: missing-authentication
name: Missing authentication for a critical function
aliases: [missing authentication for critical function, missing authentication for critical functionality, no authentication for critical function, unauthenticated endpoint, cwe-306]
area: security/authentication-and-sessions
level: core
summary: >-
  Missing authentication happens when a function that needs a known user, such as an admin endpoint or a debug port, accepts requests from anyone who can reach it.
cwe: 306
abstraction: base
languages_affected: [any]
owasp_top10_2025: A07
cwe_top25_2025: 21
history:
  year: 1988
  people: [Steven M. Bellovin]
  note: The Internet worm of 2 November 1988 spread through the debug command of sendmail, which was enabled in the standard distributions and could be used from a remote host without any login.
sources:
  - url: https://cwe.mitre.org/data/definitions/306.html
    title: "CWE-306: Missing Authentication for Critical Function"
    locator: Description; Relationships; Modes Of Introduction; Common Consequences; Detection Methods; Potential Mitigations; Demonstrative Examples; Observed Examples; Content History
  - url: https://www.rfc-editor.org/rfc/rfc1135.txt
    title: "J. Reynolds, RFC 1135: The Helminthiasis of the Internet, December 1989"
    locator: section 1, Introduction; section 2.1, the paragraph on sendmail
  - url: https://catless.ncl.ac.uk/Risks/7/74
    title: "Steven Bellovin, The worm and the debug option, The RISKS Digest, Volume 7, Issue 74, 9 November 1988"
    locator: the message "The worm and the debug option"
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 21, Rank Last Year
  - url: https://top10.owasp.org/2025/A07_2025-Authentication_Failures/
    title: "OWASP Top 10:2025, A07:2025 Authentication Failures"
    locator: How to prevent; List of Mapped CWEs
  - url: https://top10.owasp.org/2025/A02_2025-Security_Misconfiguration/
    title: "OWASP Top 10:2025, A02:2025 Security Misconfiguration"
    locator: Description, the list of misconfigurations; How to prevent; Example attack scenarios, Scenario 1; List of Mapped CWEs
  - url: https://cwe.mitre.org/data/definitions/288.html
    title: "CWE-288: Authentication Bypass Using an Alternate Path or Channel"
    locator: Description
  - url: https://cwe.mitre.org/data/definitions/489.html
    title: "CWE-489: Active Debug Code"
    locator: Description
  - url: https://cwe.mitre.org/data/definitions/1188.html
    title: "CWE-1188: Initialization of a Resource with an Insecure Default"
    locator: Description
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Authorization_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Authorization Cheat Sheet
    locator: Deny by Default; Validate the Permissions on Every Request
  - url: https://pkg.go.dev/net/http/pprof
    title: Go standard library, net/http/pprof
    locator: Overview; func Cmdline
  - url: https://docs.python.org/3.14/library/http.server.html
    title: The Python Standard Library, http.server, Python 3.14
    locator: the security warning at the top of the page
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/jdk.httpserver/com/sun/net/httpserver/HttpContext.html
    title: Java SE 25 API, com.sun.net.httpserver.HttpContext
    locator: setAuthenticator
  - url: https://www.cve.org/CVERecord?id=CVE-2020-13927
    title: CVE-2020-13927, Apache Airflow
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-21972
    title: CVE-2021-21972, VMware vCenter Server
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-37415
    title: CVE-2021-37415, Zoho ManageEngine ServiceDesk Plus
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-10263
    title: CVE-2020-10263, XIAOMI XIAOAI speaker Pro LX06
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-11680
    title: CVE-2024-11680, ProjectSend
    locator: the description, the CWE and the publication date
---

## Description

Authentication answers the question "who is this user?". Most of a program's functions need that answer before they run. Missing authentication happens when the product does not perform any authentication for functionality that requires a provable user identity or consumes a significant amount of resources.[^1] The function works, and nothing in normal use shows a problem. But anyone who can reach it can call it.

CWE describes the result in one sentence. Exposing critical functionality essentially provides an attacker with the privilege level of that functionality.[^1] An endpoint that changes the configuration gives the attacker the rights of an administrator. A debug port that opens a shell gives the attacker the rights of the process.

The weakness is easy to make for four reasons:

- **A second channel.** CWE describes a login on one network port that opens a second port for the real work. The second port has no check, because the developer assumes that only the authenticated party will connect to it.[^1] An admin port, a metrics port and a debug port often follow the same idea: nobody else knows about it.
- **A route added later.** The first routes of a service pass through the login check. A new route, written in a hurry or for the support team, does not. Each route makes its own decision, so one missing line opens one function.
- **A debug feature left on.** A tool that helps during development stays in the release. CWE-489, Active Debug Code, names this case: the product is released with debugging code still enabled or active.[^8]
- **An open default.** A product ships with authentication off, so that the first install is easy to try. CWE-1188 covers a default that is intended to be changed by the installer but is not secure.[^9] Many installs never change it.

CWE also lists the move to the cloud. Data on an internal network was protected by that network. In a storage bucket without authentication, it is reachable from the Internet.[^1]

CWE-306 is a base weakness and a child of CWE-287, Improper Authentication.[^1] Its child CWE-288 covers a product that requires authentication but has an alternate path or channel that does not.[^7] CWE-306 is rank 21 on the 2025 CWE Top 25, up from rank 25 the year before.[^4] The OWASP Top 10:2025 maps it to A07:2025 Authentication Failures.[^5] The A02:2025 Security Misconfiguration category lists unnecessary ports, services, pages, accounts and testing frameworks that are enabled.[^6]

Two related weaknesses have their own pages. With [[missing-authorization]], the program knows who the user is but never asks whether the user may act. With [[hard-coded-credentials]], a check exists, but its password is the same in every copy of the product.

### History

The Internet worm was released on the evening of 2 November 1988.[^2] It used errors in two network programs, sendmail and fingerd. In sendmail it used a non-standard "debug" command, and it propagated itself through that command into remote hosts.[^2] Steven Bellovin wrote in the RISKS Digest a week later that debugging came enabled in the standard distributions of both Berkeley and Sun. He named the flaw: "The flaw is not that debug mode was possible; rather, that sendmail's debug mode (a) was accessible remotely; and (b) expanded the range of inputs accepted by the program".[^3] That is CWE-306 in its first well-known form: a powerful function, reachable from the network, with no check of who calls it.

CWE-306 entered CWE in July 2006, from the PLOVER list.[^1]

## Vulnerable code

The program below runs two servers on 127.0.0.1. The application server checks a bearer token before it shows the orders. The operators also asked for a way to put the service into maintenance mode, so a developer added a second server on another port. That port is not in the public documentation, so the developer gave it no login. The demonstration is a request to the second port with no credentials.

```python wrong id=debug-port
import secrets
import threading
import urllib.error
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

TOKEN = secrets.token_hex(16)  # made at start; the operators get it
SETTINGS = {"maintenance": False}


class Handler(BaseHTTPRequestHandler):
    def reply(self, status, text):
        body = text.encode()
        self.send_response(status)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *args):
        pass


class AppHandler(Handler):
    def do_GET(self):
        if self.path == "/orders":
            if self.headers.get("Authorization") != "Bearer " + TOKEN:
                return self.reply(401, "login required")
            return self.reply(200, "3 orders")
        self.reply(404, "not found")


class DebugHandler(Handler):
    # A second port for the operators. Nobody else knows it, so it has no login.
    def do_POST(self):
        if self.path == "/maintenance":
            SETTINGS["maintenance"] = True
            return self.reply(200, "maintenance mode on")
        self.reply(404, "not found")


def start(handler):
    server = ThreadingHTTPServer(("127.0.0.1", 0), handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server


def call(method, server, path):
    url = f"http://127.0.0.1:{server.server_port}{path}"
    opener = urllib.request.build_opener(urllib.request.ProxyHandler({}))
    try:
        with opener.open(urllib.request.Request(url, method=method)) as response:
            return response.status, response.read().decode()
    except urllib.error.HTTPError as error:
        return error.code, error.read().decode()


app = start(AppHandler)
debug = start(DebugHandler)
print("app   GET /orders      ", call("GET", app, "/orders"))
print("debug POST /maintenance", call("POST", debug, "/maintenance"))
print(SETTINGS)
```

```output for=debug-port
app   GET /orders       (401, 'login required')
debug POST /maintenance (200, 'maintenance mode on')
{'maintenance': True}
```

The main port refuses the request without a token. The second port accepts the same caller and changes the state of the service. A port scan can find the second port, so the secret of its number does not protect it. In a real deployment both servers listen on an address that other machines can reach. The demonstration uses 127.0.0.1 so that it runs without a network.

## Fixed code

The fixed code moves the decision out of the handlers. Every handler class inherits from `GatedHandler`. Each class lists its routes in a table, and each route says whether it is public. The only public route is `/health`. A route that is not in the table gets 404. A route that is not public gets 401 unless the request has the token. The second port uses the same base class, so it gets the same check.

```python run id=gated
import hmac
import secrets
import threading
import urllib.error
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

TOKEN = secrets.token_hex(16)  # made at start; the operators get it
SETTINGS = {"maintenance": False}


class GatedHandler(BaseHTTPRequestHandler):
    # Each subclass lists its routes: (method, path) -> (method name, public).
    ROUTES = {}

    def dispatch(self, method):
        route = self.ROUTES.get((method, self.path))
        if route is None:
            return self.reply(404, "not found")
        name, public = route
        if not public and not self.authenticated():
            return self.reply(401, "login required")
        getattr(self, name)()

    def authenticated(self):
        sent = self.headers.get("Authorization", "")
        return hmac.compare_digest(sent.encode(), ("Bearer " + TOKEN).encode())

    def do_GET(self):
        self.dispatch("GET")

    def do_POST(self):
        self.dispatch("POST")

    def reply(self, status, text):
        body = text.encode()
        self.send_response(status)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *args):
        pass


class AppHandler(GatedHandler):
    ROUTES = {("GET", "/health"): ("health", True), ("GET", "/orders"): ("orders", False)}

    def health(self):
        self.reply(200, "ok")

    def orders(self):
        self.reply(200, "3 orders")


class DebugHandler(GatedHandler):
    ROUTES = {("POST", "/maintenance"): ("maintenance", False)}

    def maintenance(self):
        SETTINGS["maintenance"] = True
        self.reply(200, "maintenance mode on")


def start(handler):
    server = ThreadingHTTPServer(("127.0.0.1", 0), handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server


def call(method, server, path, token=None):
    url = f"http://127.0.0.1:{server.server_port}{path}"
    headers = {"Authorization": "Bearer " + token} if token else {}
    opener = urllib.request.build_opener(urllib.request.ProxyHandler({}))
    try:
        with opener.open(urllib.request.Request(url, method=method, headers=headers)) as response:
            return response.status, response.read().decode()
    except urllib.error.HTTPError as error:
        return error.code, error.read().decode()


app = start(AppHandler)
debug = start(DebugHandler)
print("app   GET /health      ", call("GET", app, "/health"))
print("app   GET /orders      ", call("GET", app, "/orders"))
print("debug POST /maintenance", call("POST", debug, "/maintenance"))
print(SETTINGS)
print("debug POST /maintenance", call("POST", debug, "/maintenance", TOKEN))
print(SETTINGS)
```

```output for=gated
app   GET /health       (200, 'ok')
app   GET /orders       (401, 'login required')
debug POST /maintenance (401, 'login required')
{'maintenance': False}
debug POST /maintenance (200, 'maintenance mode on')
{'maintenance': True}
```

Three choices make the fix hold when the code grows:

- **Authenticated by default.** A new route must say `True` to be public. If a developer forgets the flag, the route is protected. The OWASP Authorization Cheat Sheet asks for the same deny-by-default stance whenever new functionality is exposed.[^10]
- **One check for every channel.** CWE asks developers to identify all potential channels and to protect the ones that are assumed to be reachable only by authorized parties.[^1] The debug port here is a channel like any other.
- **A comparison in constant time.** `hmac.compare_digest` compares the token without an early exit on the first wrong byte. The page [[sensitive-information-exposure]] explains why that matters.

`http.server` itself is a teaching tool. Its documentation says that it is not recommended for production and that it only implements basic security checks.[^12] A production service uses the authentication of its framework, and the same rules apply there.

### Debug endpoints

Debug and profiling tools often register their handlers as a side effect. Go's `net/http/pprof` package is typically only imported for the side effect of registering its HTTP handlers, and its paths all begin with `/debug/pprof/`.[^11] The handlers go on `http.DefaultServeMux`. A service that serves the default mux on its public port then serves the profiles to anyone. One of them, `/debug/pprof/cmdline`, returns the command line of the running program,[^11] which may hold a path or a flag that the operators did not mean to publish. The Go example file shows the problem and the fix: a separate mux for the public server, and the pprof handlers registered by hand on a second mux behind a check.

Debug output is not the only risk. Bellovin's point from 1988 still holds: the debug mode of sendmail was dangerous because it accepted more kinds of input than the normal program did.[^3] A debug endpoint that runs commands, changes settings or dumps memory is a critical function, and it needs the same check as the admin page.

## How to detect it

- **List every listener and every route.** Start from the network side: which ports does the process open, and what answers on each one? Then list every route on each port with its authentication rule. CWE notes that manual analysis, such as penetration testing and threat modeling, can find this weakness, and it rates architecture and design review as highly effective.[^1]
- **Send requests with no credentials.** Call each route without a token, without a cookie and with an expired session. A test that expects 401 for every route that is not public fails when a new route forgets the check.
- **Look for registration by side effect.** Search for imports such as `net/http/pprof`, for admin consoles and for actuator or metrics endpoints that a framework or a library adds.
- **Check the defaults.** Read the configuration that a fresh install gets. If authentication is off until an administrator turns it on, many installs will run with it off. The Airflow case below is an example.
- **Static analysis.** CWE rates automated static analysis as limited here. Tools find common authentication libraries, but they have difficulty with custom schemes. Some functions are public by design, so a tool that reports every function without a check reports false positives.[^1]

## How to prevent it

- **Divide the product into areas.** CWE suggests anonymous, normal, privileged and administrative areas. Decide which areas need a proven identity, and use one central authentication capability for them.[^1]
- **Authenticate by default.** Make a route public only by an explicit choice in code, as the fixed example does.[^10]
- **Protect every channel.** A second port, a message queue, a serial console and a management interface each need a check. CWE gives a UART debug port as an observed example.[^1]
- **Ship with debug features off.** Remove debug code from release builds, or make it a separate binary. OWASP asks for a minimal platform without unnecessary features.[^6]
- **Ship with authentication on.** Do not ship an open default that the installer must change. OWASP also says not to ship or deploy with default credentials, particularly for admin users.[^5]
- **Use the authentication of the framework.** CWE advises against custom authentication routines where the environment provides them.[^1] The Java `HttpServer` has `HttpContext.setAuthenticator`. Once an authenticator is set on a context, all client requests to it must be authenticated.[^13]
- **Duplicate client checks on the server.** A login screen in a mobile app or a desktop client does not protect the server. CWE gives this as a mitigation.[^1]
- **Add network controls as a second layer.** Bind admin and debug listeners to the loopback address, or put them behind a firewall. This narrows who can reach the port. It does not replace the check.

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2020-10263 | published 2020-04-08 | Attackers with access to the UART interface of the XIAOMI XIAOAI speaker Pro LX06 could get a root shell.[^17] CWE lists it as debug functionality on the UART port with no authentication.[^1] |
| CVE-2020-13927 | published 2020-11-10 | The previous default setting of the Experimental API of Apache Airflow allowed all API requests without authentication. From Airflow 1.10.11, the default denies all requests.[^14] CWE notes that it was exploited in the wild, per the CISA KEV catalog.[^1] |
| CVE-2021-21972 | published 2021-02-24 | A vCenter Server plugin in the vSphere Client had a remote code execution vulnerability for an actor with network access to port 443.[^15] CWE lists it as a chain: an upload of a tar file with no authentication, then path traversal in the file.[^1] |
| CVE-2021-37415 | published 2021-09-01 | Zoho ManageEngine ServiceDesk Plus before 11302 allowed a few REST API URLs without authentication.[^16] CWE notes that it was exploited in the wild, per the CISA KEV catalog.[^1] |
| CVE-2024-11680 | published 2024-11-26 | Unauthenticated requests to `options.php` of ProjectSend before r1720 could change the configuration of the application.[^18] |

## References
