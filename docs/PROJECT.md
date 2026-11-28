# Project: PSBBV System

## Purpose
Own persistent PSBBV preferences, read-only device diagnostics, safe cache management and redacted diagnostic exports. It does not expose unsupported privileged Vita settings or arbitrary shell execution.

## Users and use cases
- PlayStation Vita owners using PSBBV as a console-style native shell.
- Maintainers integrating a built-in node against the versioned PSBBV Core API.
- Testers validating bounded state transitions, recovery and hardware constraints.

## Scope and boundaries
This repository owns the system capability domain. PSBBV Core owns global navigation, focus, lifecycle, shared operation UI, permissions and node registration. This node must not silently overwrite user data, execute arbitrary commands or claim operations that are not implemented.

## Target and architecture
- Native PS Vita / PS TV, VitaSDK and the PSBBV built-in-node model.
- Portable domain/state contracts first, with filesystem/network work behind narrow adapters.
- Manifest: psbbv-node.json. Pinned contract schema: schemas/psbbv-node-manifest.schema.json.
- CI: schema/manifest checks and host regression tests; VitaSDK and physical-device gates are separate.

## Dependencies and integration
The node consumes the versioned Node API and manifest contract from https://github.com/SilverCrow2323/PSBBV. The local schema is a pinned compatibility copy and must be reconciled with Core changes. No Python runtime or dynamic third-party loader is assumed on Vita.

## Safety, accessibility and localization
Bound all input, network reads and allocations. Reject traversal and symlink escapes; use temporary files and atomic finalization for transfers/persistence. Require confirmation for destructive operations, redact credentials and identifiers, and preserve controller accessibility. Technical documentation and UI strings are English.

## Verified baseline
The node manifest and schema-validation harness exist; availability remains planned. The project is not a complete transfer or settings service until domain code, host tests, VitaSDK integration and device recovery tests are present. The staged plan is in docs/ROADMAP.md.
