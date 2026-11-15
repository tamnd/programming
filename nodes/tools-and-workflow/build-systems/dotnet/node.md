---
tag: EMF0R0
kind: tool
slug: dotnet
name: The dotnet CLI
aliases: [.NET CLI, dotnet, .NET SDK, dotnet new, dotnet run, dotnet publish, global.json]
area: tools-and-workflow/build-systems
level: core
summary: >-
  The dotnet CLI is the command line of the .NET SDK. One driver, dotnet, makes projects from templates, restores NuGet packages, and builds, runs, tests, packs and publishes .NET programs.
role: [build, package-manager]
licence: mit
repo: https://github.com/dotnet/sdk
homepage: https://learn.microsoft.com/en-us/dotnet/core/tools/
see_also: [msbuild, csharp, build-system-concept, build-a-project, incremental-builds, maven, gradle, build-graph]
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/
    title: Microsoft Learn, .NET CLI
    locator: the introduction; Command structure; Driver; CLI commands
  - url: https://learn.microsoft.com/en-us/dotnet/core/sdk
    title: Microsoft Learn, .NET SDK overview
    locator: the introduction; How to install the .NET SDK
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-build
    title: Microsoft Learn, dotnet build command
    locator: Description; MSBuild; Options
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-restore
    title: Microsoft Learn, dotnet restore command
    locator: Description; Implicit restore
  - url: https://github.com/dotnet/sdk/blob/main/LICENSE
    title: .NET SDK licence
    locator: the whole file (MIT License)
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-new
    title: Microsoft Learn, dotnet new <TEMPLATE>
    locator: Description
  - url: https://learn.microsoft.com/en-us/dotnet/core/project-sdk/overview
    title: Microsoft Learn, .NET project SDK overview
    locator: Available SDKs; Project files; Default includes and excludes
  - url: https://learn.microsoft.com/en-us/dotnet/core/project-sdk/msbuild-props
    title: Microsoft Learn, MSBuild properties for Microsoft.NET.Sdk
    locator: ImplicitUsings
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-run
    title: Microsoft Learn, dotnet run command
    locator: Description; Implicit restore
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/global-json
    title: Microsoft Learn, global.json overview
    locator: the introduction; Matching rules
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-msbuild
    title: Microsoft Learn, dotnet msbuild command
    locator: Description
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-publish
    title: Microsoft Learn, dotnet publish command
    locator: Description; Options
  - url: https://learn.microsoft.com/en-us/dotnet/core/rid-catalog
    title: Microsoft Learn, .NET Runtime Identifier (RID) catalog
    locator: the introduction; Known RIDs
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-test
    title: Microsoft Learn, dotnet test command
    locator: Description; Choose a test runner
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-pack
    title: Microsoft Learn, dotnet pack command
    locator: Description
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-sln
    title: Microsoft Learn, dotnet sln command
    locator: Description; Create a solution file; list
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/global-tools
    title: Microsoft Learn, .NET tools
    locator: the introduction; Install a local tool
  - url: https://learn.microsoft.com/en-us/dotnet/core/sdk/artifacts-output
    title: Microsoft Learn, Artifacts output layout
    locator: the introduction; Examples; How to configure
  - url: https://learn.microsoft.com/en-us/dotnet/core/tools/telemetry
    title: Microsoft Learn, .NET SDK and .NET CLI telemetry
    locator: Scope; How to opt out
---

## What it is

The .NET command-line interface (CLI) is "a cross-platform toolchain for developing, building, running, and publishing .NET applications". It comes with the .NET SDK.[^1] The SDK is "a set of libraries and tools that developers use to create .NET applications and libraries", and it has three parts: the .NET CLI, the .NET runtime and libraries, and the `dotnet` driver.[^2]

A command has the driver `dotnet`, a command, and possibly arguments and options, as in `dotnet build --output ./build_output`.[^1] The driver has two jobs. When it gets the path of a `.dll` file and no command, it runs a framework-dependent app, as in `dotnet my_app.dll`. When it gets a command, such as `dotnet build`, it first selects the version of the SDK, and then the SDK runs the command.[^1]

The CLI does the work of a build tool and of a package manager. `dotnet build` uses [[msbuild]] to build the project, and `dotnet restore` uses NuGet to find the packages that the project needs and to download them if necessary.[^3][^4] The source code of the SDK is free software under the MIT License.[^5]

## Install and first use

The SDK has native installers, such as DEB packages on Ubuntu and MSI bundles on Windows, and an installation shell script.[^2] By default, the SDK installs "side-by-side": many versions can be on one computer at the same time.[^2]

