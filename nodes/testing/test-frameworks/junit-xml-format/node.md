---
tag: Z94DJ1
kind: format
slug: junit-xml-format
name: JUnit XML format
aliases: [JUnit XML, JUnit report, JUnitXML, Ant JUnit XML, Surefire XML report, xunit XML]
area: testing/test-frameworks
level: core
summary: >-
  JUnit XML is the de facto XML format for test reports. A testsuite element holds testcase elements, and a failure, error or skipped child marks a test that did not pass. CI services read it.
see_also: [junit, junit-5, pytest, test-framework, tap-format, xml, node-test, bats]
sources:
  - url: https://docs.junit.org/6.1.3/advanced-topics/junit-platform-reporting.html
    title: JUnit User Guide 6.1.3, JUnit Platform Reporting
    locator: Legacy XML format; Open Test Reporting
    accessed: 2026-11-16
  - url: https://ant.apache.org/manual/Tasks/junit.html
    title: Apache Ant manual, JUnit task
    locator: Nested elements, formatter
    accessed: 2026-11-16
  - url: https://plugins.jenkins.io/junit/
    title: Jenkins plugins, JUnit
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.gitlab.com/ci/testing/unit_test_reports/
    title: GitLab Docs, Unit test reports
    locator: the introduction; JUnit XML format specification
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/azure/devops/pipelines/tasks/reference/publish-test-results-v2
    title: Azure Pipelines task reference, PublishTestResults@2
    locator: Syntax
    accessed: 2026-11-16
  - url: https://maven.apache.org/surefire/maven-surefire-plugin/xsd/surefire-test-report.xsd
    title: Apache Maven Surefire, surefire-test-report.xsd
    locator: the whole schema
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/xml.html
    title: The Python Standard Library, XML Processing Modules
    locator: XML security
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/reference/reference.html
    title: pytest documentation, API Reference
    locator: Configuration Options, junit_family
    accessed: 2026-11-16
  - url: https://github.com/ota4j-team/open-test-reporting
    title: ota4j-team/open-test-reporting
    locator: the repository description
    accessed: 2026-11-16
  - url: https://docs.pytest.org/en/stable/how-to/output.html
    title: pytest documentation, Managing pytest's output
    locator: Creating JUnitXML format files; record_property
    accessed: 2026-11-16
  - url: https://nodejs.org/api/test.html
    title: Node.js v26.13.1 documentation, Test runner
    locator: Test reporters
    accessed: 2026-11-16
  - url: https://bats-core.readthedocs.io/en/stable/usage.html
    title: bats-core documentation, Usage
    locator: the options --formatter and --report-formatter
    accessed: 2026-11-16
---

## What it is

JUnit XML is an XML format for test results. A test runner writes one XML file, or one file for each test class, and a CI service reads the files and shows the passed, failed and skipped tests. No standards body owns the format. The JUnit User Guide calls it "the de facto standard for JUnit 4 based test reports that was made popular by the Ant build system".[^1] The XML formatter of the Ant JUnit task wrote these reports.[^2] Now test runners for other languages, such as pytest, the Node.js test runner and Bats, write reports in the same format.[^10][^11][^12]

The format is common because the CI services read it. Jenkins "understands the JUnit test report XML format (which is also used by TestNG)".[^3] GitLab shows test results in merge requests, and its "Unit test reports require the JUnit XML format".[^4] The task PublishTestResults@2 of Azure Pipelines reads JUnit, NUnit, VSTest, XUnit and CTest results, and JUnit is the default.[^5]

## Syntax or structure

The format has no single specification. Maven Surefire publishes an XML Schema for its reports, and GitLab documents the elements that it reads. This is the shape of a report, with the common attributes:[^6][^4]

```text
testsuites                 optional root: name, tests, failures, errors, skipped, time
  testsuite                name, tests, failures, errors, skipped, time, timestamp
    properties
      property             name, value
    testcase               name, classname, time
      failure              message, type; the text has the details
      error                message, type; the text has the details
      skipped              message
      system-out
      system-err
    system-out
    system-err
```

These are the rules that the Surefire schema gives:[^6]

- The root element is `testsuite`. Its attributes `name`, `tests`, `errors`, `skipped` and `failures` are required, and `time`, `timestamp`, `version`, `flakes` and `group` are optional.
- A `testsuite` has an optional `properties` element with `property` elements, each with a `name` and a `value`. Then come the `testcase` elements.
- A `testcase` must have a `name` and a `time`. Its `classname` is optional.
- A `testcase` can have many `failure` elements, but at most one `skipped` and at most one `error`. A `failure` and an `error` have the attributes `message` and `type`.
- Surefire adds the elements `rerunFailure`, `flakyFailure`, `rerunError` and `flakyError` for tests that it ran again.

