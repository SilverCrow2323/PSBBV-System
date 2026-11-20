#!/usr/bin/env python3
"""Validate this node's manifest and rejection fixtures against the pinned PSBBV Core contract."""

import copy
import json
from pathlib import Path

from jsonschema import Draft202012Validator

ROOT = Path(__file__).resolve().parents[1]


def load(path: Path):
    with path.open("r", encoding="utf-8") as stream:
        return json.load(stream)


def main() -> int:
    schema = load(ROOT / "schemas" / "psbbv-node-manifest.schema.json")
    manifest = load(ROOT / "psbbv-node.json")
    Draft202012Validator.check_schema(schema)
    validator = Draft202012Validator(schema)
    errors = sorted(
        validator.iter_errors(manifest),
        key=lambda error: list(map(str, error.absolute_path)),
    )
    if errors:
        for error in errors:
            location = ".".join(str(part) for part in error.absolute_path) or "<root>"
            print(f"INVALID {location}: {error.message}")
        return 1

    invalid_cases = []
    too_many_capabilities = copy.deepcopy(manifest)
    too_many_capabilities["capabilities"] = [f"node.capability{i}" for i in range(17)]
    invalid_cases.append(("capability array overflow", too_many_capabilities))

    too_many_permissions = copy.deepcopy(manifest)
    too_many_permissions["permissions"] = [
        "media.read", "media.write", "network.client",
        "storage.private", "storage.shared.read", "input.global", "media.read"
    ]
    invalid_cases.append(("permission array overflow", too_many_permissions))

    malformed_capability = copy.deepcopy(manifest)
    malformed_capability["capabilities"] = ["Invalid.Capability"]
    invalid_cases.append(("malformed capability identifier", malformed_capability))

    for label, fixture in invalid_cases:
        if not list(validator.iter_errors(fixture)):
            print(f"INVALID TEST: {label} was not rejected by the schema.")
            return 1

    print(f"PSBBV node manifest: valid; {len(invalid_cases)} invalid contract fixtures rejected.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
