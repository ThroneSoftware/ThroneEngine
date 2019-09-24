<!--- This is probably incomplete, we should improve it iterativly--> 
## Adding a library
Add a folder with the name of your library

# Open source library
You should add a CMakeLists to the library project and add it to the Targets.cmake

# Not Open source library
You should add the library as an external library in cmake.

## Modifying a vendor's source code
There should be a file called REVISIONS that describe changes made to the code. 
Having a commit sha1 ID and a PR number is also necessary. 