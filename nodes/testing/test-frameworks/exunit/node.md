---
tag: PB5817
kind: tool
slug: exunit
name: ExUnit
aliases: [ex_unit, mix test]
area: testing/test-frameworks
level: advanced
summary: >-
  ExUnit is the test framework that ships with Elixir. Test modules can run concurrently, setup callbacks fill a context map, and doctests run the iex examples in the documentation.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/elixir-lang/elixir
homepage: https://hexdocs.pm/ex_unit/ExUnit.html
see_also: [test-framework, unit-testing, xunit, doctest-python, rspec-and-minitest, test-driven-development, property-based-tests]
sources:
  - url: https://hexdocs.pm/ex_unit/ExUnit.html
    title: ExUnit v1.20.5, ExUnit
    locator: Example; Integration with Mix; configure/1
    accessed: 2026-11-15
  - url: https://hexdocs.pm/elixir/introduction-to-mix.html
    title: Elixir v1.20.5, Introduction to Mix
    locator: Our first project; Running tests
    accessed: 2026-11-15
  - url: https://github.com/elixir-lang/elixir/blob/main/LICENSE
    title: elixir, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://hexdocs.pm/ex_unit/ExUnit.Case.html
    title: ExUnit v1.20.5, ExUnit.Case
    locator: the options of use ExUnit.Case; Context; Tags; Filters; Parameterized tests; describe/2
    accessed: 2026-11-15
  - url: https://hexdocs.pm/ex_unit/ExUnit.Assertions.html
    title: ExUnit v1.20.5, ExUnit.Assertions
    locator: the introduction; assert_raise/3; assert_receive/3
    accessed: 2026-11-15
  - url: https://hexdocs.pm/ex_unit/ExUnit.DocTest.html
    title: ExUnit v1.20.5, ExUnit.DocTest
    locator: Syntax; Examples
    accessed: 2026-11-15
  - url: https://hexdocs.pm/mix/Mix.Tasks.Test.html
    title: Mix v1.20.5, mix test
    locator: Command line options; Configuration; Coverage
    accessed: 2026-11-15
  - url: https://hexdocs.pm/ex_unit/ExUnit.Callbacks.html
    title: ExUnit v1.20.5, ExUnit.Callbacks
    locator: the introduction; Examples
    accessed: 2026-11-15
  - url: https://github.com/elixir-lang/elixir/releases
    title: elixir, Releases
    locator: v1.3.0; v1.11.0; v1.12.0; v1.17.0; v1.18.0; v1.19.0; v1.20.0; v1.20.5
    accessed: 2026-11-15
---

## What it is

ExUnit is the "Unit testing framework for Elixir."[^1] It is "a unit-test based framework that ships with Elixir", together with the build tool Mix.[^2] Its licence is the Apache License 2.0, the licence of Elixir.[^3]

A test module calls `use ExUnit.Case`, and the macro `test` defines each test. The macro `assert` checks a value and, when it fails, shows the code and the values of the two sides.[^4][^5] With the option `async: true`, the tests of a module run concurrently with the tests of other modules.[^4] The macro `doctest` makes tests from the `iex>` examples in the documentation of a module.[^6]

## Install and first use

ExUnit has no separate install, because it is part of Elixir.[^2] The command `mix new` makes a project that is ready for tests. The guide uses this command:[^2]

```text
$ mix new kv --module KV
* creating README.md
* creating .formatter.exs
* creating .gitignore
* creating mix.exs
* creating lib
* creating lib/kv.ex
* creating test
* creating test/test_helper.exs
* creating test/kv_test.exs
```

The file `test/test_helper.exs` has one line, `ExUnit.start()`. Mix loads it before it runs the tests.[^2][^1] The file `lib/kv.ex` has a function with an example in its documentation:[^2]

```text
defmodule KV do
  @moduledoc """
  Documentation for KV.
  """

  @doc """
  Hello world.

  ## Examples

      iex> KV.hello()
      :world

  """
  def hello do
    :world
  end
end
```

The file `test/kv_test.exs` has one test, and the line `doctest KV` adds the example from the documentation as a second test:[^2]

