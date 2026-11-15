---
tag: QDFWB5
kind: tool
slug: renovate
name: Renovate
aliases: [Renovate, Renovate Bot, Mend Renovate, renovate.json, renovatebot]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  Renovate is an open-source bot that opens pull requests to update dependencies on many platforms. This page shows renovate.json, presets, groups, schedules, automerge and minimum release age.
role: [ci]
licence: agpl-3.0-only
repo: https://github.com/renovatebot/renovate
homepage: https://docs.renovatebot.com/
see_also: [dependabot, dependency-updates, package-manager-concept, lock-files, semantic-versioning, version-constraints, pin-dependency-versions, npm-pm]
sources:
  - url: https://github.com/renovatebot/renovate/blob/main/readme.md
    title: renovatebot/renovate, readme.md
    locator: What is the Mend Renovate CLI?; Languages
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/
    title: Renovate documentation, the home page
    locator: Why use Renovate?; Supported Platforms; the banner about Renovate 44
    accessed: 2026-11-16
  - url: https://github.com/renovatebot/renovate/blob/main/license
    title: renovatebot/renovate, license
    locator: the first lines
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/bot-comparison/
    title: Renovate documentation, Bot comparison
    locator: the comparison table
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/getting-started/installing-onboarding/
    title: Renovate documentation, Installing and onboarding
    locator: Hosted GitHub.com App; Repository onboarding; No risk onboarding; Configuration location
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/getting-started/running/
    title: Renovate documentation, Running Renovate
    locator: npm package (CLI); Docker images; GitHub Action; Global config
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/configuration-options/
    title: Renovate documentation, Configuration options
    locator: Locations for configuration filenames; extends; packageRules; groupName; schedule; automerge; minimumReleaseAge; lockFileMaintenance
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/config-presets/
    title: Renovate documentation, Shareable config presets
    locator: Example configs
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/presets-config/
    title: Renovate documentation, Full config presets
    locator: config:best-practices; config:recommended
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/presets-group/
    title: Renovate documentation, Group presets
    locator: group:allNonMajor
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/noise-reduction/
    title: Renovate documentation, Noise reduction
    locator: Package Grouping
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/key-concepts/dashboard/
    title: Renovate documentation, Dependency Dashboard
    locator: Introduction; How to enable the dashboard
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/key-concepts/scheduling/
    title: Renovate documentation, Renovate scheduling
    locator: Customizing the schedule; Schedule presets
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/key-concepts/automerge/
    title: Renovate documentation, Automerge configuration and troubleshooting
    locator: Introduction; Automerge non-major updates; Faster merges with platform-native automerge
    accessed: 2026-11-16
  - url: https://docs.renovatebot.com/key-concepts/minimum-release-age/
    title: Renovate documentation, Minimum Release Age
    locator: What is Minimum Release Age?
    accessed: 2026-11-16
  - url: https://registry.npmjs.org/renovate
    title: npm registry, the metadata of the package renovate
    locator: time; license
    accessed: 2026-11-16
---

## What it is

"Renovate is an automated dependency update tool."[^1] "When Renovate runs on your repo, it looks for references to dependencies (both public and private) and, if there are newer versions available, Renovate can create pull requests to update your versions automatically."[^1] Its documentation lists these properties:[^2]

- "Get pull requests to update your dependencies and lock files."
- "Reduce noise by scheduling when Renovate creates PRs."
- "Renovate finds relevant package files automatically, including in monorepos."
- "Share your configuration with ESLint-like config presets."

Renovate is not tied to one forge. It works on Azure DevOps, AWS CodeCommit, Bitbucket Cloud, Bitbucket Server, Forgejo, Gerrit (experimental), Gitea, GitHub and GitLab.[^2] The readme says that it supports over 90 package managers.[^1] "Renovate is licensed under the GNU Affero General Public License (AGPL-3.0-only )."[^2][^3] The project is written in TypeScript.[^4]

See [[dependency-updates]] for the problem that Renovate solves, and [[dependabot]] for the bot that is built into GitHub.

## Install and first use

There are two common ways to start:

- **The hosted app.** On GitHub.com, go to the app page github.com/apps/renovate, select Install, and choose all repositories or selected repositories.[^5] The company Mend.io hosts this app.[^1]
- **Self-hosting.** "Renovate's Open Source CLI is built and distributed as the npm package renovate".[^6] You can run it "in any Node.js environment - even via npx".[^6] Renovate is also distributed as Docker images, `renovate/renovate` on Docker Hub and `ghcr.io/renovatebot/renovate`, and as a GitHub Action, `renovatebot/github-action`.[^6]

When Renovate is enabled on a repository, it first opens a pull request with the title "Configure Renovate".[^5] This pull request adds a `renovate.json` file "in the root directory, with suggested default settings".[^5] "Renovate will not make any changes to your repository or raise any further Pull Requests until after you merge the onboarding Pull Request."[^5] A minimal `renovate.json` is:

```json
{
  "$schema": "https://docs.renovatebot.com/renovate-schema.json",
  "extends": ["config:recommended"]
}
```

The file can also be `renovate.json5`, `.github/renovate.json`, `.gitlab/renovate.json`, `.renovaterc`, or a `"renovate"` section in `package.json`.[^7]

## Concepts of the tool

