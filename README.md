# PSBBV System

**SPDW Factory · The control room, not a pile of hidden switches.**

PSBBV System owns user preferences and safe system-facing tools exposed inside PSBBV. PSBBV Core remains the authority for application lifecycle, global navigation, node registration, permissions and compatibility policy.

## Scope

- Persistent PSBBV preferences: theme, language, layout and user-configurable behavior.
- Storage, battery, firmware and application diagnostics where documented APIs permit.
- Safe cache inspection and cleanup with clear size estimates and confirmation.
- Exportable diagnostic reports with secrets and device identifiers redacted.
- Accessibility and input configuration surfaces coordinated with Core.
- Health/status summaries for built-in nodes without duplicating their internal settings.

## Architecture rules

- Follow the versioned Node API and manifest contract in [PSBBV Core](https://github.com/SilverCrow2323/PSBBV).
- Do not claim privileged system settings can be changed unless a supported Vita API exists.
- Separate read-only diagnostics from mutating operations; require confirmation for changes.
- Persist preferences atomically and preserve the previous valid file on write failure.
- Redact tokens, account details, serials, MAC addresses and other identifiers from exported logs.
- Never expose arbitrary shell execution or unrestricted filesystem access to a node.

## Portable preferences codec milestone

Preferences now have a fixed 16-byte, little-endian `PSYS` record with a version byte, validated settings fields, reserved bytes and CRC-32 integrity check. Decode validates the complete record into a temporary value before updating caller state, so corrupt or unsupported records cannot partially overwrite active preferences. This is the serialization layer only; filesystem atomic-replace and backup rotation remain separate work.

Run `cmake -S . -B build && cmake --build build --parallel && ctest --test-dir build --output-on-failure`.

## Delivery order

1. Define a versioned preference schema and migration policy.
2. Implement portable validation, defaults and atomic persistence.
3. Add read-only device/application diagnostics with capability checks.
4. Add safe cache management and redacted report export.
5. Integrate shared input/accessibility settings with Core.
6. Validate persistence, recovery and lifecycle on physical Vita hardware.

## Quality gates

Schema and migration tests, malformed-state recovery, atomic-write tests, redaction tests, capability tests, VitaSDK build and physical suspend/resume/reboot validation.

## Status

Repository bootstrap and product boundary only. Planned features are not yet implemented. English UI/docs and SPDW Factory identity are required.
