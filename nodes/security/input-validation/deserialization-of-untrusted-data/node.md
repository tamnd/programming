---
tag: YVTK68
kind: weakness
slug: deserialization-of-untrusted-data
name: Deserialization of untrusted data
aliases: [insecure deserialization, unsafe deserialization, object injection, unmarshalling, unpickling, cwe-502]
area: security/input-validation
level: core
summary: >-
  Deserialization of untrusted data happens when a program rebuilds objects from bytes that an attacker controls, in a format that lets the bytes choose the classes to create or the code to run.
cwe: 502
abstraction: base
languages_affected: [java, python, csharp, php, ruby, javascript]
owasp_top10_2025: A08
cwe_top25_2025: 15
history:
  year: 2016
  people: [Roger Riggs]
  note: JEP 290, "Filter Incoming Serialization Data", proposed a filter that the JDK calls with the class of each object in a stream before the object is created, so that a program can reject classes it does not expect.
sources:
  - url: https://cwe.mitre.org/data/definitions/502.html
    title: "CWE-502: Deserialization of Untrusted Data"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Background Details; Relationships; Detection Methods; Observed Examples
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 15, CVEs in KEV, Rank Last Year
  - url: https://owasp.org/Top10/2025/A08_2025-Software_or_Data_Integrity_Failures/
    title: "OWASP Top 10:2025, A08:2025 Software or Data Integrity Failures"
    locator: Description; How to prevent; Scenario #4; List of Mapped CWEs
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Deserialization_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Deserialization Cheat Sheet
    locator: What is Deserialization; Python, Clear-box Review; Java, Harden Your Own java.io.ObjectInputStream; Language-Agnostic Methods for Deserializing Safely
  - url: https://docs.python.org/3/library/pickle.html
    title: The Python Standard Library, pickle
    locator: the warning at the top; Restricting Globals; Unpickler.find_class
  - url: https://openjdk.org/jeps/290
    title: "JEP 290: Filter Incoming Serialization Data"
    locator: Summary; Motivation; Description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/io/ObjectInputFilter.html
    title: Java SE 25 API, java.io.ObjectInputFilter
    locator: the interface description, the filter pattern example
  - url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/binaryformatter-security-guide
    title: .NET documentation, Deserialization risks in use of BinaryFormatter and related types
    locator: the caution at the top; Deserialization vulnerabilities; BinaryFormatter security vulnerabilities
  - url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/polymorphism
    title: .NET documentation, How to serialize properties of derived classes with System.Text.Json
    locator: Polymorphic type discriminators
  - url: https://pkg.go.dev/encoding/json#Decoder.DisallowUnknownFields
    title: Go standard library, encoding/json
    locator: Decoder.DisallowUnknownFields
  - url: https://cwe.mitre.org/data/definitions/1321.html
    title: "CWE-1321: Improperly Controlled Modification of Object Prototype Attributes ('Prototype Pollution')"
    locator: Description; Demonstrative Examples, Example 1
  - url: https://codeql.github.com/codeql-query-help/python/py-unsafe-deserialization/
    title: CodeQL query help, Deserialization of user-controlled data (py/unsafe-deserialization)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/java/java-unsafe-deserialization/
    title: CodeQL query help, Deserialization of user-controlled data (java/unsafe-deserialization)
    locator: the query description; Recommendation
  - url: https://codeql.github.com/codeql-query-help/javascript/js-unsafe-deserialization/
    title: CodeQL query help, Deserialization of user-controlled data (js/unsafe-deserialization)
    locator: the query description
  - url: https://www.cve.org/CVERecord?id=CVE-2015-4852
    title: CVE-2015-4852, Oracle WebLogic Server
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2017-9805
    title: CVE-2017-9805, Apache Struts REST plugin
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-37052
    title: CVE-2024-37052, MLflow
    locator: the description and the publication date
  - url: https://cwe.mitre.org/data/definitions/915.html
    title: "CWE-915: Improperly Controlled Modification of Dynamically-Determined Object Attributes"
    locator: Description
---

## Description

Serialization turns objects into bytes, so that a program can store them or send them. Deserialization reads the bytes and rebuilds the objects.[^1] A pure data format such as JSON rebuilds only numbers, strings, lists and maps. A native object format, such as `pickle` in Python, `java.io` serialization in Java or `BinaryFormatter` in .NET, rebuilds objects of any class that the bytes name. Some formats also run code of those classes while they rebuild, such as `readObject` in Java or a call that `pickle` records in the stream.[^4][^5]

