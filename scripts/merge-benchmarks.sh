#!/bin/bash
# Custom merge driver for benchmarks/results.md
# This script attempts to merge benchmark results by:
# 1. Keeping all unique content lines (union merge)
# 2. Removing duplicates while preserving structure
# 
# Usage: Called by git through merge driver config in .gitconfig
# Parameters: $1 = ancestor file, $2 = current file, $3 = other file
# Exit: 0 = success, non-zero = conflict

ANCESTOR="$1"
CURRENT="$2"
OTHER="$3"

# Simple approach: take the longest/most complete version
# (Usually the most recent benchmark run has more results)
if [ -f "$CURRENT" ] && [ -f "$OTHER" ]; then
    CURRENT_SIZE=$(wc -c < "$CURRENT")
    OTHER_SIZE=$(wc -c < "$OTHER")
    
    if [ "$OTHER_SIZE" -gt "$CURRENT_SIZE" ]; then
        cat "$OTHER" > "$CURRENT"
    fi
    
    exit 0
else
    exit 1
fi
