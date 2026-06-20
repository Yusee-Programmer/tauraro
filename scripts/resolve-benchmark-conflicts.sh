#!/bin/bash
# Helper script to resolve benchmarks/results.md conflicts
# Usage: bash scripts/resolve-benchmark-conflicts.sh [remote-branch]
# Example: bash scripts/resolve-benchmark-conflicts.sh origin/master

REMOTE_BRANCH="${1:-origin/master}"

echo "Resolving conflicts with $REMOTE_BRANCH using 'ours' strategy..."
echo "This will keep your version of benchmarks/results.md"
echo ""

# Attempt rebase with -X ours strategy
if git rebase -X ours "$REMOTE_BRANCH"; then
    echo ""
    echo "✅ Conflicts resolved successfully!"
    echo ""
    echo "Next steps:"
    echo "  1. Review the changes: git log -5"
    echo "  2. Push: git push origin master --force-with-lease"
else
    echo ""
    echo "❌ Rebase failed with -X ours strategy"
    echo "Manually resolve remaining conflicts and run:"
    echo "  git rebase --continue"
    exit 1
fi
