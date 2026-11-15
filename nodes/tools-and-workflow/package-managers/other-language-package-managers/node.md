---
tag: KX0R19
kind: tool
slug: other-language-package-managers
name: Other language package managers
aliases: [RubyGems, gem, Bundler, Gemfile, Gemfile.lock, Composer, composer.json, composer.lock, Packagist, Swift Package Manager, SwiftPM, Mix, Hex, mix.lock, Cabal, Hackage, opam, pub, pubspec, pessimistic operator]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  Ruby, PHP, Swift, Elixir, Haskell, OCaml and Dart each have a package manager. They share the same ideas, but the same operator, for example ~ or ^, can allow different versions.
role: [package-manager]
see_also: [package-manager-concept, version-constraints, semantic-versioning, lock-files, dependency-resolution, package-registries, npm-pm, pip-tool]
sources:
  - url: https://guides.rubygems.org/dependency_management/
    title: RubyGems Guides, How to manage dependencies with Bundler
    locator: Declaring dependencies; Checking Your Code into Version Control; Updating gems
  - url: https://getcomposer.org/doc/01-basic-usage.md
    title: Composer documentation, Basic usage
    locator: The require key; Installing dependencies; Installing from composer.lock; Autoloading
  - url: https://github.com/swiftlang/swift-package-manager
    title: Swift Package Manager, README
    locator: the introduction; Installation
  - url: https://hexdocs.pm/mix/Mix.Tasks.Deps.html
    title: Mix documentation, mix deps
    locator: the introduction; Dependency definition options
  - url: https://cabal.readthedocs.io/en/stable/getting-started.html
    title: Cabal User's Guide, Getting Started
    locator: Adding dependencies
  - url: https://opam.ocaml.org/doc/Usage.html
    title: opam documentation, Using opam
    locator: Basics; opam init; opam switch; opam pin
  - url: https://dart.dev/tools/pub/dependencies
    title: Dart documentation, Package dependencies
    locator: Version constraints; Caret syntax; Dependency overrides
  - url: https://guides.rubygems.org/what-is-a-gem/
    title: RubyGems Guides, What is a gem?
    locator: Structure of a Gem
  - url: https://guides.rubygems.org/rubygems-basics/
    title: RubyGems Guides, RubyGems Basics
    locator: Installing gems
  - url: https://getcomposer.org/doc/00-intro.md
    title: Composer documentation, Introduction
    locator: Dependency management
  - url: https://guides.rubygems.org/make-your-own-gem/
    title: RubyGems Guides, Make your own gem
    locator: Your first gem
  - url: https://guides.rubygems.org/gemfile/
    title: RubyGems Guides, gemfile (Gemfile reference)
    locator: Gems; Version
  - url: https://getcomposer.org/doc/articles/versions.md
    title: Composer documentation, Versions and constraints
    locator: Tilde Version Range (~); Caret Version Range (^)
---

## What it is

Many programming languages have a package manager of their own. The general ideas are on [[package-manager-concept]], and the pages [[npm-pm]], [[pip-tool]], [[cargo]], [[go-modules]], [[nuget-pm]], [[java-dependency-management]] and [[cpp-package-managers]] describe the tools for other languages. This page describes seven more tools:

| Language | Tool | File that lists the dependencies | Default registry |
|---|---|---|---|
| Ruby | RubyGems (`gem`) and Bundler | `Gemfile`, lock file `Gemfile.lock` | rubygems.org[^1] |
| PHP | Composer | `composer.json`, lock file `composer.lock` | Packagist.org[^2] |
| Swift | Swift Package Manager | the package manifest | services such as GitHub[^3] |
| Elixir | Mix with Hex | `mix.exs`, lock file `mix.lock` | Hex[^4] |
| Haskell | Cabal | the `.cabal` file | Hackage[^5] |
| OCaml | opam | the package definitions in a repository | the repository at opam.ocaml.org[^6] |
| Dart | pub | the pubspec, with a lockfile | pub.dev[^7] |

The tools share the same ideas: a file lists the direct dependencies with [[version-constraints]], the tool gets the [[transitive-dependencies]], and a [[lock-files|lock file]] records the exact versions. The details are different. The same operator can allow different sets of versions in two tools, so read the rules of each tool.

**RubyGems** installs Ruby packages, which are called gems. Each gem has a name, a version and a platform.[^8] The command `gem install` "downloads and installs the gem and any necessary dependencies".[^9] **Bundler** works on top of RubyGems for an application: the file `Gemfile` lists the gems, and `bundle install` installs them and writes the versions to `Gemfile.lock`.[^1]

**Composer** is "a tool for dependency management in PHP". It installs the packages of a project in a directory of that project, for example `vendor`, and by default it does not install anything globally.[^10]

**The Swift Package Manager** is "a tool for managing distribution of source code, aimed at making it easy to share your code and reuse others' code". It compiles and links Swift packages and manages dependencies and versions.[^3]

**Mix** is a tool of Elixir. The file `mix.exs` of a Mix project lists the dependencies, and by default Mix gets them with the Hex package manager.[^4]

