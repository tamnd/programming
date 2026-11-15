---
tag: 4EQ4AJ
kind: tool
slug: rspec-and-minitest
name: RSpec and Minitest
aliases: [RSpec, Minitest, rspec-core, rspec-mocks, minitest/spec]
area: testing/test-frameworks
level: advanced
summary: >-
  RSpec and Minitest are the two main test frameworks of Ruby. RSpec is a DSL of describe and it blocks with matchers and mocks. Minitest is small, and its tests are plain Ruby classes and methods.
role: [test-framework]
licence: mit
repo: https://github.com/rspec/rspec
homepage: https://rspec.info
see_also: [test-framework, unit-testing, test-double, test-driven-development, xunit, jest, kotest, pytest]
sources:
  - url: https://rspec.info/
    title: RSpec, home page
    locator: the whole page
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec
    title: rspec, README of the mono repo
    locator: the list of gems; Install; Getting started
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec/blob/main/rspec-core/LICENSE.md
    title: rspec-core, LICENSE.md
    locator: the whole file
    accessed: 2026-11-15
  - url: https://github.com/minitest/minitest
    title: minitest, README
    locator: DESCRIPTION; FEATURES/PROBLEMS; SYNOPSIS; Running Your Tests; Rake Tasks; FAQ; LICENSE
    accessed: 2026-11-15
  - url: https://github.com/ruby/ruby/blob/master/gems/bundled_gems
    title: ruby, gems/bundled_gems
    locator: the line for minitest
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec/blob/main/rspec-core/README.md
    title: rspec-core, README
    locator: Basic Structure; Nested Groups; Aliases; Shared Examples and Contexts; Metadata; A Word on Scope; Store Command Line Options
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec/blob/main/rspec-expectations/README.md
    title: rspec-expectations, README
    locator: Basic usage; Built-in matchers; Compound Matcher Expressions
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-core/command-line/
    title: RSpec Core 3.13 features, Command line options
    locator: Run with ruby; Topics
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-core/command-line/order/
    title: RSpec Core 3.13 features, Using the --order option
    locator: the introduction
    accessed: 2026-11-15
  - url: https://github.com/minitest/minitest/blob/master/History.rdoc
    title: minitest, History.rdoc
    locator: 6.0.0; 5.0.0; 1.0.0
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-core/helper-methods/let/
    title: RSpec Core 3.13 features, let and let!
    locator: the introduction
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-core/hooks/before-and-after-hooks/
    title: RSpec Core 3.13 features, before and after hooks
    locator: the introduction
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec/blob/main/rspec-mocks/README.md
    title: rspec-mocks, README
    locator: Test Doubles; Method Stubs; Message Expectations; Test Spies
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-mocks/basics/test-doubles/
    title: RSpec Mocks 3.13 features, Test Doubles
    locator: the introduction; Doubles are strict by default
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-core/command-line/only-failures/
    title: RSpec Core 3.13 features, Using the --only-failures option
    locator: the introduction
    accessed: 2026-11-15
  - url: https://rspec.info/features/3-13/rspec-core/command-line/bisect/
    title: RSpec Core 3.13 features, Bisect
    locator: the introduction
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec/blob/main/rspec-core/Changelog.md
    title: rspec-core, Changelog
    locator: 4.0.0.beta1; 3.13.7
    accessed: 2026-11-15
  - url: https://rubygems.org/api/v1/versions/rspec-core.json
    title: RubyGems.org, the versions of rspec-core
    locator: 4.0.0.beta1; 3.13.7; 3.0.0; 2.0.0
    accessed: 2026-11-15
  - url: https://github.com/rspec/rspec/blob/main/rspec-expectations/Changelog.md
    title: rspec-expectations, Changelog
    locator: 4.0.0.beta1; 2.11.0
    accessed: 2026-11-15
  - url: https://rubygems.org/api/v1/versions/minitest.json
    title: RubyGems.org, the versions of minitest
    locator: 6.1.0; 6.0.0; 5.0.0
    accessed: 2026-11-15
---

## What it is

RSpec and Minitest are the two test frameworks that most Ruby projects use. This page covers both, because a Ruby programmer usually meets both, and the choice between them is mostly a choice of style.

