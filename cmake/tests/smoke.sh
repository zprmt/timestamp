#!/usr/bin/env bash
set -uo pipefail

BIN="$1"
SRC="$2"
fail=0

check() {
    local desc="$1" result="$2"
    if [ "$result" = 0 ]; then
        echo "  PASS: $desc"
    else
        echo "  FAIL: $desc"
        fail=1
    fi
}

echo "=== timestamp smoke tests ==="
echo "Binary:  $BIN"
echo "Source:  $SRC"
echo ""

# 1. No args — exits 0, non-empty stdout
echo "1. Default run"
out=$("$BIN" 2>/dev/null) && rc=0 || rc=$?
check "exits 0" "$rc"
check "produces output" "$([ -n "$out" ] && echo 0 || echo 1)"

# 2. --help — exits 0
echo "2. --help"
"$BIN" --help >/dev/null 2>&1
check "exits 0" $?

# 3. Nonexistent dir — exits non-zero
echo "3. Nonexistent directory"
"$BIN" /nonexistent_path_xyz_123 >/dev/null 2>&1
check "exits non-zero" $(( ! $? ))

# 4. Verbose — stderr contains filenames
echo "4. Verbose"
out=$("$BIN" -v "$SRC/src/" 2>&1) && rc=0 || rc=$?
check "exits 0" "$rc"
check "shows filenames on stderr" $(echo "$out" | grep -q "timestamp.cpp" && echo 0 || echo 1)

# 5. Exclude pattern — changes output vs no-exclude
echo "5. Exclude"
normal=$("$BIN" "$SRC/src/" 2>/dev/null)
filtered=$("$BIN" -e '\.cpp' "$SRC/src/" 2>/dev/null)
check "exclude changes output" $([ "$normal" != "$filtered" ] && echo 0 || echo 1)

echo ""
exit $fail