**Cabal** is a tool for Haskell packages. Hackage is "the Haskell community's central package archive of open source software", and the field `build-depends` of the `.cabal` file lists the packages.[^5]

**opam** installs OCaml packages. By default it uses the community repository at opam.ocaml.org.[^6] `opam install PACKAGE` downloads, builds and installs the package and all its dependencies.[^6]

**pub** is the package manager of Dart. The pubspec lists only the direct dependencies, and pub handles the transitive dependencies.[^7]

## Install and first use

Of these tools, only RubyGems is on the build machine of this site, so the run below uses RubyGems. The examples for the other tools are shown as text and are not run.

The script builds three versions of a gem `greet` (1.0.0, 1.1.0 and 2.0.0) with `gem build`, as in the RubyGems guide.[^11] It also builds a gem `polite` that depends on `greet` with the constraint `~> 1.0`. Then it installs `polite` with `--local`, so that RubyGems uses only the `.gem` files in the folder and does not connect to rubygems.org. The variable `GEM_HOME` points to a temporary folder, so the script does not change the gems of the system. At the end, the script asks the class `Gem::Requirement` which versions three constraints allow. The output is from RubyGems 3.4.20.

```bash run timeout=30 id=gem-local
# Build three versions of a gem and a second gem that depends on it,
# then install the second gem from the .gem files in this folder only.
# GEM_HOME is a temporary folder, so nothing goes into the system gems.
d=$(mktemp -d)
cd "$d"
export GEM_HOME="$d/gems" GEM_PATH="$d/gems"
gem --version
# mkgem NAME VERSION [REQUIREMENT]: build NAME-VERSION.gem
mkgem() {
  src="src/$1-$2"
  mkdir -p "$src/lib"
  dep=""
  [ -n "$3" ] && dep="s.add_dependency \"greet\", \"$3\""
  cat > "$src/$1.gemspec" <<RB
Gem::Specification.new do |s|
  s.name = "$1"
  s.version = "$2"
  s.summary = "An example gem"
  s.authors = ["Example"]
  s.license = "MIT"
  s.homepage = "https://example.com/$1"
  s.files = ["lib/$1.rb"]
  $dep
end
RB
  if [ "$1" = greet ]; then
    echo "module Greet; def self.hello(n) = \"Hello, #{n}! (greet $2)\"; end" > "$src/lib/greet.rb"
  else
    printf 'require "greet"\nmodule Polite; def self.hello(n) = Greet.hello(n) + " Welcome."; end\n' > "$src/lib/polite.rb"
  fi
  (cd "$src" && gem build -q "$1.gemspec" -o "$d/$1-$2.gem" >/dev/null)
}
mkgem greet 1.0.0
mkgem greet 1.1.0
mkgem greet 2.0.0
mkgem polite 1.0.0 "~> 1.0"
ls *.gem
echo "--- gem install --local polite"
gem install --local --no-document polite
echo "--- gem list"
gem list greet
gem list polite
ruby -e 'require "polite"; puts Polite.hello("RubyGems")'
echo "--- Gem::Requirement"
ruby -e '
%w[~>\ 1.0 ~>\ 1.0.3 >=\ 1.0].each do |r|
  ok = %w[1.0.0 1.0.9 1.1.0 2.0.0].select { |v| Gem::Requirement.new(r).satisfied_by?(Gem::Version.new(v)) }
  puts "#{r.ljust(8)} allows #{ok.join(" ")}"
end'
cd / && rm -rf "${d:?}"
```

```output for=gem-local
3.4.20
greet-1.0.0.gem
greet-1.1.0.gem
greet-2.0.0.gem
polite-1.0.0.gem
--- gem install --local polite
Successfully installed greet-1.1.0
Successfully installed polite-1.0.0
2 gems installed
--- gem list
greet (1.1.0)
polite (1.0.0)
Hello, RubyGems! (greet 1.1.0) Welcome.
--- Gem::Requirement
~> 1.0   allows 1.0.0 1.0.9 1.1.0
~> 1.0.3 allows 1.0.9
>= 1.0   allows 1.0.0 1.0.9 1.1.0 2.0.0
```

RubyGems installed `greet` 1.1.0 as the dependency of `polite`. Version 2.0.0 is outside `~> 1.0`, so RubyGems did not use it, and from 1.0.0 and 1.1.0 it took the higher version. Compare [[nuget-pm]], which takes the lowest version that a range allows.

## Concepts of the tool

**Version operators.** All seven tools have constraints such as `>= 1.2`. Most also have a short operator for "compatible with this version", but the operators are not the same:

| Tool | Constraint | Means |
|---|---|---|
| RubyGems, Bundler | `~> 2.0.3` | `>= 2.0.3` and `< 2.1`[^12] |
| RubyGems, Bundler | `~> 2.1` | `>= 2.1` and `< 3.0`[^12] |
| Composer | `~1.2` | `>=1.2 <2.0.0`[^13] |
| Composer | `~1.2.3` | `>=1.2.3 <1.3.0`[^13] |
| Composer | `^1.2.3` | `>=1.2.3 <2.0.0`[^13] |
| Composer | `^0.3` | `>=0.3.0 <0.4.0`[^13] |
| Dart pub | `^1.3.0` | `>=1.3.0 <2.0.0`[^7] |
| Dart pub | `^0.1.2` | `>=0.1.2 <0.2.0`[^7] |
| Cabal | `^>=1.0.0.0` | version 1.0.0.0 or a later minor release with the same major version[^5] |