`dotnet new` makes a project or other files from a template. It calls the template engine, which makes the files on disk from the template and its options.[^6] The script below makes a console program from the template `console`, and shows the files that the template made. The `grep` keeps two lines of the output of `dotnet new`, and the `sed` removes the empty lines of the project file. The run used the .NET SDK 10.0.400 on the build machine of this site.

```bash run timeout=30 id=dotnet-new
# Make a console program from a template.
d=$(mktemp -d)
cd "$d"
dotnet --version
dotnet new console -n hello -o hello 2>&1 | grep -E 'created successfully|Restore succeeded'
echo "--- the files"
ls hello
echo "--- the project file"
sed -e '1s/^\xEF\xBB\xBF//' -e '/^$/d' hello/hello.csproj
echo "--- the code"
grep -o 'Console.*' hello/Program.cs
cd / && rm -rf "${d:?}"
```

```output for=dotnet-new
10.0.400
The template "Console App" was created successfully.
Restore succeeded.
--- the files
Program.cs
hello.csproj
obj
--- the project file
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>
</Project>
--- the code
Console.WriteLine("Hello, World!");
```

The project has a project file, `hello.csproj`, and one source file, `Program.cs`, with one line of [[csharp]]. The project file is an [[msbuild]] project file. Its first line names the SDK `Microsoft.NET.Sdk`, so MSBuild imports `Sdk.props` at the top of the project and `Sdk.targets` at the bottom, with the build logic of .NET.[^7] The SDK also adds the source files, such as `*.cs`, as items to compile, so the project file does not list `Program.cs`.[^7] `ImplicitUsings` adds global `using` directives for a set of default namespaces, so the code can use `Console` without a `using System;` line.[^8] `dotnet new` restored the project at once, because it is one of the commands that run a restore implicitly.[^4] The directory `obj` holds the temporary files of the build.[^9]

`dotnet run` runs a program from its source code with one command. It builds the code with `dotnet build` first, and it writes the output files to `bin/<configuration>/<target>`.[^9] The script below writes the same two files by hand, without the template. It runs the project with `dotnet run`, lists the output of the build, and then runs the built `.dll` file with the driver.

```bash run timeout=30 id=dotnet-run
# Write the project by hand, build and run it with one command,
# then run the built file with the driver.
d=$(mktemp -d)
cd "$d"
cat > hello.csproj <<'XML'
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>net10.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>
</Project>
XML
echo 'Console.WriteLine("Hello, World!");' > Program.cs
dotnet run
echo "--- the output of the build"
ls bin/Debug/net10.0
dotnet bin/Debug/net10.0/hello.dll
cd / && rm -rf "${d:?}"
```

```output for=dotnet-run
Hello, World!
--- the output of the build
hello
hello.deps.json
hello.dll
hello.pdb
hello.runtimeconfig.json
Hello, World!
```

The configuration was `Debug`, the default for most projects.[^3] The files in `bin/Debug/net10.0` are the files that the documentation of `dotnet build` lists: the code of the project in Intermediate Language (IL) in `hello.dll`, an executable `hello` that runs the program, the debug symbols in `hello.pdb`, the dependencies of the program in `hello.deps.json`, and the shared runtime and its version in `hello.runtimeconfig.json`.[^3] The last line came from the driver, which ran `hello.dll` with no command.[^1]

## Concepts of the tool

**The SDK version.** Before the driver runs a command, it selects an SDK. If there is no `global.json` file, it uses the highest SDK version on the computer.[^1] A `global.json` file selects the version of the SDK, and so the version of the CLI. This is a different thing from the version of the runtime that a project targets.[^10] The SDK searches for `global.json` up through the ancestor directories of its start location.[^10]

**Implicit restore.** A .NET project names the NuGet packages that it needs in its project file, and `dotnet restore` gets them.[^4] The commands `new`, `build`, `run`, `test`, `publish` and `pack` run a restore implicitly when one is necessary. The option `--no-restore` stops the implicit restore, for example in a build system that must control when the network is used.[^4]

**Build and incremental build.** `dotnet build` builds a project, a solution or a file-based app, and its dependencies.[^3] It supports parallel and incremental builds, and it also takes MSBuild options, such as `-p` to set a property.[^3] `dotnet msbuild` gives access to all of MSBuild, and `dotnet build` is the same as `dotnet msbuild -restore`.[^11] The page [[msbuild]] shows how MSBuild skips a target when its outputs are up to date.

