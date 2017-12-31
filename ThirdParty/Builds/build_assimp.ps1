# Create folder if it does not exist:
$folderName = "assimp"
If(!(test-path $folderName))
{
	New-Item -ItemType Directory -Force -Path $folderName
}

# Generate cmake files inside the folder:
cd "assimp"
cmake ../../assimp -G"Visual Studio 15 2017 Win64" -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=FALSE -DASSIMP_BUILD_FBX_IMPORTER=TRUE -DASSIMP_BUILD_GLTF_IMPORTER=TRUE

# Start Developer Command Prompt:
$installationPath = ../vswhere.exe -prerelease -latest -property installationPath
if ($installationPath -and (test-path "$installationPath\Common7\Tools\vsdevcmd.bat")) {
  & "${env:COMSPEC}" /s /c "`"$installationPath\Common7\Tools\vsdevcmd.bat`" -no_logo && set" | foreach-object {
    $name, $value = $_ -split '=', 2
    set-content env:\"$name" $value
  }
}

# Build:
devenv Assimp.sln /build Debug /project assimp
devenv Assimp.sln /build Release /project assimp

# Return to the original folder:
cd ../