```text
defmodule KVTest do
  use ExUnit.Case
  doctest KV

  test "greets the world" do
    assert KV.hello() == :world
  end
end
```

The guide shows this output of `mix test`:[^2]

```text
$ mix test
Compiled lib/kv.ex
Generated kv app
Running ExUnit with seed: 540224, max_cases: 16
..

Finished in 0.04 seconds
1 doctest, 1 test, 0 failures
```

ExUnit prints a dot for each test that passes, and it puts the tests in a random order. The seed in the first line makes the order.[^2][^7] When the assertion is changed to `assert KV.hello() == :oops`, the test fails, and the guide shows this output:[^2]

```text
  1) test greets the world (KVTest)
     test/kv_test.exs:5
     Assertion with == failed
     code:  assert KV.hello() == :oops
     left:  :world
     right: :oops
     stacktrace:
       test/kv_test.exs:6: (test)

.

Finished in 0.05 seconds
1 doctest, 1 test, 1 failure
```

The report gives the test name, the file and line of the test, the code of the assertion, and the values of the left and right sides.[^2] Without Mix, a script that calls `ExUnit.start()` and defines a test module runs with `elixir assertion_test.exs`.[^1]

## Concepts of the tool

### Concurrency

The option `:async` "configures tests in this module to run concurrently with tests in other modules. Tests in the same module never run concurrently". The default is `false`, and the option is safe only when the tests do not change global state.[^4] The option `:max_cases` sets the maximum number of tests that run in parallel. Its default is `System.schedulers_online * 2`.[^1] The option `:group` puts modules in a group, and the tests of one group never run concurrently.[^4]

### describe and tags

The macro `describe` puts tests in a block. The name of the block is a prefix of the test names, and a `setup` in the block runs only for the tests of that block.[^4]

```text
defmodule StringTest do
  use ExUnit.Case, async: true

  describe "String.downcase/1" do
    test "with ascii characters" do
      assert String.downcase("HELLO") == "hello"
    end

    test "with Unicode" do
      assert String.downcase("HÉLLÒ") == "héllò"
    end
  end
end
```

A `describe` block cannot be in another `describe` block. The documentation recommends named setups instead.[^4] A tag is metadata on a test. `@tag` sets it for the next test, `@describetag` for the tests of a block, and `@moduletag` for all the tests of a module. `@tag :key` is the same as `@tag key: true`.[^4] Filters select tests by their tags. For example, `ExUnit.configure(exclude: [external: true])` in the test helper skips the tests with the tag `:external`, and `mix test --include external:true` runs them again.[^4]

### Setup and the context

Each test gets a context, a map that the setup callbacks can add to.[^4] `setup_all` runs one time for each module, before the tests of the module. `setup` runs before each test, in the process of the test. `on_exit` registers a function that runs after the test, in a new process.[^8][^4]

```text
defmodule KVTest do
  use ExUnit.Case

  setup do
    {:ok, pid} = KV.start_link()
    {:ok, pid: pid}
  end

  test "stores key-value pairs", context do
    assert KV.put(context[:pid], :hello, :world) == :ok
    assert KV.get(context[:pid], :hello) == :world
  end
end
```

In this example from the documentation, `setup` starts a process and adds its pid to the context. The test reads the pid from the context.[^4] The tag `:tmp_dir` gives a test a new temporary directory in the context.[^4]

### Assertions

The macro `assert` "introspects your code and provides good reporting whenever there is a failure".[^5] There are also special assertions. `assert_raise` checks that a function raises an exception, and it can also check the message. `assert_in_delta` checks that two numbers differ by no more than a delta. `assert_receive` checks that the process gets a message that matches a pattern, and it waits up to 100 milliseconds by default.[^5]

```text
assert_raise ArithmeticError, "bad argument in arithmetic expression", fn ->
  1 / 0
end
```

### Doctests

A doctest is an example in a `@moduledoc` or `@doc` attribute. Each example starts on a new line with the prefix `iex>`, and the lines that continue an expression start with `...>`. The expected result is on the next line.[^6]

```text
iex> Enum.map([1, 2, 3], fn x ->
...>   x * 2
...> end)
[2, 4, 6]
```

