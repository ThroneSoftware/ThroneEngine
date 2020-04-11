# Required programs
1. Git
2. Git lfs
3. Cmake version 3.16 or higher
4. A c++ build system supported by cmake (Only visual studio is supported currently)

# Commands
All the commands should be executed in this order.
##### Cloning the repo
```
git clone https://github.com/ThroneSoftware/ThroneEngine.git
cd ThroneEngine
git lfs fetch
git lfs pull
```
##### Generating the files for the build system
```
cmake -S "." -B "Build/"
```
##### Building Throne and the tests
```
cmake --build "Build/" -j
```
