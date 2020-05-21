#!/bin/sh

# Create a clean mirrored clone of EicRoot locally
git clone --mirror https://git.racf.bnl.gov/gitea/EIC/EicRoot EicRootScratch

# Make all changes to the mirrored local scratch repository first
cd EicRootScratch

# Remove DipoleMap3.dat from the entire history, including updating all branches and tags
git filter-branch --force --index-filter "git rm --cached --ignore-unmatch input/DipoleMap3.dat" --prune-empty --tag-name-filter cat -- --all

# Verify that locally, DipoleMap3 file is gone everywhere and your branches and tags still make sense

# Push the modified mirror to your new empty github repository
git push --mirror https://github.com/eic/EicRoot.git

# ONLY DO THIS LAST STEP AFTER YOU VERIFY THAT ALL BRANCHES AND TAGS MAKE SENSE ON THE NEW GITHUB REPO
# Force push the mirrored repository back to the origin repository at BNL, so that the old and new repositories are identical
git push --mirror --force
