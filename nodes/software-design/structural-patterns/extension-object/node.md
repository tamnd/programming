---
tag: 8N285T
kind: pattern
slug: extension-object
name: Extension object
aliases: [extension object pattern, extensible object, extension interface, adaptable object]
area: software-design/structural-patterns
level: advanced
summary: >-
  The Extension object pattern adds interfaces to an object without changing its class. A client asks the object for an extension by its type, and it gets the extension or nothing.
family: structural-design
polarity: positive
intent: "Let an object get new interfaces after its class is published: put each new interface in a separate extension object, and let clients ask the object for an extension at run time."
also_known_as: [Extensible object, Adaptable]
history:
  year: 1997
  people: [Erich Gamma]
  note: Erich Gamma described the Extension Object pattern in Pattern Languages of Program Design 3, edited by Robert Martin and others. In an interview of 2005, Gamma gave the IAdaptable interface of Eclipse as an example of the pattern.
see_also: [interfaces, adapter-pattern, decorator-pattern, marker-interface, runtime-reflection, dynamic-dispatch]
sources:
  - book: isbn:9780201310115
    title: "Robert C. Martin, Dirk Riehle and Frank Buschmann (eds.), Pattern Languages of Program Design 3, Addison-Wesley"
    locator: "Part 1, chapter 6: Extension Object, by Erich Gamma"
  - url: https://www.artima.com/articles/design-principles-from-design-patterns
    title: "Bill Venners, Design Principles from Design Patterns: A Conversation with Erich Gamma, Part III, Artima, June 6, 2005"
    locator: section on API evolution; footnote 4
  - url: https://help.eclipse.org/latest/topic/org.eclipse.platform.doc.isv/reference/api/org/eclipse/core/runtime/IAdaptable.html
    title: Interface IAdaptable, Eclipse Platform API Specification
    locator: interface description; getAdapter
  - url: https://learn.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-queryinterface(refiid_void)
    title: IUnknown::QueryInterface(REFIID,void**) method (unknwn.h), Microsoft Learn
    locator: description; Return value; Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/framework/wcf/extending/extensible-objects
    title: Extensible objects, Windows Communication Foundation, Microsoft Learn
    locator: The IExtensibleObject<T> Pattern; Extensible Objects in Windows Communication Foundation
  - url: https://github.com/eclipse-equinox/equinox/blob/R4_38/bundles/org.eclipse.equinox.common/src/org/eclipse/core/runtime/PlatformObject.java#L47-L74
    title: Eclipse Equinox R4_38 source, org/eclipse/core/runtime/PlatformObject.java
    locator: PlatformObject and getAdapter, lines 47 to 74
---

## Intent

Extension object is a structural pattern that lets an object take new interfaces without a change to its class. Each new interface is in a separate extension object. A client asks the object for an extension at run time and uses it if it is there.[^1][^2]

## Problem

When a published interface gets a new method, all of its clients break. Erich Gamma gives the rule that a published interface is immutable, so a new method must go in a separate interface. Eclipse has such extension interfaces, for example `IWorkbenchPart2`, which adds methods to `IWorkbenchPart`. The cost is that a caller must find out at run time whether an object implements the extension interface.[^2]

The same problem comes when different clients need different services from one object. If every service goes into the class of the object, the class gets large, and each client depends on services that it does not use.

## Solution

The subject keeps its own small interface and one more operation: get the extension of a given type. The operation returns the extension object, or nothing if the subject has no such extension.

```text
   Client
     |  getExtension(WordCount)
     v
   Subject -------- extensions --------+-----------------+
   getExtension(type)                  |                 |
                                   WordCount           Upper
                                   (extension)       (extension)
                                   holds the subject  holds the subject
```

The pattern has many forms in real systems:

- In Eclipse, an object that implements `IAdaptable` has the method `getAdapter(Class<T> adapter)`. It returns an object of the given class, or null if it has no such adapter. Adaptable objects "can be dynamically extended to provide different interfaces".[^3]
- In COM, `QueryInterface` asks an object for a pointer to one of its interfaces, by an interface identifier. It returns `S_OK` if the object supports the interface and `E_NOINTERFACE` if not.[^4]
- In Windows Communication Foundation, an extensible object holds a collection of `IExtension<T>` objects. `Find` returns an extension of the given type, or null.[^5]

## Example

The example files show one program in each language. A `Document` holds text and a collection of extensions that is keyed by type. A `WordCount` extension counts the words. An `Upper` extension gives the text in capitals, and it is added only later. The client asks for `Upper` before and after the extension is added. The program prints:

