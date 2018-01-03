function Create-Folder ($Path) {
    If(!(test-path $Path))
    {
	    New-Item -ItemType Directory -Force -Path $Path
    }
}
