. .\create-folder.ps1
. .\start-developer-command-prompt.ps1

Start-Developer-Command-Prompt
$rootFolder = (Get-Item -Path ".\" -Verbose).FullName + "\"

# Create folder if it does not exist:
$folderName = "assimp"
Create-Folder -Path $folderName

# Generate cmake files inside the folder:
Set-Location $folderName
cmake ../../assimp -G"Visual Studio 15 2017 Win64" -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=FALSE -DASSIMP_BUILD_FBX_IMPORTER=TRUE -DASSIMP_BUILD_GLTF_IMPORTER=TRUE

# Build:
devenv Assimp.sln /build Debug /project assimp
devenv Assimp.sln /build Release /project assimp

# Clean up:
Get-ChildItem -Hidden | Remove-Item -Recurse -Force
Get-ChildItem | Where-Object {($_ -notlike "code")} | Remove-Item -Recurse -Force
Set-Location "code"
Get-ChildItem | Where-Object {($_ -notlike "Debug" -and $_ -notlike "Release")} | Remove-Item -Recurse -Force

# Return to the original folder:
Set-Location $rootFolder