<!--- This is probably incomplete, we should improve it iterativly--> 
# Adding a library
Add a folder with the name of your library. Libs should be built. You should also include a BUILD_README.md in the folder that describes instructions to build 
(even if they are simple). Providing a link to where the instructions were found can also be helpful. 

Vendors includes are accessed by the <vendor/> path. If a lib must be accessed by a more direct path then it should also be added to the 
.clang-format file in the include categories. There is already boost and glm so you can copy what is already there.

## Open source library
You should add a CMakeLists to the library project and add it to the Targets.cmake

## Not Open source library
You should add the library as an external library in cmake.

# Modifying a vendor's source code
There should be a file called REVISIONS.md that describe changes made to the code. 
Having an issue number is also necessary. You should not modify a vendor inside an issue related to something else.
All modifications to a vendor must have a unique issue and PR associated with it. Obviously many changes can be made to a
vendor in one issue, but it cannot be inside a PR with changes made to the Throne code.