Other producers put a `testsuites` element around the `testsuite` elements, so a consumer must accept both roots.[^4] The format does not define the difference between a `failure` and an `error` in one place. GitLab reads the content of both as the message and the stack trace.[^4]

A minimal report for GitLab is this. The attribute `file` is not in the Surefire schema, but GitLab reads it:[^4][^6]

```xml
<testsuites>
  <testsuite name="Authentication Tests" tests="1" failures="1">
    <testcase classname="LoginTest" name="test_invalid_password" file="spec/auth_spec.rb" time="0.23">
      <failure>Expected authentication to fail</failure>
      <system-out>[[ATTACHMENT|screenshots/failure.png]]</system-out>
    </testcase>
  </testsuite>
</testsuites>
```

## Examples

### Write a report in Python

This program converts a list of test results to a report with the standard library. It writes a `testsuites` root, a `testsuite` for each class, and a `failure`, `error` or `skipped` element for each test that did not pass. The counts on each element come from the results, so they agree with the `testcase` elements. ElementTree escapes `<` and `>` in the message.

```python run id=python-emit
import xml.etree.ElementTree as ET

# (class name, test name, seconds, outcome, message)
results = [
    ("test_cart", "test_empty_total", 0.002, "passed", ""),
    ("test_cart", "test_total", 0.003, "failed", "expected: <850> but was: <350>"),
    ("test_cart", "test_discount", 0.0, "skipped", "discounts are not ready"),
    ("test_checkout", "test_pay", 0.011, "error", "ConnectionError: no route to host"),
]


def counts(rows):
    return {
        "tests": str(len(rows)),
        "failures": str(sum(r[3] == "failed" for r in rows)),
        "errors": str(sum(r[3] == "error" for r in rows)),
        "skipped": str(sum(r[3] == "skipped" for r in rows)),
        "time": f"{sum(r[2] for r in rows):.3f}",
    }


root = ET.Element("testsuites", {"name": "shop", **counts(results)})
for suite_name in dict.fromkeys(r[0] for r in results):
    rows = [r for r in results if r[0] == suite_name]
    suite = ET.SubElement(root, "testsuite", {"name": suite_name, **counts(rows)})
    for classname, name, seconds, outcome, message in rows:
        case = ET.SubElement(
            suite,
            "testcase",
            {"classname": classname, "name": name, "time": f"{seconds:.3f}"},
        )
        if outcome == "failed":
            ET.SubElement(case, "failure", {"message": message}).text = message
        elif outcome == "error":
            ET.SubElement(case, "error", {"message": message, "type": "ConnectionError"})
        elif outcome == "skipped":
            ET.SubElement(case, "skipped", {"message": message})

ET.indent(root)
print(ET.tostring(root, encoding="unicode"))
```

```output for=python-emit
<testsuites name="shop" tests="4" failures="1" errors="1" skipped="1" time="0.016">
  <testsuite name="test_cart" tests="3" failures="1" errors="0" skipped="1" time="0.005">
    <testcase classname="test_cart" name="test_empty_total" time="0.002" />
    <testcase classname="test_cart" name="test_total" time="0.003">
      <failure message="expected: &lt;850&gt; but was: &lt;350&gt;">expected: &lt;850&gt; but was: &lt;350&gt;</failure>
    </testcase>
    <testcase classname="test_cart" name="test_discount" time="0.000">
      <skipped message="discounts are not ready" />
    </testcase>
  </testsuite>
  <testsuite name="test_checkout" tests="1" failures="0" errors="1" skipped="0" time="0.011">
    <testcase classname="test_checkout" name="test_pay" time="0.011">
      <error message="ConnectionError: no route to host" type="ConnectionError" />
    </testcase>
  </testsuite>
</testsuites>
```

To write a file, use `ET.ElementTree(root).write(path, encoding="utf-8", xml_declaration=True)`.

### Read both variants in Go

The package `encoding/xml` reads a report into structs. The function `read` accepts a report with a `testsuites` root, as in the GitLab example, and a report with a single `testsuite` root, as in the Surefire schema. The second report says `tests="3"` but has only 2 `testcase` elements, so the program counts the elements and does not trust the attribute.

