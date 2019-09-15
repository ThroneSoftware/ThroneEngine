from pathlib import Path
import os

# Change the directory to be the same as this file
os.chdir(os.path.dirname(__file__))

# Recursivly find all files with extension .h, .hpp and .cpp
files = list(Path('.').rglob("*.h"))
files.extend(Path('.').rglob("*.hpp"))
files.extend(Path('.').rglob("*.cpp"))

# Sort files by folders instead of extension
files.sort()

# Open the FileList.cmake file, creates it if not present
fileList = open("FileList.cmake","w+")

# Write all source files in the FileList.cmake
fileList.write("target_sources(ManualTesting PRIVATE \n")

for file in files:
    fileList.writelines("\t" + file.as_posix() + "\n")

fileList.write(")")