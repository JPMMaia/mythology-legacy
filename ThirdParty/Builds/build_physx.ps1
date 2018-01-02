# Cache the root folder:
$rootFolder = (Get-Item -Path ".\" -Verbose).FullName

# Create libraries folder:
$librariesDestinationFolder = $rootFolder + "\PhysX\Libaries\x64"
If(!(test-path $librariesDestinationFolder))
{
	New-Item -ItemType Directory -Force -Path $librariesDestinationFolder
}

# Create binaries folder:
$binariesDestinationFolder = $rootFolder + "\PhysX\Binaries\x64"
If(!(test-path $binariesDestinationFolder))
{
	New-Item -ItemType Directory -Force -Path $binariesDestinationFolder
}

# Chache the PhysX root folder:
$physXRootFolder = $rootFolder + "\..\PhysX"

# Start Developer Command Prompt:
$installationPath = ./vswhere.exe -prerelease -latest -property installationPath
if ($installationPath -and (test-path "$installationPath\Common7\Tools\vsdevcmd.bat")) {
  & "${env:COMSPEC}" /s /c "`"$installationPath\Common7\Tools\vsdevcmd.bat`" -no_logo && set" | foreach-object {
    $name, $value = $_ -split '=', 2
    set-content env:\"$name" $value
  }
}

# Copy PxShared projects:
$pxSharedCompilerFolder = "..\PhysX\PxShared\src\compiler\vc14win64"
Copy-Item -Path "PhysX\PxShared\*" -Destination $pxSharedCompilerFolder -Recurse -Force

# Copy PhysX projects:
$physXCompilerFolder = "..\PhysX\PhysX_3.4\Source\compiler\vc14win64"
Copy-Item -Path "PhysX\PhysX\*" -Destination $physXCompilerFolder -Recurse -Force

# Build everything:
Set-Location $physXCompilerFolder
devenv PhysX.sln /build checked /project PhysX
devenv PhysX.sln /build release /project PhysX

# Copy libraries:
Set-Location $physXRootFolder
Copy-Item -Path "PhysX_3.4\Lib\vc14win64\*" -Destination $librariesDestinationFolder -Recurse -Force
Copy-Item -Path "PxShared\lib\vc14win64\*" -Destination $librariesDestinationFolder -Recurse -Force

# Copy binaries:
Copy-Item -Path "PhysX_3.4\Bin\vc14win64\*" -Destination $binariesDestinationFolder -Recurse -Force
Copy-Item -Path "PxShared\bin\vc14win64\*" -Destination $binariesDestinationFolder -Recurse -Force

# Clean up everything:
Set-Location $physXRootFolder
git clean -fx -d
git reset --hard

# Return to the root folder:
Set-Location $rootFolder