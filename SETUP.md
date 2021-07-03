# Required dependencies
1. Git
2. Git lfs
3. Cmake version 3.16 or higher
4. vkpkg
5. LunarG SDK
6. A c++ compiler supported by cmake (Throne currently only support MSVC, but we want to support Linux in the future)

# Commands
All the steps should be executed in this order.

#### Installing external dependencies

##### Installing vcpkg
You can install vcpkg [here](https://github.com/microsoft/vcpkg).  
By default, Throne will check for C:\vcpkg\ (on windows), but it is possible to change the path with the cmake variable VCPKG_ROOT_FOLDER.  

You don't have to install any packages since it is done with Cmake.

##### Installing the LunarG SDK
You can install the LunarG SDK [here](https://vulkan.lunarg.com/).  
We carry the headers in the repository and linking is done dynamically at runtime so the SDK install path does not really matter.

#### Cloning the repo
```
git clone https://github.com/ThroneSoftware/ThroneEngine.git
cd ThroneEngine
git lfs fetch
git lfs pull
```
#### Generating the files for the build system
```
cmake -S "." -B "Build/"
```
#### Building Throne and the tests
```
cmake --build "Build/" -j
```
