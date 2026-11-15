---
tag: JVV03T
kind: tool
slug: dependabot
name: Dependabot
aliases: [Dependabot, dependabot.yml, Dependabot alerts, Dependabot security updates, Dependabot version updates, dependabot-core]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Dependabot is the service of GitHub that sends alerts for vulnerable dependencies and opens pull requests that update them. This page shows dependabot.yml, schedules, groups, cooldown and auto-merge.
role: [ci]
licence: mit
repo: https://github.com/dependabot/dependabot-core
homepage: https://docs.github.com/en/code-security/dependabot
see_also: [dependency-updates, package-manager-concept, version-constraints, lock-files, pin-dependency-versions, semantic-versioning, package-registries, add-a-dependency]
sources:
  - url: https://docs.github.com/en/code-security/dependabot/dependabot-alerts/about-dependabot-alerts
    title: GitHub Docs, About Dependabot alerts
    locator: the introduction; When Dependabot sends alerts
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/dependabot/dependabot-security-updates/about-dependabot-security-updates
    title: GitHub Docs, About Dependabot security updates
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/dependabot/dependabot-version-updates/about-dependabot-version-updates
    title: GitHub Docs, About Dependabot version updates
    locator: the introduction
    accessed: 2026-11-16
  - url: https://github.com/dependabot/dependabot-core/blob/main/README.md
    title: dependabot-core, README
    locator: What is Dependabot-Core?; Dependabot CLI
    accessed: 2026-11-16
  - url: https://github.com/dependabot/dependabot-core/blob/main/LICENSE
    title: dependabot-core, LICENSE
    locator: the first line
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/dependabot/dependabot-version-updates/configuring-dependabot-version-updates
    title: GitHub Docs, Configuring Dependabot version updates
    locator: Enabling Dependabot version updates
    accessed: 2026-11-16
  - url: https://github.blog/news-insights/product-news/keep-all-your-packages-up-to-date-with-dependabot/
    title: GitHub Blog, Keep all your packages up to date with Dependabot (2020-06-01)
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/reference/supply-chain-security/dependabot-options-reference
    title: GitHub Docs, Dependabot options reference
    locator: Required keys; package-ecosystem; open-pull-requests-limit; cooldown; ignore
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/dependabot/dependabot-version-updates/optimizing-pr-creation-version-updates
    title: GitHub Docs, Optimizing the creation of pull requests for Dependabot version updates
    locator: Controlling the frequency; Prioritizing meaningful updates with groups
    accessed: 2026-11-16
  - url: https://docs.github.com/en/code-security/dependabot/working-with-dependabot/automating-dependabot-with-github-actions
    title: GitHub Docs, Automating Dependabot with GitHub Actions
    locator: Fetching metadata about a pull request; Enabling automerge on a pull request
    accessed: 2026-11-16
  - url: https://github.blog/news-insights/product-news/introducing-new-ways-to-keep-your-code-secure/
    title: GitHub Blog, Introducing new ways to keep your code secure (2019-05-23)
    locator: Automated security fixes with Dependabot
    accessed: 2026-11-16
---

## What it is

Dependabot is a service of GitHub that watches the dependencies of a repository. It has three parts:

- **Dependabot alerts.** "Dependabot alerts notify you about vulnerable dependencies so you can upgrade to secure versions and protect your project."[^1] Dependabot sends an alert when "A new vulnerability is added to the GitHub Advisory Database", or when the dependency graph of the repository changes.[^1]
- **Dependabot security updates.** "Dependabot can fix vulnerable dependencies for you by raising pull requests with security updates."[^2]
- **Dependabot version updates.** "When Dependabot identifies an outdated dependency, it raises a pull request to update the manifest to the latest version of the dependency."[^3] The tests of the project run on the pull request, and a person reviews and merges it.

The service runs on GitHub, so there is nothing to install. The code that makes the updates is Dependabot-Core, "the library at the heart of" the security and version updates.[^4] Its licence is the MIT License.[^5] The open-source Dependabot CLI runs the same updates outside GitHub, for example in another CI system.[^4] See [[dependency-updates]] for the problem that Dependabot solves.

## Install and first use

"You enable Dependabot version updates by committing a dependabot.yml configuration file to your repository."[^6] The file is `.github/dependabot.yml`. This file checks the npm packages in the root directory each week, and the actions of the GitHub Actions workflows each month:

```yaml
# .github/dependabot.yml
version: 2
updates:
  - package-ecosystem: "npm"
    directory: "/"
    schedule:
      interval: "weekly"
  - package-ecosystem: "github-actions"
    directory: "/"
    schedule:
      interval: "monthly"
```