**Publish.** `dotnet run` is for development. Microsoft does not recommend it for production: `dotnet publish` makes the files to deploy.[^9] `dotnet publish` compiles the app, reads its dependencies from the project file, and puts the result in a directory.[^12] A framework-dependent app needs a compatible .NET runtime on the target computer. A self-contained app, made with `--self-contained`, has the .NET runtime in it, so the target computer does not need one.[^12] The option `-r` gives the target platform as a runtime identifier (RID), such as `linux-x64`, `win-x64` or `osx-x64`.[^12][^13] The default output directory is `bin/<configuration>/<framework>/publish/`, or `bin/<configuration>/<framework>/<runtime>/publish/` for a self-contained app.[^12]

**Test, pack and solutions.** `dotnet test` builds the solution and runs the tests with VSTest or with Microsoft Testing Platform (MTP). VSTest is the default, and a `global.json` file can select MTP.[^14] `dotnet pack` builds the project and makes a NuGet package, a `.nupkg` file.[^15] `dotnet sln` lists and changes the projects in a solution file, and `dotnet new sln` makes the solution file, an `.slnx` file.[^16]

**Tools.** A .NET tool is "a special NuGet package that contains a console application".[^17] The CLI installs a tool in one of three ways:[^17]

| Kind | Where it goes | Which directories use it |
|---|---|---|
| Global tool | a default directory on the `PATH` | all directories, with one version |
| Tool-path tool | a directory that the user gives | all directories, with one version |
| Local tool | a default directory, with a record in a manifest file | the directory of the manifest and its subdirectories, so different directories can use different versions |

`dotnet new tool-manifest` makes the manifest `.config/dotnet-tools.json`. When the manifest is in the root of a repository, a contributor who clones the repository runs `dotnet tool restore` to install all its tools.[^17] The CLI runs local tools from the manifest, so Microsoft warns that the manifest must be in a controlled location.[^17]

## Common tasks

| Task | Command |
|---|---|
| Make a console project | `dotnet new console`[^1] |
| List the installed templates | `dotnet new list`[^6] |
| Build the project in the current directory | `dotnet build`[^3] |
| Build and run the project | `dotnet run`[^9] |
| Run the tests | `dotnet test`[^14] |
| Make the files to deploy for Linux on x64, with the runtime | `dotnet publish -r linux-x64 --self-contained`[^12] |
| Make a NuGet package | `dotnet pack`[^15] |
| Install the local tools of a repository | `dotnet tool restore`[^17] |
| Run all of MSBuild | `dotnet msbuild`[^11] |

## Configuration

**The project file.** A `.csproj` file is an MSBuild project file, and its properties control the build. A project can give its target platforms in the element `RuntimeIdentifier`, or as a list in `RuntimeIdentifiers`.[^13]

**global.json.** This file selects the SDK version. The `rollForward` setting gives a range of acceptable versions. This file from the documentation selects the SDK 10.0.100 or any later feature band or patch of 10.0 on the computer:[^10]

```json
{
  "sdk": {
    "version": "10.0.100",
    "rollForward": "latestFeature"
  }
}
```

**The artifacts output layout.** Without other settings, the outputs of a project go to its `bin` and `obj` directories.[^9] With the property `UseArtifactsOutput` set to `true` in a `Directory.Build.props` file, all the outputs of all the projects go into one directory, `artifacts`, next to that file. An example path is `artifacts/bin/MyApp/debug`.[^18]

**Telemetry.** The SDK collects telemetry by default in the Microsoft distributions. Telemetry is collected when a CLI command, such as `dotnet build`, runs, but not when the driver starts an app with `dotnet [path-to-app].dll`. To opt out, set the environment variable `DOTNET_CLI_TELEMETRY_OPTOUT` to `1` or `true`.[^19] The runs on this page set this variable.

## Alternatives

- [[msbuild]]: the build engine under `dotnet build`, which can also run alone, as `MSBuild.exe` on Windows.
- [[maven]] and [[gradle]]: the build tools of Java, which also get packages from a repository.

The page [[build-a-project]] shows how to build a project with the common build tools.

## History and versions

The documentation of the CLI gives some changes between SDK versions:

- From the .NET 7 SDK, the options `--list`, `--search`, `--install` and `--uninstall` of `dotnet new` became the subcommands `list`, `search`, `install` and `uninstall`.[^6]
- The .NET 10 SDK added the choice of the test runner for `dotnet test`. Before it, `dotnet test` always used VSTest.[^14]
- In .NET 10, `dotnet pack` can also take a `.nuspec` file or a file-based app, a `.cs` file, as its argument.[^15]

## References
