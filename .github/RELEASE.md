# Release Process

This project uses automatic versioning and releases when PRs are merged to `main`.

## ⚠️ Setup Required

For automatic releases to work, you need to set up a Personal Access Token (PAT). See [SETUP_PAT.md](./SETUP_PAT.md) for instructions.

## How It Works

1. **Create a PR** with your changes
2. **Add a version label** to the PR (optional):
   - `release` → `v1.0.0` → `v2.0.0` (breaking changes/major releases)
   - `patch` → `v1.0.0` → `v1.0.1` (bugfixes only)
   - No label → `v1.0.0` → `v1.1.0` (default: new features/minor)
3. **Merge to main** → Auto-tag workflow runs
4. **Tag is created** → Release workflow builds all platforms
5. **GitHub Release** is published with all artifacts

## Version Bump Guidelines

### Minor (default - no label needed)
- New features
- Non-breaking enhancements
- New platform support
- Most PRs should be minor bumps

### Patch (`patch` label)
- Bug fixes only
- Documentation updates
- Minor tweaks
- Use when CI catches issues

### Major (`release` label)
- Breaking API changes
- Major refactors
- Significant gameplay changes
- Official releases

## Examples

**PR Title:** "Add power-ups and multi-ball feature"
**Label:** None (default)
**Result:** `v0.1.5` → `v0.2.0` (minor bump)

**PR Title:** "Fix ball collision bug"
**Label:** `patch`
**Result:** `v0.2.0` → `v0.2.1` (patch bump)

**PR Title:** "Version 1.0 - Official Release"
**Label:** `release`
**Result:** `v0.5.3` → `v1.0.0` (major bump)

## Manual Releases

You can still create manual releases:
1. Create a tag: `git tag v1.2.3`
2. Push: `git push origin v1.2.3`
3. Release workflow runs automatically

Or use the Actions tab to manually trigger the release workflow.