After the commit, Dependabot opens one pull request for each dependency that has a newer version. Each pull request changes the manifest and the lock file, and its description has the release notes and the changelog of the new version.[^3] Security updates are turned on in the settings of the repository, in the section Security & analysis, and they need no file.[^7]

## Concepts of the tool

**The required keys.** `version` is "Always: 2".[^8] `updates` has one entry for each package manager. Each entry needs `package-ecosystem`, `directory` or `directories`, and `schedule.interval`.[^8] The interval is one of `daily`, `weekly`, `monthly`, `quarterly`, `semiannually`, `yearly` and `cron`.[^8]

**Ecosystems.** The value of `package-ecosystem` names the package manager. Some values are `npm`, `pip`, `uv`, `gomod`, `cargo`, `nuget`, `maven`, `gradle`, `bundler`, `docker` and `github-actions`.[^8] The repository "must also contain a dependency manifest or lock file for each package manager".[^8]

**The limit of open pull requests.** By default, "If five pull requests with version updates are open, no further pull requests are raised until some of those open requests are merged or closed."[^8] `open-pull-requests-limit` changes this number, and the value 0 turns version updates off.[^8] Security updates have "a separate, fixed limit of 10 open pull requests".[^8]

**Groups.** `groups` puts several updates into one pull request. A group selects dependencies with `patterns`, `exclude-patterns`, `dependency-type` and `update-types`.[^9] One pull request for all the development tools is easier to review than ten.

**Ignore.** `ignore` stops updates for some dependencies or some versions. With `update-types: ["version-update:semver-major"]`, Dependabot does not propose major versions; see [[semantic-versioning]].[^8]

**Cooldown.** `cooldown` "Defines a cooldown period for dependency updates, allowing updates to be delayed for a configurable number of days".[^8] With a cooldown, a new release must be a few days old before Dependabot proposes it. "The cooldown option is only available for version updates, not security updates."[^8]

## Common tasks

A configuration with a group, an ignore rule and a cooldown:

```yaml
version: 2
updates:
  - package-ecosystem: "pip"
    directory: "/"
    schedule:
      interval: "weekly"
    open-pull-requests-limit: 10
    groups:
      dev-tools:
        dependency-type: "development"
        patterns:
          - "pytest*"
          - "ruff"
    ignore:
      - dependency-name: "django"
        update-types: ["version-update:semver-major"]
    cooldown:
      default-days: 5
      semver-major-days: 30
```

**Merge the safe updates automatically.** A workflow of GitHub Actions can read the metadata of a pull request from Dependabot with the action `dependabot/fetch-metadata`, which gives `dependency-names`, `dependency-type` and `update-type`.[^10] The workflow can then turn on auto-merge with `gh pr merge --auto`, for example for patch updates only.[^10] Auto-merge lets "the pull request to be merged when any tests and approvals required by the branch protection rules are successfully met".[^10]

```yaml
# .github/workflows/dependabot-auto-merge.yml (the important steps)
on: pull_request
permissions:
  contents: write
  pull-requests: write
jobs:
  auto-merge:
    runs-on: ubuntu-latest
    if: github.event.pull_request.user.login == 'dependabot[bot]'
    steps:
      - id: metadata
        uses: dependabot/fetch-metadata@d7267f607e9d3fb96fc2fbe83e0af444713e90b7
      - if: steps.metadata.outputs.update-type == 'version-update:semver-patch'
        run: gh pr merge --auto --merge "$PR_URL"
        env:
          PR_URL: ${{ github.event.pull_request.html_url }}
          GH_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

The documentation pins the action to a full commit hash, as this workflow does.[^10]

## Configuration

All the configuration is in `.github/dependabot.yml`.[^6] The top-level key `registries` gives "authentication details that Dependabot can use to access private package registries".[^8] Other keys of an entry are `target-branch`, `labels`, `assignees`, `commit-message`, `versioning-strategy` and `allow`.[^8]

## Alternatives

Renovate is an open-source bot with the same purpose; see [[renovate]]. Without a bot, a scheduled CI job can run the update command of the package manager, such as `npm outdated`; see [[dependency-updates]].

## History and versions

On 2019-05-23, GitHub announced: "we’ve acquired and integrated Dependabot into GitHub". GitHub would "monitor your dependencies for known security vulnerabilities and automatically open pull requests to update them to the minimum required version".[^11] On 2020-06-01, GitHub announced "Dependabot version updates, which keep all of your packages updated on a regular basis".[^7] Dependabot is a service, so it has no version numbers for users; the configuration format is version 2.[^8]

## References