`doctest MyModule` in a test module finds the examples in the documentation of all the functions and macros of `MyModule`, and it runs each one as a test.[^6] For the same idea in Python, see [[doctest-python]].

### Parameterized tests

The option `:parameterize` is a list of maps. ExUnit runs all the tests of the module one time for each map, and it merges the map into the context. With `async: true`, the different parameters run concurrently.[^4] This example from the documentation runs the tests for 4 kinds of `Registry`:[^4]

```text
use ExUnit.Case,
  async: true,
  parameterize:
    for(kind <- [:unique, :duplicate],
        partitions <- [1, 8],
        do: %{kind: kind, partitions: partitions})
```

A test then gets the values with a pattern, for example `test "starts a registry", %{kind: kind, partitions: partitions} do`.[^4]

## Common tasks

```text
mix test                                      run all the tests
mix test test/kv_test.exs                     run the tests in one file
mix test test/kv_test.exs:5                   run the test at line 5
mix test --failed                             run the tests that failed the last time
mix test --stale                              run the tests for the modules that changed
mix test --only describe:"String.downcase/1"   run the tests in one describe block
mix test --exclude slow                       skip the tests with the tag slow
mix test --seed 0                             run the tests in the order they are defined
mix test --trace                              run one test at a time, with detailed output
mix test --slowest 10                         show the 10 slowest tests
mix test --repeat-until-failure 1000          run the suite again until a test fails
mix test --cover                              measure the line coverage
mix test --dry-run                            show the tests that would run
```

The options come from the documentation of `mix test` and of `ExUnit.Case`.[^7][^4] `--trace` sets `--max-cases` to 1.[^7] With `--partitions 4`, Mix splits the test files into 4 groups, and the environment variable `MIX_TEST_PARTITION` selects the group to run. The documentation says that this can also distribute the tests across many machines.[^7]

## Configuration

The test helper `test/test_helper.exs` starts ExUnit, and it can configure it. For example, `ExUnit.configure(exclude: [external: true])` excludes a tag by default.[^1][^4] Other options of `ExUnit.configure/1` include `:max_cases`, `:seed`, `:timeout` and `:assert_receive_timeout`.[^1]

The project function in `mix.exs` configures `mix test`. `:test_paths` is the list of test directories. Its default is `["test"]`, and each test directory must have a `test_helper.exs`.[^7] Mix loads the files that end in `_test.exs`. Before Elixir 1.19.0, the option `:test_pattern` had the default `*_test.exs`.[^7]

`mix test --cover` uses the line-based coverage of the Erlang module `:cover`. The key `:test_coverage` in `mix.exs` configures it. The report goes to the directory `cover`, and the summary has a threshold of 90 percent by default. When the total coverage is below the threshold, the task exits with status 1.[^7]

## Alternatives

- [[rspec-and-minitest]]: Ruby test frameworks. ExUnit also has `describe` and `test` blocks.
- [[doctest-python]]: tests from examples in the documentation, for Python.
- [[xunit]]: the family of test frameworks with assertions and setup methods.
- [[property-based-tests]]: tests that check a property for many generated inputs.

## History and versions

ExUnit is part of Elixir, so it has the same versions as Elixir.[^1][^9]

| Version | Release | ExUnit change |
|---|---|---|
| Elixir 1.3.0 | 21 June 2016 | `describe/2`; named setups[^9][^4] |
| Elixir 1.11.0 | 6 October 2020 | the tag `:tmp_dir`[^9][^4] |
| Elixir 1.12.0 | 19 May 2021 | `mix test --warnings-as-errors`[^9][^7] |
| Elixir 1.17.0 | 12 June 2024 | `mix test --repeat-until-failure`[^9][^7] |
| Elixir 1.18.0 | 19 December 2024 | parameterized tests; test groups[^9][^4] |
| Elixir 1.19.0 | 16 October 2025 | a new default for `:test_pattern`; `:test_group` in the context[^9][^7] |
| Elixir 1.20.0 | 3 June 2026 | `mix test --dry-run`; the option `:formatter` of `ExUnit.CaptureLog`[^9] |

On 12 November 2026, the latest release was Elixir 1.20.5, from 29 October 2026.[^9]

## References
