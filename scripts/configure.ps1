$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot
$preset = (Get-Content (Join-Path $repoRoot 'CMakeUserPresets.json') | ConvertFrom-Json).configurePresets[0].name
Set-Location $repoRoot
cmake --preset $preset
