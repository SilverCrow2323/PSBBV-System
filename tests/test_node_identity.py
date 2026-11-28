"""Verify the PSBBV node's declared identity and conservative readiness state."""
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
EXPECTED_ID = "factory.spdw.psbbv.system"
EXPECTED_CATEGORY = "system"
EXPECTED_NAME = "PSBBV System"


class NodeIdentityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.manifest = json.loads((ROOT / "psbbv-node.json").read_text(encoding="utf-8"))

    def test_manifest_identity_matches_repository_role(self):
        self.assertEqual(self.manifest["id"], EXPECTED_ID)
        self.assertEqual(self.manifest["name"], EXPECTED_NAME)
        self.assertEqual(self.manifest["category"], EXPECTED_CATEGORY)

    def test_runtime_remains_builtin(self):
        self.assertEqual(self.manifest["runtime"]["kind"], "builtin")

    def test_capabilities_are_namespaced_to_node_domain(self):
        for capability in self.manifest["capabilities"]:
            self.assertTrue(capability.startswith(EXPECTED_CATEGORY + "."), capability)

    def test_node_does_not_claim_ready_before_vita_qualification(self):
        self.assertEqual(self.manifest["availability"], "planned")


if __name__ == "__main__":
    unittest.main()