```text
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

## Consequences

The benefits:

- New interfaces come without a change to the published interface of the subject, so the clients of the subject do not break.[^2]
- Each client sees only the extensions that it asks for.
- Extensions can be added at run time. An Eclipse adaptable object can get new adapters from adapter factories.[^3]

The costs:

- **A run-time check.** The caller must find out at run time whether the subject has the extension, and it must handle the case where it does not. `getAdapter` can return null, and `QueryInterface` can return `E_NOINTERFACE`.[^2][^3][^4]
- **Rules for the lookup.** COM requires that the set of interfaces of an object is static, and that `QueryInterface` is reflexive, symmetric and transitive.[^4] A homemade lookup has no such rules unless the design gives them.
- **Ambiguity.** If several extensions in a WCF collection have the same type, `Find` returns one of them.[^5]

The key of the lookup matters. Here the extensions are keyed by name, and the client asks with a spelling that is a little different. The lookup does not fail with an error. It returns nothing, and the client reports that the document has no word count:

```python wrong id=string-key
class Document:
    def __init__(self, text):
        self.text = text
        self._extensions = {}

    def add_extension(self, name, extension):
        self._extensions[name] = extension

    def get_extension(self, name):
        return self._extensions.get(name)


class WordCount:
    def __init__(self, doc):
        self.doc = doc

    def count(self):
        return len(self.doc.text.split())


doc = Document("the quick brown fox")
doc.add_extension("word_count", WordCount(doc))
counter = doc.get_extension("wordcount")
print("words: not supported" if counter is None else f"words: {counter.count()}")
```

```output for=string-key
words: not supported
```

Eclipse and WCF key the lookup by type, as in `getAdapter(Class<T> adapter)` and in the `Find` method of `IExtensionCollection<T>`.[^3][^5] A misspelled class name is then an error at the call, not a silent miss:

```python run id=type-key
class Document:
    def __init__(self, text):
        self.text = text
        self._extensions = {}

    def add_extension(self, extension):
        self._extensions[type(extension)] = extension

    def get_extension(self, kind):
        return self._extensions.get(kind)


class WordCount:
    def __init__(self, doc):
        self.doc = doc

    def count(self):
        return len(self.doc.text.split())


doc = Document("the quick brown fox")
doc.add_extension(WordCount(doc))
counter = doc.get_extension(WordCount)
print("words: not supported" if counter is None else f"words: {counter.count()}")
try:
    doc.get_extension(WordCounter)
except NameError as e:
    print(f"error: {e}")
```

```output for=type-key
words: 4
error: name 'WordCounter' is not defined
```

## When not to use it

- The set of interfaces is known and small. The class can implement them directly, and clients need no run-time check.
- The interface is not published, so its clients can change with it.
- The language can add methods to an existing interface without breaking clients. Gamma notes that in Java, an abstract class can get a new method with a default implementation, and all clients continue to work.[^2]

## Variants and related patterns

**Adapter factories.** In Eclipse, adapters are created by adapter factories, which adapter managers keep by type. The subject does not need to store its extensions, and `PlatformObject` gives a default `getAdapter` that asks the adapter manager.[^3][^6]

**Type-based dictionary.** WCF uses the extensions collection of an object as a dictionary keyed by type, so that one component can store state on an object and another component can find it.[^5]

**Adapter.** An extension that gives a subject a new interface is an [[adapter-pattern]] that the subject itself hands out. Eclipse calls its extensions adapters.[^3]

**Marker interface.** A [[marker-interface]] is a type check with no methods. An extension lookup is a type check that also returns an object with methods.

## Known uses

- Eclipse: [PlatformObject.java at R4_38](https://github.com/eclipse-equinox/equinox/blob/R4_38/bundles/org.eclipse.equinox.common/src/org/eclipse/core/runtime/PlatformObject.java#L47-L74) declares `public abstract class PlatformObject implements IAdaptable`, and its `getAdapter` returns `AdapterManager.getDefault().getAdapter(this, adapter)`.[^6] Gamma named `IAdaptable` as an example of the Extension Object pattern.[^2]
- COM: a client calls `QueryInterface` on a COM object to get a pointer to another of its interfaces, by the interface identifier.[^4]
- Windows Communication Foundation: `ServiceHostBase`, `InstanceContext`, `OperationContext` and `IContextChannel` are extensible objects.[^5]

## References
