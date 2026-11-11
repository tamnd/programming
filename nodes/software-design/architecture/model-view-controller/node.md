---
tag: 8SSG44
kind: pattern
slug: model-view-controller
name: Model-view-controller
aliases: [MVC, Model View Controller]
area: software-design/architecture
level: core
summary: >-
  MVC splits an interactive program into a model that holds the data and the rules, views that present the model, and controllers that turn user input into requests to the model.
family: architectural
polarity: positive
intent: "Splits user interface interaction into three distinct roles."
also_known_as: [Models-Views-Controllers]
history:
  year: 1979
  people: [Trygve Reenskaug]
  note: Reenskaug defined the terms model, view and controller in a technical note dated 10 December 1979. Krasner and Pope described the MVC framework of Smalltalk-80 in 1988. Web frameworks such as Rails and ASP.NET Core later used the three names for a request and response design.
see_also: [observer-pattern, composite-pattern, strategy-pattern, mediator-pattern, command-pattern, cross-site-scripting, unit-testing, dependency-injection]
sources:
  - url: https://zenodo.org/records/3676092
    title: Trygve Reenskaug, Models-Views-Controllers, technical note, 10 December 1979 (doi:10.5281/zenodo.3676092)
    locator: sections MODELS, VIEWS, CONTROLLERS and EDITORS
  - paper: Glenn E. Krasner and Stephen T. Pope, A cookbook for using the model-view controller user interface paradigm in Smalltalk-80, Journal of Object-Oriented Programming 1(3), 1988, pages 26 to 49
    title: Krasner and Pope, A cookbook for using the model-view controller user interface paradigm in Smalltalk-80, JOOP 1(3), 1988
    locator: the dependency mechanism between a model and its views (changed and update messages)
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 1, Introduction, section Design Patterns in Smalltalk MVC
  - url: https://martinfowler.com/eaaCatalog/modelViewController.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Model View Controller
    locator: pattern summary; the book's Chapter 14
  - url: https://martinfowler.com/eaaDev/uiArchs.html
    title: Martin Fowler, GUI Architectures, 18 July 2006
    locator: sections Model View Controller, VisualWorks Application Model, Model-View-Presenter (MVP) and Humble View
  - url: https://learn.microsoft.com/en-us/aspnet/core/mvc/overview#mvc-pattern
    title: Microsoft Learn, Overview of ASP.NET Core MVC
    locator: MVC pattern; Model Responsibilities; Controller Responsibilities
  - url: https://guides.rubyonrails.org/action_controller_overview.html#introduction
    title: Ruby on Rails Guides, Action Controller Overview
    locator: 1. Introduction
  - url: https://docs.djangoproject.com/en/5.2/faq/general/
    title: Django 5.2 documentation, FAQ, General
    locator: Django appears to be a MVC framework, but you call the Controller the "view", and the View the "template". How come you don't use the standard names?
  - url: https://www.oracle.com/java/technologies/a-swing-architecture.html
    title: Amy Fowler, A Swing Architecture Overview, Oracle Java SE technical article
    locator: Swing architecture is loosely based on the traditional MVC design (separable model architecture)
  - url: https://docs.python.org/3.14/library/html.html#html.escape
    title: The Python Standard Library, html, Python 3.14
    locator: html.escape
  - url: https://github.com/openjdk/jdk/blob/jdk-21-ga/src/java.desktop/share/classes/javax/swing/JTable.java#L3744-L3756
    title: OpenJDK jdk-21-ga source, javax/swing/JTable.java
    locator: JTable.setModel, lines 3744 to 3756
---

## Intent

Splits user interface interaction into three distinct roles.[^4] The model holds the data and the rules of the problem. A view presents the model to the user. A controller takes the input of the user and turns it into requests. The model does not know how it is shown or how the user changes it.

## Problem

A to-do list program shows a list of items and a line that counts the open and the finished items. The user types commands. The first version keeps the items in the window code. Each command handler changes the list, redraws the list, and recomputes the count.

After a few months the program has a second window with a chart, a command line mode, and a set of tests. The rule "an item is done only once" is in the window code, so the command line mode has a second copy of it. The tests must create a window to test a rule about items. Each command handler must know every window that shows the items, and a handler that forgets one leaves a stale screen.

The forces are these. The rules about items should exist once, in code that has no screen. Several presentations of the same data should stay consistent. The code that reads input should not also format output. And a person should be able to add a presentation without a change to the rules.

