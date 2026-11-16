---
tag: 8T0WAF
kind: feature
owner: python
slug: web-libraries
name: Web libraries
aliases: [WSGI, ASGI, PEP 3333, wsgiref, Django, Flask, FastAPI, Starlette, Jinja, web framework]
area: languages/python/ecosystem
level: core
summary: >-
  Python web servers and frameworks meet at two interfaces: WSGI (PEP 3333) for synchronous apps and ASGI for asynchronous apps. Django and Flask use WSGI. FastAPI and Starlette use ASGI.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-3333/", locator: "PEP 3333, Python Web Server Gateway Interface v1.0.1"}
see_also: [python/the-python-package-index, python/governance-of-python]
sources:
  - url: https://peps.python.org/pep-3333/
    title: "PEP 3333: Python Web Server Gateway Interface v1.0.1"
    locator: Abstract; The Application/Framework Side; The start_response() Callable
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/wsgiref.html
    title: wsgiref, WSGI Utilities and Reference Implementation
    locator: introduction; wsgiref.util; wsgiref.validate
    accessed: 2026-11-16
  - url: https://asgi.readthedocs.io/en/latest/introduction.html
    title: Introduction, ASGI documentation
    locator: What’s wrong with WSGI?; How does ASGI work?
    accessed: 2026-11-16
  - url: https://asgi.readthedocs.io/en/latest/specs/www.html
    title: HTTP & WebSocket ASGI Message Format
    locator: Request; Response Start; Response Body
    accessed: 2026-11-16
  - url: https://www.djangoproject.com/
    title: Django
    locator: home page
    accessed: 2026-11-16
  - url: https://docs.djangoproject.com/en/6.1/intro/overview/
    title: Django at a glance
    locator: introduction
    accessed: 2026-11-16
  - url: https://flask.palletsprojects.com/en/stable/
    title: Welcome to Flask, Flask Documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://fastapi.tiangolo.com/
    title: FastAPI
    locator: introduction; Requirements
    accessed: 2026-11-16
  - url: https://starlette.dev/
    title: Introduction, Starlette
    locator: Introduction
    accessed: 2026-11-16
  - url: https://jinja.palletsprojects.com/en/stable/intro/
    title: Introduction, Jinja Documentation
    locator: Introduction
    accessed: 2026-11-16
  - url: https://lp.jetbrains.com/python-developers-survey-2024/
    title: Python Developers Survey 2024 Results
    locator: Frameworks and Libraries, Web frameworks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/net/http
    title: Package net/http
    locator: Overview; type Handler
    accessed: 2026-11-16
---

## Synopsis

```python run id=wsgi-app
from wsgiref.util import setup_testing_defaults

def app(environ, start_response):
    body = f"Hello from {environ['PATH_INFO']}\n".encode()
    start_response('200 OK', [('Content-Type', 'text/plain; charset=utf-8'),
                              ('Content-Length', str(len(body)))])
    return [body]

def start_response(status, headers, exc_info=None):
    print(status, headers)

environ = {'PATH_INFO': '/hello'}
setup_testing_defaults(environ)
print(b''.join(app(environ, start_response)))
```

```output for=wsgi-app
200 OK [('Content-Type', 'text/plain; charset=utf-8'), ('Content-Length', '18')]
b'Hello from /hello\n'
```

A WSGI application is a plain callable. The server calls it with a dict of request data and a `start_response` function, and the application returns the body as bytes. The example calls the application directly, as a server does. Frameworks such as Django and Flask build on this interface. ASGI is the asynchronous interface, and FastAPI and Starlette build on it.

## Semantics

**r1.** "The Web Server Gateway Interface (WSGI) is a standard interface between web server software and web applications written in Python."[^2] PEP 3333 "specifies a proposed standard interface between web servers and Python web applications or frameworks, to promote web application portability across a variety of web servers."[^1]

