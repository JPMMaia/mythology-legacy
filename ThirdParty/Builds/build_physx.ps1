. .\create-folder.ps1
. .\start-developer-command-prompt.ps1

Start-Developer-Command-Prompt

function Create-Folders ($Paths) {
  $Paths | ForEach-Object { Create-Folder -Path $_ }
}

function Move-Debug-Items ($Source, $Destination) {
  Get-ChildItem -Recurse -Path $Source | Where-Object {($_ -like '*DEBUG*')} | Move-Item -Destination $Destination
}

function Move-Release-Items ($Source, $Destination) {
  Get-ChildItem -Recurse -Path $Source | Where-Object {($_ -notlike '*CHECKED*' -and $_ -notlike '*DEBUG*' -and $_ -notlike '*PROFILE*')} | Move-Item -Destination $Destination
}

# Cache key folders:
$rootFolder = (Get-Item -Path ".\" -Verbose).FullName + "\"
$physXRootFolder = $rootFolder + "\..\PhysX\"
$x64Folder = "x64\"
$debugFolder = "Debug\"
$releaseFolder = "Release\"

# Create libraries folders:
$librariesDestinationFolders = @{}
$librariesDestinationFolders.root = $rootFolder + "PhysX\Libraries\"
$librariesDestinationFolders.x64 = $librariesDestinationFolders.root + $x64Folder
$librariesDestinationFolders.x64Debug = $librariesDestinationFolders.x64 + $debugFolder
$librariesDestinationFolders.x64Release = $librariesDestinationFolders.x64 + $releaseFolder
Create-Folders -Paths $librariesDestinationFolders.Values

# Create binaries folders:
$binariesDestinationFolders = @{}
$binariesDestinationFolders.root = $rootFolder + "PhysX\Binaries\"
$binariesDestinationFolders.x64 = $binariesDestinationFolders.root + $x64Folder
$binariesDestinationFolders.x64Debug = $binariesDestinationFolders.x64 + $debugFolder
$binariesDestinationFolders.x64Release = $binariesDestinationFolders.x64 + $releaseFolder
Create-Folders -Paths $binariesDestinationFolders.Values

# Copy PxShared projects:
$pxSharedCompilerFolder = "..\PhysX\PxShared\src\compiler\vc14win64\"
Copy-Item -Path "PhysX\PxShared\*" -Destination $pxSharedCompilerFolder -Recurse -Force

# Copy PhysX projects:
$physXCompilerFolder = "..\PhysX\PhysX_3.4\Source\compiler\vc14win64\"
Copy-Item -Path "PhysX\PhysX\*" -Destination $physXCompilerFolder -Recurse -Force

# Build everything:
Set-Location $physXCompilerFolder
devenv PhysX.sln /build debug /project PhysX
devenv PhysX.sln /build debug /project PhysXExtensions
devenv PhysX.sln /build release /project PhysX
devenv PhysX.sln /build release /project PhysXExtensions

# Copy libraries:
Set-Location $physXRootFolder
$physXLibrariesFolder = "PhysX_3.4\Lib\vc14win64\"
$pxSharedLibrariesFolder = "PxShared\Lib\vc14win64\"
Move-Debug-Items -Source $physXLibrariesFolder -Destination $librariesDestinationFolders.x64Debug
Move-Debug-Items -Source $pxSharedLibrariesFolder -Destination $librariesDestinationFolders.x64Debug
Move-Release-Items -Source $physXLibrariesFolder -Destination $librariesDestinationFolders.x64Release
Move-Release-Items -Source $pxSharedLibrariesFolder -Destination $librariesDestinationFolders.x64Release

# Copy binaries:
$physXBinariesFolder = "PhysX_3.4\Bin\vc14win64\"
$pxSharedBinariesFolder = "PxShared\bin\vc14win64\"
Move-Debug-Items -Source $physXBinariesFolder -Destination $binariesDestinationFolders.x64Debug
Move-Debug-Items -Source $pxSharedBinariesFolder -Destination $binariesDestinationFolders.x64Debug
Move-Release-Items -Source $physXBinariesFolder -Destination $binariesDestinationFolders.x64Release
Move-Release-Items -Source $pxSharedBinariesFolder -Destination $binariesDestinationFolders.x64Release

# Clean up everything:
Set-Location $physXRootFolder
git clean -fx -d
git reset --hard

# Return to the root folder:
Set-Location $rootFolder