## Solution

Reenskaug's note of 1979 gives the three roles.[^1]

- "Models represent knowledge." A model can be one object or a structure of objects. Its parts should correspond to the parts of the problem, not to details of the screen.
- "A view is a (visual) representation of its model." It shows some attributes and hides others, so it acts as "a presentation filter". It gets its data from the model by asking questions in the terms of the model.
- "A controller is the link between a user and the system." It arranges the views on the screen, gives the user menus and other means to give commands, and translates the input of the user into messages.

The note also states two limits. A controller should never supplement the views, and a view should never know about user input such as mouse operations and keystrokes.[^1]

In the Smalltalk-80 framework, the model keeps a list of dependents, and each view registers as a dependent of its model. When the model changes, it sends a changed message, and each dependent gets an update message and asks the model for the data it needs.[^2] This is the [[observer-pattern]]. Fowler calls this use Observer Synchronization: the controller changes the model, and the views follow without the controller knowing which views exist.[^5]

```text
              user input
                  |
                  v
            +------------+   requests    +---------+
            | Controller | ------------> |  Model  |
            +------------+               +---------+
                                           |    ^
                         "I changed"       |    |  queries
                       (observer list)     v    |
                                         +---------+
                                         |  View   | ---> screen
                                         +---------+
```

The model knows its views only as observers. It does not import any view class. The arrows from the controller and the view point to the model, and no arrow leaves the model except the notification.

Design Patterns uses Smalltalk MVC in its first chapter to show patterns that work together. The subscribe and notify protocol between a model and its views is Observer. Views can nest, and a composite view contains other views, which is [[composite-pattern]]. A view delegates its response to input to a controller object, and a different controller gives a different response, which is [[strategy-pattern]].[^3]

Fowler names the most influential idea of MVC Separated Presentation: a clear division between the domain objects and the presentation objects, where the domain objects do not refer to the presentation.[^5] The view and controller split matters less. Many toolkits put both roles in one object. Swing collapses the view and the controller of each component into a single UI object, and keeps the model separate. Its documentation calls this a "separable model architecture".[^9]

### The web form

Web frameworks use the same three names for a different flow. A request arrives. A router picks a controller method. The controller works with the model, then chooses a view and gives it the data. The view renders HTML. The response goes back, and the request is over.[^6][^7] No view observes the model, because no view lives longer than one request.

ASP.NET Core states the dependency rule: both the view and the controller depend on the model, and the model depends on neither.[^6] It also asks for thin controllers: business logic belongs in the model, not in the controller.[^6]

The following program sends six requests to a router in memory. The model has no HTML and no HTTP. The view functions escape the text of the items with `html.escape`, so a title with markup is shown as text.[^10]

```python run id=web
import re
from html import escape


class TodoModel:
    def __init__(self) -> None:
        self.items: list[dict] = []

    def add(self, title: str) -> None:
        if not title:
            raise ValueError("a title is required")
        self.items.append({"title": title, "done": False})

    def complete(self, number: int) -> None:
        if not 1 <= number <= len(self.items):
            raise LookupError(f"no item {number}")
        self.items[number - 1]["done"] = True


# Views: data in, HTML out. No input handling, no rules.
def list_page(items: list[dict]) -> str:
    rows = "".join(f"<li>{'[x]' if i['done'] else '[ ]'} {escape(i['title'])}</li>" for i in items)
    return f"<ul>{rows}</ul>"


def error_page(message: str) -> str:
    return f"<p>{escape(message)}</p>"


class TodoController:
    def __init__(self, model: TodoModel) -> None:
        self.model = model

    def index(self, form: dict) -> tuple[int, str]:
        return 200, list_page(self.model.items)

    def create(self, form: dict) -> tuple[int, str]:
        self.model.add(form.get("title", ""))
        return 303, "/todos"

    def complete(self, form: dict, number: str) -> tuple[int, str]:
        try:
            self.model.complete(int(number))
        except LookupError as err:
            return 404, error_page(str(err))
        return 303, "/todos"


controller = TodoController(TodoModel())
routes = [
    ("GET", r"/todos", controller.index),
    ("POST", r"/todos", controller.create),
    ("POST", r"/todos/(\d+)/done", controller.complete),
]


def dispatch(method: str, path: str, form: dict) -> tuple[int, str]:
    for route_method, pattern, action in routes:
        match = re.fullmatch(pattern, path)
        if match and method == route_method:
            return action(form, *match.groups())
    return 404, error_page(f"no route for {method} {path}")


requests = [
    ("POST", "/todos", {"title": "buy milk"}),
    ("POST", "/todos", {"title": "<b>tea</b>"}),
    ("POST", "/todos/1/done", {}),
    ("POST", "/todos/7/done", {}),
    ("GET", "/todos", {}),
    ("GET", "/admin", {}),
]
for method, path, form in requests:
    status, body = dispatch(method, path, form)
    print(f"{method} {path} -> {status} {body}")
```

