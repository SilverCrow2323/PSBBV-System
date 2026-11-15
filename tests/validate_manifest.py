#!/usr/bin/env python3
"""Validate this node's manifest against the pinned PSBBV Core contract."""
import json
from pathlib import Path
from jsonschema import Draft202012Validator
ROOT = Path(__file__).resolve().parents[1]
def load(path):
    with path.open("r", encoding="utf-8") as stream:
        return json.load(stream)
def main():
    schema = load(ROOT / "schemas" / "psbbv-node-manifest.schema.json")
    manifest = load(ROOT / "psbbv-node.json")
    Draft202012Validator.check_schema(schema)
    errors = sorted(Draft202012Validator(schema).iter_errors(manifest),
                    key=lambda error: list(map(str, error.absolute_path)))
    if errors:
        for error in errors:
            location = ".".join(str(part) for part in error.absolute_path) or "<root>"
            print(f"INVALID {location}: {error.message}")
        return 1
    print("PSBBV node manifest: valid against pinned Core schema.")
    return 0
if __name__ == "__main__":
    raise SystemExit(main())