The root cause is that the bytes decide what the program does. A developer thinks of the bytes as data: "this cookie holds the settings that the server wrote". The deserializer treats them as instructions: "import this class, call this function with these arguments". When an attacker can change the bytes, the attacker chooses the instructions. The Python documentation says that by default, unpickling will import any class or function that it finds in the data.[^5] Microsoft compares a call to `BinaryFormatter.Deserialize` with launching the payload as a standalone executable.[^8]

The weakness is common because the native format is the shortest path. One call saves a whole object graph, and one call loads it back. The danger is not in the code of the program. It is in every class that the program can load, including the classes of its libraries. CWE-502 calls a sequence of instances and method calls that run during deserialization a gadget. It warns that more gadgets are found all the time, so an allowlist of classes alone is not sufficient.[^1] A type check after the load comes too late, because the code of the classes has already run.

The consequences vary with the classes that the program can load. CWE-502 lists changed data and unexpected state, denial of service, and in many cases the execution of code.[^1] CWE-502 is a base weakness, a child of CWE-913, improper control of dynamically-managed code resources. CWE-915, the uncontrolled change of object attributes, is its peer.[^1] It is rank 15 in the 2025 CWE Top 25, with 11 CVEs in the CISA catalogue of known exploited vulnerabilities, up from rank 16 the year before.[^2] The OWASP Top 10:2025 maps it to A08:2025 Software or Data Integrity Failures.[^3]

### History

On November 18, 2015, CVE-2015-4852 recorded that a crafted serialized Java object sent to Oracle WebLogic Server let remote attackers run commands. The record names a copy of the Apache Commons Collections library in the server.[^15] The named file is a library. A native deserializer can load any class on the class path, so the bytes can reach the code of every library, not only the code of the server.[^1] In 2016, Roger Riggs wrote JEP 290, "Filter Incoming Serialization Data". It added a filter interface to `ObjectInputStream`. The JDK calls the filter with the class of each object in the stream before it creates the object, and the filter accepts or rejects the class.[^6]

## Vulnerable code

A server keeps the settings of a user in a cookie. It writes the cookie with `pickle` and reads it back with `pickle.loads`. The second cookie is the classic demonstration input: the hand-made stream from the `pickle` documentation, with the harmless function `print` in place of the shell command of the documentation.[^5]

```python wrong id=pickle-cookie
import base64
import pickle


def load_settings(cookie: str) -> dict:
    return pickle.loads(base64.b64decode(cookie))


cookie = base64.b64encode(pickle.dumps({"theme": "dark"})).decode()
print(load_settings(cookie))

# The demonstration stream from the pickle documentation, with print.
crafted = b"cbuiltins\nprint\n(S'code ran during loading'\ntR."
print(load_settings(base64.b64encode(crafted).decode()))
```

```output for=pickle-cookie
{'theme': 'dark'}
code ran during loading
None
```

The program exits normally, and the output is the problem. The stream does not hold settings. It tells the unpickler to import `builtins.print` and to call it with a string. The call runs inside `pickle.loads`, before `load_settings` returns. Any function that the process can import could be in its place. The page does not show one.

## Fixed code

The fix uses a pure data format and builds the result from the fields that the program expects. The OWASP cheat sheet says that a switch to a pure data format such as JSON is a large reduction of risk.[^4] CWE-502 asks to populate a fresh object, so that the data flows through input validation.[^1]

```python run id=json-cookie
import base64
import json

THEMES = {"dark", "light"}


def load_settings(cookie: str) -> dict:
    data = json.loads(base64.b64decode(cookie))
    if not isinstance(data, dict) or set(data) != {"theme"}:
        raise ValueError("unexpected fields")
    if data["theme"] not in THEMES:
        raise ValueError("unknown theme")
    return {"theme": data["theme"]}


cookie = base64.b64encode(json.dumps({"theme": "dark"}).encode()).decode()
print(load_settings(cookie))

crafted = b"cbuiltins\nprint\n(S'code ran during loading'\ntR."
try:
    load_settings(base64.b64encode(crafted).decode())
except ValueError as e:
    print("refused:", type(e).__name__)
```

```output for=json-cookie
{'theme': 'dark'}
refused: JSONDecodeError
```

`json.loads` cannot import or call anything, so the crafted stream is only a syntax error. The program then accepts exactly one field with one of two values. The validation is part of the fix. A pure data format stops the code from running, but the values can still be wrong.[^4]

### When the format must stay

Some programs cannot change the format at once. Two controls then reduce the risk, and neither one makes untrusted bytes safe.

