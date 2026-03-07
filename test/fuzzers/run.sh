#!/bin/sh
# Build and run a fuzzer by name.
#
# Usage:
#   ./test/fuzzers/run.sh html              # build & run html fuzzer
#   ./test/fuzzers/run.sh ast 300           # run for 300 seconds
#   ./test/fuzzers/run.sh heal 60           # run heal fuzzer for 60s
#
# Corpus is stored in test/fuzzers/corpus/<name>/ (gitignored).
# Seed corpus from test/fuzzers/seed-corpus/ is used as read-only seed.

set -e

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
FUZZ_OUT="${FUZZ_OUT_DIR:-$ROOT/fuzz-out}"
CORPUS_DIR="$ROOT/test/fuzzers/corpus"
SEED_DIR="$ROOT/test/fuzzers/seed-corpus"

TARGET="${1:?Usage: $0 <html|ast|ansi|text|meta|heal> [max_total_time]}"
MAX_TIME="${2:-60}"

case "$TARGET" in
    html|ast|ansi|text|meta|heal) ;;
    *) echo "Unknown target: $TARGET (expected: html, ast, ansi, text, meta, heal)" >&2; exit 1 ;;
esac

BINARY="$FUZZ_OUT/fuzz-md$TARGET"

# Build the fuzzer
"$ROOT/test/fuzzers/build.sh" "$TARGET"

# Create corpus directory
mkdir -p "$CORPUS_DIR/$TARGET"

echo "Running fuzz-md$TARGET for ${MAX_TIME}s..."
"$BINARY" "$CORPUS_DIR/$TARGET" "$SEED_DIR" -max_total_time="$MAX_TIME"