```output
POST /todos -> 303 /todos
POST /todos -> 303 /todos
POST /todos/1/done -> 303 /todos
POST /todos/7/done -> 404 <p>no item 7</p>
GET /todos -> 200 <ul><li>[x] buy milk</li><li>[ ] &lt;b&gt;tea&lt;/b&gt;</li></ul>
GET /admin -> 404 <p>no route for GET /admin</p>
```

The controller answers a POST with a redirect, status 303, instead of a page. The browser then sends a GET for the list. The controller methods stay small: each one reads the request, calls the model once, and picks a response. The escaping is in the view, because only the view knows that the output is HTML. See [[cross-site-scripting]].

Django uses other names for the same parts. A Django "view" is the Python function for a URL, and it decides which data is presented. A template decides how the data is presented. The Django FAQ says that the controller is "probably the framework itself", the machinery that sends a request to the view for its URL, and suggests the name "MTV", for model, template and view.[^8]

## Example

Each example file builds the to-do program of the problem section in the classic form. A `TodoModel` holds the items and a list of observers. A list view and a summary view subscribe to the model. A controller reads five command lines, prints each one, and calls `add` or `complete` on the model. The model then tells both views, and each view prints its part. The controller prints the errors that the model reports, and it rejects a line that it cannot read.

The views never receive a call from the controller. When the controller adds an item, the list and the summary update because they observe the model. All eight programs print the same output. The notes in each file show how the language holds the observer list: a list of callables in Python, an `EventTarget` in JavaScript, an `event` in C#, a slice of functions in Go, `std::function` in C++, and boxed closures that receive a slice in Rust.

## Consequences

**Benefits.**

- **The rules live in one place, without a screen.** The model has no imports from the user interface, so a test can call `add` and `complete` and check the result with no window and no server.[^5][^6]
- **Several views of one model stay consistent.** Fowler describes the classic MVC example: a spreadsheet window and graphs of the same data in separate windows. The spreadsheet window does not need to know which other windows are open. It changes the model, and the observers update.[^5]
- **A view can be added without a change to the model or the controller.** In the example files, a third view is one more subscription.
- **Each part changes for its own reasons.** ASP.NET Core gives the reason: user interface logic tends to change more often than business logic, and code that mixes the two must change each time the user interface changes.[^6]

**Liabilities.**

**The flow of control is implicit.** Fowler writes that the problem with Observer Synchronization "is the core problem of the observer pattern itself - you can't tell what is happening by reading the code".[^5] A change to the model can start a chain of updates that no single method shows. The [[observer-pattern]] node covers the failure modes: update order, updates that change the model again, and observers that are never removed.

The alternative, where the controller updates each view itself, has the opposite fault. Fowler calls it Flow Synchronization.[^5] The controller must know every view, and a controller that forgets one view leaves it stale:

```python wrong id=forgotten-view
class TodoModel:
    def __init__(self):
        self.items = []

    def add(self, title):
        self.items.append(title)


class ListView:
    def __init__(self, model):
        self.model = model
        self.text = ""

    def refresh(self):
        self.text = ", ".join(self.model.items)


class CountView:
    def __init__(self, model):
        self.model = model
        self.text = ""

    def refresh(self):
        self.text = f"{len(self.model.items)} items"


class Controller:
    def __init__(self, model, list_view, count_view):
        self.model = model
        self.list_view = list_view
        self.count_view = count_view

    def add(self, title):
        self.model.add(title)
        self.list_view.refresh()
        # bug: self.count_view.refresh() is missing


model = TodoModel()
list_view, count_view = ListView(model), CountView(model)
list_view.refresh()
count_view.refresh()
controller = Controller(model, list_view, count_view)
controller.add("buy milk")
controller.add("write report")
print("list: ", list_view.text)
print("count:", count_view.text)
```

