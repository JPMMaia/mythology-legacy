. .\create-folder.ps1
. .\start-developer-command-prompt.ps1

Start-Developer-Command-Prompt
$rootFolder = (Get-Item -Path ".\" -Verbose).FullName + "\"

# Create folder if it does not exist:
$folderName = "glfw"
Create-Folder -Path $folderName

# Generate cmake files inside the folder:
Set-Location $folderName
cmake ../../glfw -G"Visual Studio 15 2017 Win64"

# Build:
devenv GLFW.sln /build Debug
devenv GLFW.sln /build Release

# Clean up:
Get-ChildItem -Hidden | Remove-Item -Recurse -Force
Get-ChildItem | Where-Object {($_ -notlike "src")} | Remove-Item -Recurse -Force
Set-Location "src"
Get-ChildItem | Where-Object {($_ -notlike "Debug" -and $_ -notlike "Release")} | Remove-Item -Recurse -Force
Set-Location "../"
Rename-Item "src" "x64"

# Return to the original folder:
Set-Location $rootFolder