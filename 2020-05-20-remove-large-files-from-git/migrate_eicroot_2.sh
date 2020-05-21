#!/bin/sh

# Create a clean mirrored clone of EicRoot locally
git clone --mirror https://gitlab.com/eic/EicRoot EicRootScratch
#git clone --mirror https://git.racf.bnl.gov/gitea/EIC/EicRoot EicRootScratch

# Make all changes to the mirrored local scratch repository first
cd EicRootScratch

# Examine current repo commits, tags, branches
git --no-pager log --graph --decorate --pretty=oneline --abbrev-commit --all

# Check that problematic file is present
git --no-pager log --all --full-history -- "input/SolenoidMap3.dat"
git diff-tree --no-commit-id --name-only -r b2ffd6 | grep input

# Check that repo size before filtering
du -h -d 0
# Previous size = 257 MB

# Remove SolenoidMap3.dat from the entire history, including updating all branches and tags
git filter-branch --force --index-filter "git rm --cached --ignore-unmatch input/SolenoidMap3.dat" --prune-empty --tag-name-filter cat -- --all

# The files still exist in history because there is one or more hidden refs under .git/refs/original

# So we delete the problematic refs
rm -Rf refs/original

# And trigger immediate garbage collection
git gc --aggressive --prune=now

# Verify that SolenoidMap3 file is gone everywhere and your branches and tags still make sense
git --no-pager log --all --full-history -- "input/SolenoidMap3.dat"
git --no-pager log --graph --decorate --pretty=oneline --abbrev-commit --all

# Verify repo size is smaller now
du -h -d 0

# Push the modified mirror to your new empty github repository
git push --mirror https://github.com/eic/EicRoot.git

# ONLY DO THIS LAST STEP AFTER YOU VERIFY THAT ALL BRANCHES AND TAGS MAKE SENSE ON THE NEW GITHUB REPO
# Force push the mirrored repository back to the origin repository at BNL, so that the old and new repositories are identical
git push --mirror --force