RSpec calls itself "Behaviour Driven Development for Ruby".[^1] It is a set of gems. The gem `rspec-core` gives the structure of the examples and the command `rspec`, `rspec-expectations` gives the matchers, `rspec-mocks` gives the test doubles, and `rspec-support` has the shared helper code.[^2] RSpec uses the words `describe` and `it`, so a spec reads "like a conversation".[^2] The licence is the MIT License, and the oldest copyright notice in the licence file is "Copyright © 2005 Steven Baker".[^3]

Minitest "provides a complete suite of testing facilities supporting TDD, BDD, and benchmarking".[^4] It has a small unit test library (`minitest/test`), a spec engine on top of it (`minitest/spec`), benchmarks (`minitest/benchmark`) and coloured output (`minitest/pride`).[^4] Its README makes the difference short: "rspec is a testing DSL. minitest is ruby."[^4] A Minitest test is a class with methods, and its licence is the MIT License.[^4] Minitest is a dependency of Rails, and the Ruby source tree lists it as a bundled gem.[^4][^5]

## Install and first use

Install the gems with `gem install`, or add them to the `Gemfile` of the project. The gem `rspec` installs `rspec-core`, `rspec-expectations` and `rspec-mocks`.[^2]

```text
gem install minitest
gem install rspec
```

The examples on this page test this class. It has a bug: `total_cents` forgets the quantity.

```text
# lib/cart.rb
# A Cart holds lines of (price in cents, quantity).
class Cart
  def initialize
    @lines = []
  end

  def add(price_cents, quantity = 1)
    raise ArgumentError, "quantity must be positive" unless quantity.positive?

    @lines << [price_cents, quantity]
    self
  end

  def total_cents
    @lines.sum { |price, _quantity| price }
  end
end
```

### A first Minitest test

A Minitest test class is a subclass of `Minitest::Test`, and each method whose name starts with `test_` is a test.[^4] The method `setup` runs before each test. `require "minitest/autorun"` makes the file run its tests when Ruby runs the file.[^4]

```text
# test/cart_test.rb
require "minitest/autorun"
require_relative "../lib/cart"

class CartTest < Minitest::Test
  def setup
    @cart = Cart.new
  end

  def test_empty_cart_costs_nothing
    assert_equal 0, @cart.total_cents
  end

  def test_total_multiplies_price_by_quantity
    @cart.add(250, 3).add(100)
    assert_equal 850, @cart.total_cents
  end

  def test_rejects_zero_quantity
    error = assert_raises(ArgumentError) { @cart.add(250, 0) }
    assert_equal "quantity must be positive", error.message
  end
end
```

Run the file with Ruby. The option `--seed` sets the random seed of the order of the tests, so the run can be repeated.[^4] This output is from Minitest 6.1.0 on Ruby 3.2.3:

```text
$ ruby -Ilib -Itest test/cart_test.rb --seed 42
Run options: --seed 42

# Running:

..F

Finished in 0.091798s, 32.6806 runs/s, 43.5741 assertions/s.

  1) Failure:
CartTest#test_total_multiplies_price_by_quantity [test/cart_test.rb:15]:
Expected: 850
  Actual: 350

3 runs, 4 assertions, 1 failures, 0 errors, 0 skips
```

The exit status is 1. Change the block in `total_cents` to `{ |price, quantity| price * quantity }`, and the run passes:

```text
$ ruby -Ilib -Itest test/cart_test.rb --seed 42 --verbose
Run options: --seed 42 --verbose

# Running:

CartTest#test_empty_cart_costs_nothing = 0.00 s = .
CartTest#test_rejects_zero_quantity = 0.00 s = .
CartTest#test_total_multiplies_price_by_quantity = 0.00 s = .

Finished in 0.071153s, 42.1626 runs/s, 56.2168 assertions/s.

3 runs, 4 assertions, 0 failures, 0 errors, 0 skips
```

### A first RSpec spec

`RSpec.describe` makes an example group, and `it` declares an example in the group.[^6] `context` is another name for `describe`, for nested groups.[^6] `expect(actual).to eq(expected)` passes if `actual == expected`.[^7] RSpec adds `lib` and `spec` to the load path, so `require "cart"` finds `lib/cart.rb`.[^8]