The Gemfile reference calls `~>` the "pessimistic" specifier, because it sets both a lower and an upper bound.[^12] The tilde of Composer lets the last digit that you give go up.[^13] The caret of Composer and of Dart allows the versions up to the next one that can break compatibility under [[semantic-versioning]]. For a version below 1.0, that is the next minor version.[^13][^7] The Composer documentation recommends `^` "for maximum interoperability when writing library code".[^13]

**Lock files.** Bundler writes "a snapshot of all of the gems and versions that it installed" to `Gemfile.lock`, and the guide says to commit this file to version control.[^1] Composer writes all the packages and their exact versions to `composer.lock` when you run `composer update`. When `composer.lock` is present, `composer install` uses the exact versions in it.[^2] Mix ignores the lockfile `mix.lock` of a project when another project uses that project as a dependency.[^4] Dart pub updates the lockfile of the pubspec when you run `dart pub get` or `dart pub upgrade`.[^7]

**Where the packages go.** Composer puts the packages in the project, and for libraries that give autoload information it makes the file `vendor/autoload.php`. A PHP file includes it to use the classes of the libraries.[^2] In deployment mode, Bundler installs the gems into `vendor/bundle` in the application.[^1] opam keeps its state in the directory `~/.opam`.[^6]

**Other sources.** Most of these tools can also get a package from a Git repository or a local path. Mix accepts `git:` and `path:` dependencies,[^4] and pub accepts hosted, Git, path and SDK dependencies.[^7] `opam pin` can point a package to a version, a local path or a Git branch or commit.[^6]

## Common tasks

| Task | RubyGems and Bundler | Composer | Dart pub |
|---|---|---|---|
| Declare a dependency | `gem 'nokogiri', '~> 1.19.4'` in `Gemfile`[^1] | `"require": {"monolog/monolog": "2.0.*"}` in `composer.json`[^2] | `transmogrify: ^1.2.0` under `dependencies` in `pubspec.yaml`[^7] |
| Install the locked versions | `bundle install`[^1] | `composer install`[^2] | `dart pub get`[^7] |
| Update the versions in the lock file | `bundle update rails puma`[^1] | `composer update`[^2] | `dart pub upgrade`[^7] |

| Task | Command |
|---|---|
| Install one gem and its dependencies | `gem install NAME`[^9] |
| Build a gem from a gemspec | `gem build NAME.gemspec`[^11] |
| Run a gem executable with the versions of the bundle | `bundle exec rspec spec/models`[^1] |
| Show the dependencies of a Mix project and their status | `mix deps`[^4] |
| Update the package index of Cabal | `cabal update`[^5] |
| Find, show and install an opam package | `opam search QUERY`, `opam show PACKAGE`, `opam install PACKAGE`[^6] |
| Use a different OCaml compiler | `opam switch create [name] <package-or-version>`[^6] |

Bundler gives a warning about `bundle update` with no arguments: run it "only when you intend to move every gem to the newest version your Gemfile allows".[^1]

## Configuration

**Bundler.** The Gemfile names the source of the gems, for example `source 'https://rubygems.org'`, and a private gem server can be given for some gems.[^1] On a deployment machine or in CI, the guide recommends deployment mode, with `bundle config set --local deployment true` before `bundle install`. Deployment mode needs an up-to-date `Gemfile.lock`.[^1]

**Composer.** The key `repositories` of `composer.json` adds package repositories. Without it, Composer looks for packages on Packagist.org.[^2]

**Mix.** A dependency in `mix.exs` has one of the forms `{app, requirement}`, `{app, opts}` or `{app, requirement, opts}`. The options include `git:`, `path:` and `only:`.[^4] Hex expects a version requirement for each dependency and gives a warning if there is none.[^4]

**Dart pub.** The section `dependency_overrides` of the pubspec replaces a dependency with another version or a local path, for example to test a fix.[^7]

**opam.** A switch is an installation with its own prefix, its own set of packages and its own compiler version. After `opam switch create`, run `eval $(opam env)` to update the `PATH`.[^6] The command `opam repo add` adds a package repository to the community repository, or replaces it.[^6]

## Alternatives

A project can copy the source of its dependencies into its own repository instead: see [[vendoring]]. A system package manager can also install libraries for some languages: see [[apt-tool]], [[homebrew-pm]], [[nix]] and [[other-system-package-managers]]. For the general rules of choosing versions, see [[dependency-resolution]] and [[pin-dependency-versions]].

## History and versions

| Version | Event |
|---|---|
| Xcode 8.0 | The Swift Package Manager is included in Xcode 8.0 and in all later releases.[^3] |
| opam 2.1 | opam can be run with the command line of an earlier version, with the option `--cli`.[^6] |
| Mix 1.20.5, Cabal 3.18.2.0 | The versions of the Mix and Cabal documentation that this page uses.[^4][^5] |

## References