**r2.** "The application object is simply a callable object that accepts two arguments."[^1] "When called by the server, the application object must return an iterable yielding zero or more bytestrings."[^1] Example: [`wsgi-app`](#wsgi-app).

**r3.** The status is one string with the code and the reason phrase, in the form `999 Message here`.[^1] "response_headers is a list of (header_name, header_value) tuples describing the HTTP response header."[^1] Example: [`router`](#router).

**r4.** The standard library has the module `wsgiref`. "wsgiref is a reference implementation of the WSGI specification that can be used to add WSGI support to a web server or framework."[^2] It includes "a validation tool that checks WSGI servers and applications for conformance to the WSGI specification (PEP 3333)."[^2] Example: [`validate`](#validate).

**r5.** "ASGI is a spiritual successor to WSGI, the long-standing Python standard for compatibility between web servers, frameworks, and applications."[^3] "WSGI applications are a single, synchronous callable that takes a request and returns a response; this doesn’t allow for long-lived connections, like you get with long-poll HTTP or WebSocket connections."[^3]

**r6.** "ASGI is structured as a single, asynchronous callable."[^3] It takes a scope, "which is a dict containing details about the specific connection", and the two callables send and receive.[^3] Example: [`asgi-app`](#asgi-app).

**r7.** For HTTP, the application receives events of type `"http.request"`, and the `body` key holds the "Body of the request."[^4] The application sends one event of type `"http.response.start"` with the status and the headers, and then events of type `"http.response.body"`.[^4] The headers are byte strings, and "Header names must be lowercased."[^4] Example: [`asgi-app`](#asgi-app).

**r8.** "Django is a high-level Python web framework that encourages rapid development and clean, pragmatic design."[^5] "Because Django was developed in a fast-paced newsroom environment, it was designed to make common web development tasks fast and easy."[^6]

**r9.** "Flask is a lightweight WSGI web application framework."[^7] "Flask depends on the Werkzeug WSGI toolkit, the Jinja template engine, and the Click CLI toolkit."[^7]

**r10.** "FastAPI is a modern, fast (high-performance), web framework for building APIs with Python based on standard Python type hints."[^8] "FastAPI depends on Pydantic and Starlette."[^8] "Starlette is a lightweight ASGI framework/toolkit, which is ideal for building async web services in Python."[^9]

**r11.** "Jinja is a fast, expressive, extensible templating engine."[^10] "Special placeholders in the template allow writing code similar to Python syntax."[^10]

**r12.** The Python Developers Survey 2024 had "more than 30,000 Python developers and enthusiasts from almost 200 countries and regions".[^11] In its table of web frameworks, the 2024 column gives FastAPI 38%, Django 35% and Flask 34%.[^11] "Percentages are calculated within each column."[^11]

## Examples

A small router. A framework maps each path to a function and sets the status line (rules r2 and r3):

```python run id=router
def make_app(routes):
    def app(environ, start_response):
        handler = routes.get(environ['PATH_INFO'])
        if handler is None:
            start_response('404 Not Found', [('Content-Type', 'text/plain')])
            return [b'not found\n']
        start_response('200 OK', [('Content-Type', 'text/plain')])
        return [handler(environ).encode()]
    return app

app = make_app({'/': lambda env: 'home\n', '/about': lambda env: 'about\n'})

def call(app, path):
    seen = []
    body = b''.join(app({'PATH_INFO': path}, lambda s, h, e=None: seen.append(s)))
    return seen[0], body

for path in ['/', '/about', '/missing']:
    print(path, call(app, path))
```

```output for=router
/ ('200 OK', b'home\n')
/about ('200 OK', b'about\n')
/missing ('404 Not Found', b'not found\n')
```

The validator of `wsgiref` finds an application that returns `str` in place of bytes (rules r2 and r4):

```python run id=validate
from wsgiref.util import setup_testing_defaults
from wsgiref.validate import validator

def bad_app(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/plain')])
    return ['text, not bytes']

environ = {'QUERY_STRING': ''}
setup_testing_defaults(environ)
result = validator(bad_app)(environ, lambda s, h, e=None: None)
try:
    for chunk in result:
        pass
except AssertionError:
    print('the validator rejected the application')
finally:
    result.close()
```

```output for=validate
the validator rejected the application
```

The validator also checks that the caller closes the result, so the example calls `result.close()`.

An ASGI application with a fake receive and a fake send. A server such as an ASGI web server supplies the real ones (rules r6 and r7):

```python run id=asgi-app
import asyncio

async def app(scope, receive, send):
    assert scope['type'] == 'http'
    event = await receive()
    await send({'type': 'http.response.start', 'status': 200,
                'headers': [(b'content-type', b'text/plain')]})
    await send({'type': 'http.response.body', 'body': b'echo: ' + event['body']})

async def main():
    scope = {'type': 'http', 'method': 'POST', 'path': '/echo'}

    async def receive():
        return {'type': 'http.request', 'body': b'hi', 'more_body': False}

    async def send(message):
        print(message)

    await app(scope, receive, send)

asyncio.run(main())
```

```output for=asgi-app
{'type': 'http.response.start', 'status': 200, 'headers': [(b'content-type', b'text/plain')]}
{'type': 'http.response.body', 'body': b'echo: hi'}
```

## Pitfalls

Do not use the server of `wsgiref` in production. The documentation says: "wsgiref is a reference implementation and is not recommended for production. The module only implements basic security checks."[^2]

A WSGI application must return bytes, not `str` (rule r2). Encode the text before you return it, and run the validator in tests (example [`validate`](#validate)).

The status of WSGI is one string with the code and the reason, such as `'404 Not Found'` (rule r3). The status of ASGI is an `int`, such as `404`, in the `"http.response.start"` event (rule r7).

A WSGI application cannot keep a connection open for WebSocket (rule r5). Use an ASGI framework for WebSocket and for long-poll HTTP.

The survey numbers count the respondents who use each framework (rule r12). They do not measure which framework is right for a project.

## Comparison

In Go, the standard library has the HTTP server. "Package http provides HTTP client and server implementations."[^12] "A Handler responds to an HTTP request."[^12] A Go handler has the method `ServeHTTP(ResponseWriter, *Request)`. In Python, the standard library has `wsgiref`, and it "is not recommended for production."[^2]

[^1]: [PEP 3333: Python Web Server Gateway Interface v1.0.1](https://peps.python.org/pep-3333/)
[^2]: [wsgiref, WSGI Utilities and Reference Implementation](https://docs.python.org/3/library/wsgiref.html)
[^3]: [Introduction, ASGI documentation](https://asgi.readthedocs.io/en/latest/introduction.html)
[^4]: [HTTP & WebSocket ASGI Message Format](https://asgi.readthedocs.io/en/latest/specs/www.html)
[^5]: [Django](https://www.djangoproject.com/)
[^6]: [Django at a glance](https://docs.djangoproject.com/en/6.1/intro/overview/)
[^7]: [Welcome to Flask](https://flask.palletsprojects.com/en/stable/)
[^8]: [FastAPI](https://fastapi.tiangolo.com/)
[^9]: [Introduction, Starlette](https://starlette.dev/)
[^10]: [Introduction, Jinja Documentation](https://jinja.palletsprojects.com/en/stable/intro/)
[^11]: [Python Developers Survey 2024 Results](https://lp.jetbrains.com/python-developers-survey-2024/)
[^12]: [Package net/http](https://pkg.go.dev/net/http)