The first control is to accept only data that the program wrote itself. CWE-502 and OWASP both recommend a signature or an HMAC over the bytes, checked before the bytes go to the deserializer.[^1][^3] An attacker without the key cannot make a valid tag. The `pickle` documentation suggests the same, with `hmac`.[^5] The Python example file shows it.

The second control is an allowlist of classes, checked before each object is created. In Python, a subclass of `Unpickler` overrides `find_class` and refuses every global that is not on the list.[^5] In Java, an `ObjectInputFilter` does the same. The pattern below allows one class and rejects all others with `!*`.[^7]

```java run id=serial-filter
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InvalidClassException;
import java.io.ObjectInputFilter;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;

public class Main {
    record Settings(String theme) implements Serializable {}

    static byte[] save(Object value) throws IOException {
        var bytes = new ByteArrayOutputStream();
        try (var out = new ObjectOutputStream(bytes)) {
            out.writeObject(value);
        }
        return bytes.toByteArray();
    }

    static Object load(byte[] data) throws IOException, ClassNotFoundException {
        try (var in = new ObjectInputStream(new ByteArrayInputStream(data))) {
            in.setObjectInputFilter(ObjectInputFilter.Config.createFilter("Main$Settings;!*"));
            return in.readObject();
        }
    }

    public static void main(String[] args) throws Exception {
        System.out.println(load(save(new Settings("dark"))));
        try {
            load(save(new ArrayList<String>()));
        } catch (InvalidClassException e) {
            System.out.println("refused: " + e.getMessage());
        }
    }
}
```

```output for=serial-filter
Settings[theme=dark]
refused: filter status: REJECTED
```

The filter runs before the object is created, so the code of a rejected class never runs.[^6] CWE-502 still calls an allowlist alone not sufficient, because a class on the list can itself be a gadget.[^1]

## How to detect it

- **Static analysis.** CWE-502 rates automated static analysis as highly effective.[^1] CodeQL queries `py/unsafe-deserialization`, `java/unsafe-deserialization` and `js/unsafe-deserialization` follow data from a request to a deserializer.[^12][^13][^14]
- **Code review.** Search for `pickle.load`, `pickle.loads`, `ObjectInputStream.readObject`, `BinaryFormatter`, and YAML loaders that are not the safe loader.[^4] For each call, find where the bytes come from. A file, a cookie, a cache, a queue and a model file are all inputs if another party can write them.
- **Traffic review.** The OWASP cheat sheet lists signs of native formats in traffic. Base64 data that starts with `rO0` is a Java object stream, and data that starts with `gASV` is often a pickle.[^4][^3]
- **Dependency review.** List the libraries on the class path or in the environment. Each library adds classes that a native deserializer can load.[^1]

## How to prevent it

- **Do not deserialize untrusted data with a native object format.** The `pickle` documentation says to unpickle only data that you trust.[^5] Microsoft says that `BinaryFormatter` is insecure and cannot be made secure.[^8]
- **Use a pure data format and map it to declared types.** Parse JSON into a data-transfer object that has only the fields the operation needs, then validate each field.[^4][^1] In .NET, `System.Text.Json` creates a derived type only when the code declares it with a type discriminator.[^9] In Go, `Decoder.DisallowUnknownFields` makes the decoder refuse keys that the struct does not have.[^10]
- **Sign the data that you write, and verify before you read.** An HMAC or a digital signature shows that the bytes came from the expected source and were not changed.[^1][^3]
- **Allowlist the classes.** Use `ObjectInputFilter` in Java and `find_class` in Python, and reject everything else.[^7][^5] Keep the list short, and treat it as defence in depth.[^1]
- **Watch for attribute injection.** A merge of parsed data into an object can still change fields that the program did not expect. CWE-915 and, in JavaScript, CWE-1321 describe this, and the example files show it.[^18][^11]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2015-4852 | published 2015-11-18 | The WLS Security component of Oracle WebLogic Server let remote attackers run commands with a crafted serialized Java object in T3 protocol traffic. The record names a copy of Apache Commons Collections in the server.[^15][^1] |
| CVE-2017-9805 | published 2017-09-15 | The REST plugin of Apache Struts deserialized XML with XStream and no type filtering, which could lead to remote code execution.[^16] |
| CVE-2024-37052 | published 2024-06-04 | MLflow deserialized untrusted data, so a scikit-learn model that an attacker uploaded could run code on the system of a user who interacted with it.[^17][^1] |

## References
