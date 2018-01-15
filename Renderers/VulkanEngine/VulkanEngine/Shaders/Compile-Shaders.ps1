param(
    [String]$DestinationPath, 
    [String]$ShadersPath
)

function Get-Shaders($Extension) 
{
    return (Get-ChildItem | Where-Object {($_ -like "*." + $Extension)})
}

function Copy-Shaders($TargetPath)
{
    If(!(test-path $TargetPath))
    {
	    New-Item -ItemType Directory -Force -Path $TargetPath
    }

    Copy-Item -Path "./*.spv" -Destination $TargetPath -Force -Recurse 
}

function Compile-Shaders($TargetPath)
{
    $vulkanPath = (Get-Item env:"VULKAN_SDK").Value + "\"
    $vulkanBinPath = $vulkanPath + "Bin\"
    $glslValidator = $vulkanBinPath + "glslangValidator.exe"

    $shaders = @{}
    $shaders.vertex = Get-Shaders("vert")
    $shaders.fragment = Get-Shaders("frag")

    $shaders.Values | ForEach-Object { & $glslValidator -V $_.Name }

    Copy-Shaders($TargetPath)
}

Write-Output $ShadersPath
Set-Location $ShadersPath
Compile-Shaders($DestinationPath)
