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

    duplicate_platform = copy.deepcopy(manifest)
    duplicate_platform["platforms"] = ["psvita", "psvita"]
    invalid_cases.append(("duplicate platform declaration", duplicate_platform))

    duplicate_capability = copy.deepcopy(manifest)
    duplicate_capability["capabilities"] = [manifest["capabilities"][0], manifest["capabilities"][0]]
    invalid_cases.append(("duplicate capability declaration", duplicate_capability))

    unknown_runtime = copy.deepcopy(manifest)
    unknown_runtime["runtime"]["kind"] = "python"
    invalid_cases.append(("unsupported runtime kind", unknown_runtime))

    invalid_availability = copy.deepcopy(manifest)
    invalid_availability["availability"] = "coming-soon"
    invalid_cases.append(("unknown availability state", invalid_availability))

    extra_field = copy.deepcopy(manifest)
    extra_field["debug"] = True
    invalid_cases.append(("undeclared top-level field", extra_field))

    invalid_version = copy.deepcopy(manifest)
    invalid_version["version"] = "v0.1"
    invalid_cases.append(("malformed semantic version", invalid_version))

    for label, fixture in invalid_cases:
        if not list(validator.iter_errors(fixture)):
            print(f"INVALID TEST: {label} was not rejected by the schema.")
            return 1

    print(f"PSBBV node manifest: valid; {len(invalid_cases)} invalid contract fixtures rejected.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
