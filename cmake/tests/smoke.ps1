param (
    [Parameter(Mandatory=$true)][string]$Bin,
    [Parameter(Mandatory=$true)][string]$Src
)

$fail = 0

function check {
    param([string]$desc, [int]$result)
    if ($result -eq 0) {
        Write-Host "  PASS: $desc"
    } else {
        Write-Host "  FAIL: $desc"
        $script:fail = 1
    }
}

Write-Host "=== timestamp smoke tests ==="
Write-Host "Binary:  $Bin"
Write-Host "Source:  $Src"
Write-Host ""

# 1. No args — exits 0, non-empty stdout
Write-Host "1. Default run"
$out = & $Bin 2>$null
check "exits 0" $LASTEXITCODE
check "produces output" $(if ([string]::IsNullOrEmpty($out)) { 1 } else { 0 })

# 2. --help — exits 0
Write-Host "2. --help"
& $Bin --help 2>$null | Out-Null
check "exits 0" $LASTEXITCODE

# 3. Nonexistent dir — exits non-zero
Write-Host "3. Nonexistent directory"
& $Bin /nonexistent_path_xyz_123 2>$null | Out-Null
check "exits non-zero" $(if ($LASTEXITCODE -ne 0) { 0 } else { 1 })

# 4. Verbose — stderr contains filenames
Write-Host "4. Verbose"
$out = & $Bin -v "$Src/src/" 2>&1
check "exits 0" $LASTEXITCODE
check "shows filenames on stderr" $(if (($out | Out-String) -match "timestamp.cpp") { 0 } else { 1 })

# 5. Exclude pattern — changes output vs no-exclude
Write-Host "5. Exclude"
$normal = & $Bin "$Src/src/" 2>$null
$filtered = & $Bin -e '\.cpp' "$Src/src/" 2>$null
check "exclude changes output" $(if ($normal -ne $filtered) { 0 } else { 1 })

Write-Host ""
exit $fail
