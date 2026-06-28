$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot
$presets = Get-Content (Join-Path $repoRoot 'CMakeUserPresets.json') | ConvertFrom-Json
$binaryDir = $presets.configurePresets[0].binaryDir -replace '\$\{sourceDir\}', $repoRoot
if (Test-Path $binaryDir) {
    Remove-Item -Recurse -Force $binaryDir
    Write-Host "Cleaned: $binaryDir"
} else {
    Write-Host "Nothing to clean: $binaryDir"
}