```text
# spec/cart_spec.rb
require "cart"

RSpec.describe Cart do
  subject(:cart) { described_class.new }

  it "costs nothing when empty" do
    expect(cart.total_cents).to eq(0)
  end

  context "with two lines" do
    before { cart.add(250, 3).add(100) }

    it "multiplies each price by its quantity" do
      expect(cart.total_cents).to eq(850)
    end

    it "has a total above 5 euros" do
      expect(cart.total_cents).to be > 500
    end
  end

  it "rejects a zero quantity" do
    expect { cart.add(250, 0) }.to raise_error(ArgumentError, "quantity must be positive")
  end
end
```

With the bug still in `total_cents`, the run fails. This output is from rspec-core 3.13.7 on Ruby 3.2.3:

```text
$ rspec spec/cart_spec.rb --order defined
..FF

Failures:

  1) Cart with two lines multiplies each price by its quantity
     Failure/Error: expect(cart.total_cents).to eq(850)

       expected: 850
            got: 350

       (compared using ==)
     # ./spec/cart_spec.rb:14:in `block (3 levels) in <top (required)>'

  2) Cart with two lines has a total above 5 euros
     Failure/Error: expect(cart.total_cents).to be > 500

       expected: > 500
            got:   350
     # ./spec/cart_spec.rb:18:in `block (3 levels) in <top (required)>'

Finished in 0.0287 seconds (files took 0.22907 seconds to load)
4 examples, 2 failures

Failed examples:

rspec ./spec/cart_spec.rb:13 # Cart with two lines multiplies each price by its quantity
rspec ./spec/cart_spec.rb:17 # Cart with two lines has a total above 5 euros
```

Each line under `Failed examples` is a command that runs that example again. The exit status is 1. After the fix, the documentation formatter prints the groups and examples as a nested list (the spec for `Checkout` is in the section on test doubles below). In the defined order, each group runs its own examples before its nested groups, so `rejects a zero quantity` comes before `with two lines`.[^9]

```text
$ rspec --format documentation --order defined

Cart
  costs nothing when empty
  rejects a zero quantity
  with two lines
    multiplies each price by its quantity
    has a total above 5 euros

Checkout
  charges the cart total to the card
  can check the call after it happens

Finished in 0.02504 seconds (files took 0.21719 seconds to load)
6 examples, 0 failures
```

## Concepts of the tool

### Example groups are classes

An RSpec example group is a class. The block of `describe` is evaluated in the context of a subclass of `RSpec::Core::ExampleGroup`, and nested groups are subclasses of the outer group.[^6] Each example runs in a new instance of its group class.[^6] Examples do not run when RSpec loads the spec file. RSpec waits until all spec files are loaded, and then it can filter and randomize the examples.[^6]

So the RSpec DSL and Minitest are closer than they look. A Minitest test class is a class too, and its spec DSL, `describe` and `it`, makes classes and test methods. In the verbose output of the spec version of the cart tests, the example "costs nothing when empty" is the method `test_0001_costs nothing when empty` of the class `Cart`:

```text
# test/cart_spec_test.rb
require "minitest/autorun"
require_relative "../lib/cart"

describe Cart do
  before do
    @cart = Cart.new
  end

  it "costs nothing when empty" do
    _(@cart.total_cents).must_equal 0
  end

  describe "with two lines" do
    before do
      @cart.add(250, 3).add(100)
    end

    it "multiplies each price by its quantity" do
      _(@cart.total_cents).must_equal 850
    end
  end

  it "rejects a zero quantity" do
    _ { @cart.add(250, 0) }.must_raise ArgumentError
  end
end
```

```text
$ ruby -Ilib -Itest test/cart_spec_test.rb --seed 42 --verbose
Run options: --seed 42 --verbose

# Running:

Cart::with two lines#test_0001_multiplies each price by its quantity = 0.00 s = .
Cart#test_0002_rejects a zero quantity = 0.00 s = .
Cart#test_0001_costs nothing when empty = 0.00 s = .

Finished in 0.040447s, 74.1720 runs/s, 74.1720 assertions/s.