```output
list:  buy milk, write report
count: 0 items
```

The model holds two items, and the count view still shows the state before the first command. With observers, the count view would subscribe once, and no controller method could forget it.

**Controllers grow.** In web MVC, the controller is the entry point for each request, and logic that has no other home lands there. ASP.NET Core advises that controllers should not have too many responsibilities, and that business logic belongs in the domain model.[^6] A controller that validates, computes prices and sends mail has become the model, and the rules are again tied to the user interface.

**The names do not mean one thing.** Fowler calls MVC "one of the most quoted (and most misquoted) patterns around".[^4] In Smalltalk, the controller handles mouse and keyboard input for one view. In Rails, the controller handles an HTTP request and picks a template.[^7] In Django, the same role is the framework itself, and a "view" is what other frameworks call a controller.[^8] A discussion of MVC should say which form it means.

**The view and controller split often has no value.** Widget toolkits handle the raw input in the widget. Swing puts the view and the controller of a component into one UI object.[^9] A separate controller class for each widget adds code and no separation.

## When not to use it

**For a program with one output and no interaction.** A script that reads a file and prints a report has no user input to interpret and one presentation. A function that computes the data and a function that formats it give the separation that matters.

**When the toolkit gives a different split.** A toolkit with data binding, or one that gives each widget its own input handling, expects a different structure, such as a presenter or a view model. Forcing a Smalltalk-style controller onto each widget fights the toolkit. Fowler writes that the parts of MVC that clearly worked, and stayed as Smalltalk developed, were Separated Presentation and Observer Synchronization.[^5] Keep those, and use the structure the toolkit expects for the rest.

**As a reason to split thin code into three files.** A form that reads two fields and writes one row has almost no model. Three classes for it add indirection without a rule to protect.

## Variants and related patterns

**Application Model.** Classic MVC had no good place for view logic and view state, such as whether a button is enabled. VisualWorks Smalltalk added the Application Model, a construct that moves towards Presentation Model, to hold them.[^5]

**Model-View-Presenter.** The widgets handle the user's input themselves and pass it to a presenter, which works with the model. Fowler describes two forms. In Supervising Controller, the view handles the presentation that it can describe declaratively, and the presenter handles the complex cases. In Passive View, the presenter does all the work on the widgets, and the widgets do not see the model.[^5]

**Humble View.** Move as much behavior as possible out of the objects that are hard to test, so that the screen is a thin layer that a test can replace. Passive View and Presentation Model are two ways to do it.[^5] With [[dependency-injection]], a test gives the presenter a fake view and checks what it receives. See [[unit-testing]].

**Web MVC, MTV.** The request and response form of the solution section. A router picks a controller, the controller calls the model and picks a view, and the view renders the response.[^6][^7] Django names the same parts model, template and view.[^8]

**Observer.** The link between the model and its views in the classic form. See [[observer-pattern]].

**Composite.** Nested views, where a window contains panes that contain views. See [[composite-pattern]].

**Strategy.** A view delegates its response to input to a controller, and a different controller changes the response without a change to the view.[^3] See [[strategy-pattern]].

**Mediator.** The controller of a form that holds the rules joining its widgets plays the role of a mediator. See [[mediator-pattern]].

**Command.** A controller can turn each user action into a command object. Each command can then be queued, logged or undone. See [[command-pattern]].

## Known uses

- Smalltalk-80: the MVC framework, with views registered as dependents of a model, as Krasner and Pope describe.[^2]
- Java Swing: [JTable.java at jdk-21-ga](https://github.com/openjdk/jdk/blob/jdk-21-ga/src/java.desktop/share/classes/javax/swing/JTable.java#L3744-L3756). `setModel` removes the table from the listeners of the old `TableModel` and adds it to the listeners of the model it receives. The table observes its model, and the model knows only the `TableModelListener` interface.[^11][^9]
- Ruby on Rails: Action Controller is "the C in the Model View Controller (MVC) pattern". After the router matches a controller to a request, the controller fetches or saves data through a model and uses a view to create the output.[^7]
- ASP.NET Core MVC: requests are routed to a controller, which works with the model and chooses the view.[^6]
- Django: model, template and view, with the URL configuration in the role of the controller.[^8]

## References