```go run id=go-read
package main

import (
	"encoding/xml"
	"fmt"
)

type problem struct {
	Message string `xml:"message,attr"`
}

type testcase struct {
	Class   string   `xml:"classname,attr"`
	Name    string   `xml:"name,attr"`
	Failure *problem `xml:"failure"`
	Error   *problem `xml:"error"`
	Skipped *problem `xml:"skipped"`
}

type testsuite struct {
	Name  string     `xml:"name,attr"`
	Tests int        `xml:"tests,attr"`
	Cases []testcase `xml:"testcase"`
}

// read accepts both variants: a <testsuites> root and a single <testsuite> root.
func read(data string) ([]testsuite, error) {
	var root struct {
		XMLName xml.Name
		testsuite
		Suites []testsuite `xml:"testsuite"`
	}
	if err := xml.Unmarshal([]byte(data), &root); err != nil {
		return nil, err
	}
	if root.XMLName.Local == "testsuite" {
		return []testsuite{root.testsuite}, nil
	}
	return root.Suites, nil
}

func status(c testcase) string {
	switch {
	case c.Failure != nil:
		return "failed: " + c.Failure.Message
	case c.Error != nil:
		return "error: " + c.Error.Message
	case c.Skipped != nil:
		return "skipped: " + c.Skipped.Message
	}
	return "passed"
}

func main() {
	reports := []string{
		`<testsuites>
  <testsuite name="pytest" tests="2">
    <testcase classname="test_cart" name="test_total" time="0.003">
      <failure message="assert 350 == 850">test_cart.py:12: AssertionError</failure>
    </testcase>
    <testcase classname="test_cart" name="test_empty" time="0.001"/>
  </testsuite>
</testsuites>`,
		`<?xml version="1.0" encoding="UTF-8"?>
<testsuite name="com.example.CartTest" tests="3" failures="0" errors="0" skipped="1">
  <testcase name="emptyCart" classname="com.example.CartTest" time="0.004"/>
  <testcase name="discount" classname="com.example.CartTest" time="0">
    <skipped message="not ready"/>
  </testcase>
</testsuite>`,
	}
	for _, r := range reports {
		suites, err := read(r)
		if err != nil {
			fmt.Println("bad report:", err)
			continue
		}
		for _, s := range suites {
			fmt.Printf("suite %s: tests=%d, found %d\n", s.Name, s.Tests, len(s.Cases))
			for _, c := range s.Cases {
				fmt.Printf("  %s.%s %s\n", c.Class, c.Name, status(c))
			}
		}
	}
}
```

```output for=go-read
suite pytest: tests=2, found 2
  test_cart.test_total failed: assert 350 == 850
  test_cart.test_empty passed
suite com.example.CartTest: tests=3, found 2
  com.example.CartTest.emptyCart passed
  com.example.CartTest.discount skipped: not ready
```

### Read a Surefire report in Java

This program reads a report with the DOM parser of the JDK. It finds each `testcase`, reads its status from the child elements, and compares the counts on the `testsuite` with the elements. A report is input from outside the program, so the parser rejects a document type declaration.[^7]

```java run id=java-read
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

public class Main {
    static final String REPORT = """
        <?xml version="1.0" encoding="UTF-8"?>
        <testsuite name="com.example.CartTest" tests="3" failures="1" errors="0" skipped="1" time="0.021">
          <testcase name="emptyCartCostsNothing" classname="com.example.CartTest" time="0.004"/>
          <testcase name="totalAddsTheLines" classname="com.example.CartTest" time="0.012">
            <failure message="expected: &lt;850&gt; but was: &lt;350&gt;" type="org.opentest4j.AssertionFailedError">org.opentest4j.AssertionFailedError: expected: &lt;850&gt; but was: &lt;350&gt;
            at com.example.CartTest.totalAddsTheLines(CartTest.java:21)</failure>
          </testcase>
          <testcase name="discountIsApplied" classname="com.example.CartTest" time="0.0">
            <skipped message="discounts are not ready"/>
          </testcase>
        </testsuite>
        """;

    public static void main(String[] args) throws Exception {
        var factory = DocumentBuilderFactory.newInstance();
        // A report is input from outside the program, so reject DTDs.
        factory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        var doc = factory.newDocumentBuilder()
                .parse(new ByteArrayInputStream(REPORT.getBytes(StandardCharsets.UTF_8)));

        NodeList cases = doc.getElementsByTagName("testcase");
        int failures = 0;
        for (int i = 0; i < cases.getLength(); i++) {
            var testcase = (Element) cases.item(i);
            String status = "passed";
            if (child(testcase, "failure") instanceof Element f) {
                status = "failed: " + f.getAttribute("message");
                failures++;
            } else if (child(testcase, "error") instanceof Element e) {
                status = "error: " + e.getAttribute("message");
            } else if (child(testcase, "skipped") != null) {
                status = "skipped";
            }
            System.out.println(testcase.getAttribute("name") + " " + status);
        }

        var suite = doc.getDocumentElement();
        System.out.println("tests=" + suite.getAttribute("tests") + ", counted " + cases.getLength());
        System.out.println("failures=" + suite.getAttribute("failures") + ", counted " + failures);
    }

    static Element child(Element parent, String tag) {
        NodeList list = parent.getElementsByTagName(tag);
        return list.getLength() == 0 ? null : (Element) list.item(0);
    }
}
```

