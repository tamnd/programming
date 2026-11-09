# Licences

Each part of this repository has a licence. `REUSE.toml` sets the licence for each path.

| Part | Paths | Licence |
| --- | --- | --- |
| Prose | `nodes/**/node.md`, `nodes/**/README.md` | CC BY-SA 4.0 |
| Code, examples, tests and scenes | `nodes/**/examples/**`, `nodes/**/impl/**`, `nodes/**/viz/**`, `nodes/**/probes/**` | MIT |
| Metadata | `registers/**`, `topics/*.yaml`, `manifests/**`, `prov/**`, `reviews/**` | CC0 1.0 |

The front matter of `node.md` is the YAML block between the first two `---` lines. It is metadata, so it is under CC0 1.0. The rest of `node.md` is under CC BY-SA 4.0. REUSE gives one licence expression for each file, so `node.md` has the expression `CC-BY-SA-4.0 AND CC0-1.0`.

The example files in `examples/` mix prose and code. They are under MIT, so that a reader can copy the code into any program.