3 runs, 3 assertions, 0 failures, 0 errors, 0 skips
```

The spec expectations of Minitest 6 work on a value wrapped with `_`, `value` or `expect`, for example `_(x).must_equal 1`. Minitest 6.0.0 removed the old expectations that it had added to every `Object`.[^10]

### Assertions and matchers

Minitest has assertion methods, for example `assert_equal`, `assert_nil`, `assert_raises`, `assert_predicate` and `assert_operator`. The `refute_*` methods, for example `refute_match`, check the opposite.[^4][^10] Since 6.0.0, `assert_equal(nil, value)` is an error, and the test must use `assert_nil`.[^10]

RSpec has matchers. An expectation is `expect(actual).to matcher` or `expect(actual).not_to matcher`.[^7] Some built-in matchers are below.[^7]

```text
expect(actual).to eq(expected)         # actual == expected
expect(actual).to eql(expected)        # actual.eql?(expected)
expect(actual).to be > 500             # comparison
expect(actual).to match(/pattern/)     # regular expression
expect(list).to include(3)
expect { block }.to raise_error(ArgumentError)
expect(actual).to be_empty             # predicate matcher: actual.empty?
expect(actual).to start_with("a").and end_with("z")
```

Matchers combine with `and` and `or`, and many matchers take other matchers as arguments.[^7] A predicate matcher `be_xxx` passes if `actual.xxx?` is true, so `be_empty` calls `empty?`.[^7]

### let, subject and hooks

`let` defines a memoized helper method. Its value is cached across calls in the same example, but not across examples.[^11] `let` is lazy: the block runs the first time the example calls the method. `let!` runs it before each example.[^11]

`before` and `after` hooks run code around examples. `before(:example)` runs before each example, and `before(:context)` runs one time before all the examples in a group. A bare `before` uses the `:example` scope.[^12] The scopes `:each` and `:all` are other names for `:example` and `:context`.[^12] `around` hooks run before any `before` hooks and after any `after` hooks.[^12]

In Minitest, a test class has the method `setup`, and a spec has `before` blocks.[^4]

### Shared examples

RSpec can share a group of examples between groups. Declare it with `shared_examples`, and include it with `it_behaves_like` or `include_examples`.[^6] `it_behaves_like` wraps the examples in a new nested group, so a block can give its own `let` values. `include_examples` puts the examples directly in the current group.[^6] `shared_context` and `include_context` share hooks, `let` declarations and helper methods without examples.[^6]

```text
RSpec.shared_examples "a collection" do
  it "is empty when first created" do
    expect(described_class.new).to be_empty
  end
end

RSpec.describe Array do
  include_examples "a collection"
end

RSpec.describe Hash do
  include_examples "a collection"
end
```

`described_class` is the class given to `describe`.[^6] In Minitest, the same reuse is ordinary Ruby: put the tests in a module and include the module in each test class.

### Test doubles

rspec-mocks makes test doubles. `double("name")` makes a double, `allow(obj).to receive(:msg)` stubs a method, and `expect(obj).to receive(:msg)` is a message expectation, which fails the example if the message does not come.[^13] A double is strict by default: a message that is not allowed or expected is an error.[^14] `have_received` checks a call after it happens, which makes the double a spy.[^13]

This class charges a card through a payment gateway:

```text
# lib/checkout.rb
# Checkout asks a payment gateway to charge the total of a cart.
class Checkout
  def initialize(cart, gateway)
    @cart = cart
    @gateway = gateway
  end

  def pay(card)
    @gateway.charge(card, @cart.total_cents)
  end
end
```

The spec uses a double in place of the real gateway, in the two styles:

```text
# spec/checkout_spec.rb
require "cart"
require "checkout"

RSpec.describe Checkout do
  let(:cart) { Cart.new.add(250, 2) }
  let(:gateway) { double("gateway") }

  it "charges the cart total to the card" do
    expect(gateway).to receive(:charge).with("card-1", 500).and_return(:paid)

    expect(Checkout.new(cart, gateway).pay("card-1")).to eq(:paid)
  end

  it "can check the call after it happens" do
    allow(gateway).to receive(:charge).and_return(:paid)

    Checkout.new(cart, gateway).pay("card-2")

    expect(gateway).to have_received(:charge).with("card-2", 500).once
  end