**Presets.** `extends` lists "Configuration presets to use or extend."[^7] A preset is a shared piece of configuration. `config:recommended` "is Renovate's default configuration. It mostly uses Renovate config defaults but adds a few smart customizations such as grouping monorepo packages together."[^8] `config:best-practices` extends `config:recommended` and adds more rules, for example `docker:pinDigests` and `helpers:pinGitHubActionDigests`; it is "Recommended for advanced users".[^9] "Presets are repo-hosted", so a team can keep its own preset in a repository and extend it from its other repositories with a name such as `github>abc/foo`.[^8]

**Package rules.** `packageRules` "is a powerful feature that lets you apply rules to individual packages or to groups of packages using regex pattern matching."[^7] Each rule has match conditions, such as `matchPackageNames` and `matchUpdateTypes`, and the settings that apply when the rule matches.[^7]

**Groups.** `groupName` puts several updates into one pull request.[^7] The preset `group:allNonMajor` will "Group all minor and patch updates together."[^10] The default presets include `group:monorepos`, so "sibling" packages from known monorepos "will always be grouped into the same branch/PR".[^11]

**The Dependency Dashboard.** "When the Dependency Dashboard is enabled, Renovate will create a new issue in the repository."[^12] The issue gives an overview of the status of all updates. It also enables an approval workflow for new upgrades, with checkboxes in the issue.[^12] The preset `config:recommended` includes `:dependencyDashboard`, so the dashboard is on by default.[^9] The bot comparison table lists the dashboard as a feature of Renovate that Dependabot does not have.[^4]

**Schedules.** `schedule` limits the times "when you are willing to allow Renovate to create branches."[^7] "Setting a schedule does not itself cause or trigger Renovate to run."[^7] The value is a list of times. "We recommend you use the cron syntax in your Renovate schedules."[^13]

**Automerge.** "By default, Renovate raises PRs but leaves them to someone or something else to merge them."[^7] With `automerge`, "Renovate will wait for the required tests to pass before it automerges."[^14]

**Minimum release age.** "minimumReleaseAge is a feature that requires Renovate to wait for a specified amount of time before suggesting a dependency update."[^15] Other ecosystems call this a dependency cooldown.[^15] The documentation says that a wait of 14 days gives "security researchers and automated security tools" time "to catch malicious intent in packages".[^15]

## Common tasks

A configuration that uses the recommended preset, creates branches before 5 AM, waits three days for new releases, groups the non-major updates, and merges the non-major updates of stable packages automatically:

```json
{
  "$schema": "https://docs.renovatebot.com/renovate-schema.json",
  "extends": ["config:recommended"],
  "schedule": ["* 0-4 * * *"],
  "minimumReleaseAge": "3 days",
  "packageRules": [
    {
      "groupName": "all non-major dependencies",
      "matchPackageNames": ["*"],
      "matchUpdateTypes": ["minor", "patch"]
    },
    {
      "matchUpdateTypes": ["minor", "patch"],
      "matchCurrentVersion": "!/^0/",
      "automerge": true
    }
  ]
}
```

The documentation gives `* 0-4 * * *` as the cron syntax for "before 5:00am".[^13] The rule with `matchCurrentVersion` follows the documented example for non-major updates. It excludes "any dependencies which are pre-1.0.0 because those can make breaking changes at any time according to the SemVer spec".[^14] See [[semantic-versioning]].

**Refresh the lock file.** `lockFileMaintenance` refreshes the lock file completely and leaves the version ranges in the manifest unchanged.[^14] Its default schedule is `before 4am on monday`.[^7] To turn it on and merge it automatically:[^14]

```json
{
  "lockFileMaintenance": { "enabled": true, "automerge": true }
}
```

See [[lock-files]].

**Turn on the dashboard.** Add the preset `:dependencyDashboard` to `extends`, or set `"dependencyDashboard": { "enabled": true }`.[^12]

## Configuration

The configuration of a repository is in `renovate.json` or one of the other file names.[^7] A self-hosted instance also has a global config, which "can be set by using either: a config file, or an additional config file, or environment variables, or CLI parameters".[^6] By default, the global config file is `config.js`.[^6] Environment variables have the prefix `RENOVATE_`, for example `RENOVATE_TOKEN`.[^6] The token gives Renovate access to the repositories, so keep it in the secret store of the CI system.

On GitHub, Renovate can read the vulnerability alerts of the repository and raise pull requests that fix them. The option `vulnerabilityAlerts` customizes these pull requests. By default, they have no schedule and no minimum release age.[^7]

## Alternatives

Dependabot is the update bot of GitHub; see [[dependabot]]. The bot comparison lists these differences:[^4]

| | Renovate | Dependabot |
|---|---|---|
| Built into GitHub | No, requires app or self-hosting | Yes |
| Dependency Dashboard | Yes | No |
| Licence | GNU Affero General Public License | MIT License |
| Language of the project | TypeScript | Ruby |

On scheduling, the table says: "By default, Renovate runs as often as it is allowed to".[^4]

## History and versions

The npm package `renovate` was created on 2017-01-11.[^16] Some major versions and their release dates on npm are:[^16]

| Version | Released |
|---|---|
| 10.0.0 | 2017-11-01 |
| 19.0.0 | 2019-07-15 |
| 25.0.0 | 2021-04-22 |
| 35.0.0 | 2023-03-10 |
| 39.0.0 | 2024-11-04 |
| 42.0.0 | 2025-11-06 |
| 43.0.0 | 2026-01-29 |
| 44.0.0 | 2026-07-29 |

About version 44, the documentation says: "Renovate 44 was accidentally released as a major version, even though the changes were non-breaking."[^2]

## References
