# Setting Up Personal Access Token (PAT) for Auto-Release

## Why is this needed?

GitHub Actions has a security feature where workflows triggered by `GITHUB_TOKEN` cannot trigger other workflows. This prevents the auto-tag workflow from triggering the release workflow.

To fix this, we need to create a Personal Access Token (PAT) with the necessary permissions.

## Setup Instructions

### 1. Create a Fine-Grained Personal Access Token

1. Go to **GitHub Settings** → **Developer settings** → **Personal access tokens** → **Fine-grained tokens**
2. Click **"Generate new token"**
3. Fill in the details:
   - **Token name**: `VitaBreak Auto-Release`
   - **Expiration**: Choose your preference (90 days, 1 year, or custom)
   - **Repository access**: Select "Only select repositories" → Choose `VitaBreak`
4. Under **Repository permissions**, set:
   - **Contents**: Read and write (for creating tags and releases)
   - **Metadata**: Read (automatically selected)
   - **Workflows**: Read and write (to trigger workflows)
5. Click **"Generate token"**
6. **Copy the token immediately** (you won't be able to see it again!)

### 2. Add Token to Repository Secrets

1. Go to your repository: `https://github.com/kion-dgl/VitaBreak`
2. Click **Settings** → **Secrets and variables** → **Actions**
3. Click **"New repository secret"**
4. Name: `PAT_TOKEN`
5. Secret: Paste the token you copied
6. Click **"Add secret"**

### 3. Test the Setup

1. Create a new PR and merge it to `main`
2. The auto-tag workflow should run and create a tag
3. The release workflow should automatically trigger and build all platforms
4. A GitHub Release should be created with all artifacts

## Troubleshooting

### Release workflow still not triggering?

Check the auto-tag workflow logs:
```bash
gh run list --workflow=auto-tag.yml --limit 1
gh run view <run-id> --log
```

Look for "Created and pushed tag" - if this succeeds but release doesn't trigger, verify:
- PAT_TOKEN secret exists in repository settings
- PAT has "Workflows: Read and write" permission
- PAT hasn't expired

### Need to update the token?

1. Generate a new PAT following steps above
2. Update the `PAT_TOKEN` secret in repository settings
3. No code changes needed!

## Alternative: Manual Tagging

If you prefer not to use a PAT, you can still create releases manually:

```bash
git tag v1.0.0
git push origin v1.0.0
```

This will trigger the release workflow directly.