end
```

A verifying double, for example `instance_double("Book", :pages => 250)`, also checks that the real class has the stubbed methods, with a valid number of arguments.[^13] The RSpec documentation recommends verifying doubles where possible.[^14]

Minitest 6.0.0 removed `minitest/mock.rb` from the gem. The mock object is now in the separate gem `minitest-mock`.[^10]

## Common tasks

```text
rspec --init                              make .rspec and spec/spec_helper.rb
rspec spec/cart_spec.rb:13                run the example at line 13
rspec --format documentation              print the nested list of examples
rspec --tag focus                         run only the examples with the tag
rspec --only-failures                     run the examples that failed last time
rspec --next-failure                      run only the next failing example, then stop
rspec --seed 1234 --bisect                find the smallest set of examples that reproduce a failure
ruby -Ilib -Itest test/cart_test.rb --seed 42    repeat a Minitest run in the same order
rake test                                 run the Minitest suite through the Rake task
MT_CPU=4 rake test                        set the number of threads for parallel Minitest tests
```

The RSpec options come from the pages on command line options.[^8][^15][^16] `--next-failure` is short for `--only-failures --fail-fast --order defined`.[^15] The Minitest commands come from the README.[^4] `rspec --init` writes `.rspec`, which has the line `--require spec_helper`, and `spec/spec_helper.rb`. Minitest 6.0.0 added its own command `minitest`, which can run a test at a line, and `minitest --bisect`.[^10]

## Configuration

RSpec reads command line options from a file `.rspec` in the project root, as if they were typed on the command line.[^6] Code configuration goes in an `RSpec.configure` block, which is conventionally in `spec/spec_helper.rb`.[^8]

```text
# .rspec
--require spec_helper
--order rand
```

```text
# spec/spec_helper.rb
RSpec.configure do |config|
  config.example_status_persistence_file_path = "spec/examples.txt"
end
```

In RSpec 3, the default order is `defined`. `--order rand` randomizes the order of the groups, and of the examples in each group, and it can take a seed.[^9] rspec-core 4.0.0.beta1 makes random order the default.[^17] `--only-failures` needs `config.example_status_persistence_file_path`, which is the file where RSpec stores the status of each example from the last run.[^15]

For Minitest, a Rakefile makes the Rake tasks with `Minitest::TestTask.create`, and the task reads environment variables like `SEED`, `MT_CPU`, `N` (a filter by name) and `X` (an exclusion).[^4] Minitest 6 loads plugins only when the test helper asks for them, for example with `Minitest.load_plugins`.[^10]

```text
# Rakefile
require "minitest/test_task"

Minitest::TestTask.create(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.warning = false
  t.test_globs = ["test/**/*_test.rb"]
end

task :default => :test
```

## Alternatives

- [[xunit]]: the family of test frameworks that `Minitest::Test` follows, with test classes, test methods and `setup`.
- [[jest]], [[kotest]] and [[pytest]]: frameworks of other languages, which use the same describe and it style or the same plain-function style.

## History and versions

| Version | Change |
|---|---|
| minitest 1.0.0, 30 October 2006 | the first release[^10] |
| rspec-core 2.0.0, 10 October 2010 | the first stable release of the gem `rspec-core`[^18] |
| rspec-expectations 2.11.0, 7 July 2012 | `expect` works on values as well as blocks; options to choose the `expect` syntax, the `should` syntax or both[^19] |
| minitest 5.0.0, 10 May 2013 | `MiniTest` renamed to `Minitest`; `Minitest::Runnable`[^10][^20] |
| rspec-core 3.0.0, 2 June 2014 | RSpec 3[^18] |
| minitest 6.0.0, 17 December 2025 | Ruby 3.2 or later; `MiniTest` compatibility namespace and `minitest/mock.rb` removed; plugins load only on request; `--include` replaces `--name`; the `minitest` command[^10] |
| rspec-core 4.0.0.beta1, 18 February 2026 | a pre-release; Ruby 3.0 or later; the `should` syntax removed; random order by default[^18][^17][^19] |

On 12 November 2026, the latest release of rspec-core was 3.13.7, from 6 November 2026, and the latest release of minitest was 6.1.0, from 6 November 2026.[^18][^20] The Ruby source tree lists minitest 6.1.0 as a bundled gem.[^5]

## References
