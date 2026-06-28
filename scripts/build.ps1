$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot
$presets = Get-Content (Join-Path $repoRoot 'CMakeUserPresets.json') | ConvertFrom-Json
$preset = if ($presets.buildPresets) { $presets.buildPresets[0].name } else { $presets.configurePresets[0].name }
Set-Location $repoRoot
cmake --build --preset $preset