```output for=java-read
emptyCartCostsNothing passed
totalAddsTheLines failed: expected: <850> but was: <350>
discountIsApplied skipped
tests=3, counted 3
failures=1, counted 1
```

## Versions

The format has no version number. These are the main variants:

| Variant | Notes |
|---|---|
| Ant JUnit task | the XML formatter is one of the 4 predefined formatters of the task[^2] |
| Maven Surefire | the schema `surefire-test-report.xsd` with the version 3.0.2; one `testsuite` root, and elements for tests that ran again[^6] |
| JUnit Platform, legacy | `LegacyXmlReportGeneratingListener` writes "a separate XML report for each root in the TestPlan"; the Console Launcher uses it[^1] |
| pytest | `junit_family` is `legacy`, `xunit1` or `xunit2`; the default is `xunit2` since pytest 6.1[^8] |
| GitLab | reads a subset: the counts on `testsuite` and the `properties` elements are not parsed[^4] |

pytest says that `xunit1` is "compatible with the xunit 1.0 format", and that `xunit2` "should be more compatible with latest Jenkins versions".[^8] The JUnit publisher was part of Jenkins core until version 1.577, when it moved to a plugin.[^3]

The JUnit team also maintains Open Test Reporting, which has "Language-agnostic XML and HTML test reporting formats and tooling".[^9] The JUnit Platform can write it with `OpenTestReportGeneratingListener` in an event-based format, but the parameter `junit.platform.reporting.open.xml.enabled` is false by default.[^1]

## Pitfalls

- **The counts can be wrong.** The `tests`, `failures`, `errors` and `skipped` attributes are copies, and nothing makes them agree with the `testcase` elements. GitLab does not parse the counts on `testsuite`.[^4] Count the elements when you read a report.
- **Two roots.** Some producers write a `testsuites` root and some write a single `testsuite` root. A consumer that accepts one root only loses reports.[^6][^4]
- **Characters are lost.** XML cannot hold some characters. The Ant manual says that the XML formatter "may not include the same output that your tests have written as some characters are illegal in XML documents and will be dropped".[^2]
- **Extra elements break schemas.** pytest can write a `properties` element in a `testcase`, and it warns that this will "break schema verifications for the latest JUnitXML schema".[^10]
- **The report does not fail the job.** In GitLab, the reports "do not affect job status". The script of the job must exit with a nonzero status when tests fail.[^4]
- **XML attacks.** A report can come from an untrusted pull request. "An attacker can abuse XML features to carry out denial of service attacks, access local files, generate network connections to other machines, or circumvent firewalls when attacker-controlled XML is being parsed, in Python or elsewhere."[^7] Turn off document type declarations and external entities in the parser.
- **Wrong paths.** GitLab reads files and glob patterns in `artifacts:reports:junit`, but not directories.[^4]

## Tools and libraries

- **Java.** [[junit-5]] writes the legacy XML format with the Console Launcher.[^1] Maven Surefire publishes the schema of its reports.[^6]
- **Python.** [[pytest]] writes a report with `pytest --junit-xml=path`. The option `junit_suite_name` sets the name of the root test suite.[^10]
- **Node.js.** The [[node-test]] runner has a reporter `junit` that "outputs test results in a jUnit XML format". The exact output of the reporters can change.[^11]
- **Shell.** [[bats]] writes a report with `--formatter junit`, and `--report-formatter junit` writes a report file next to the normal output.[^12]
- **CI services.** The Jenkins JUnit plugin, GitLab unit test reports, and PublishTestResults@2 in Azure Pipelines read the format. The default file pattern of the Azure task is `**/TEST-*.xml`.[^3][^4][^5]

For a line-based format that a test program writes to standard output, see [[tap-format]].

## References
