$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot
$presets = Get-Content (Join-Path $repoRoot 'CMakeUserPresets.json') | ConvertFrom-Json
$configPreset = $presets.configurePresets[0]
$binaryDir = $configPreset.binaryDir -replace '\$\{sourceDir\}', $repoRoot
$exe = if ($IsWindows) { Join-Path $binaryDir 'artificial_will.exe' } else { Join-Path $binaryDir 'artificial_will' }
& $exe
