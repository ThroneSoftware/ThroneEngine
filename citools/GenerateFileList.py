# Will generate the FileLists.cmake files for each TARGET

import os
from pathlib import Path

# Scan the folder to find every .h, .hpp or .cpp files to append them to the file list
# relativeTo is the folder path where the TARGET is
def scanFolder(folderPath, file, relativeTo):
    # Scan every files and directories in the folderPath
    for sourceFile in os.scandir(folderPath):
        # Check if it's a source code file
        if sourceFile.name.endswith((".h", ".hpp", ".cpp")):
            # Convert the sourceFile.path to be relative to relativeTo 
            # and convert it in posix (cmake requires posix style in target_source)
            sourcePath = Path(sourceFile.path).relative_to(relativeTo).as_posix()
            file.writelines(f"\t{sourcePath}\n")
        # The sourceFile is a directory
        elif sourceFile.is_dir():
            if Path(os.path.abspath(f"{sourceFile.path}/TARGET")).exists():
                # TARGET exists in the directory so we need to start another sequence
                generateFileListRecursive(sourceFile.path)
            else:
                # Continue to scan subdirectories for more source code files
                scanFolder(sourceFile.path, file, relativeTo)

# Populate the FileList.cmake if the file TARGET is present in the targetPath folder
def generateFileListRecursive(targetPath, targetsList):
    # Change the directory to targetPath
    os.chdir(targetPath)

    if Path('./TARGET').exists():
        # Open the FileList.cmake file, creates it if not present
        file = open("FileList.cmake","w+")
        
        # Trim the folder name from the path
        # The folder name must also be the target name
        # Example : c:/Throne/src/Core to Core
        folder_name = os.path.basename(os.path.normpath(targetPath))

        file.write(f"target_sources({folder_name} PRIVATE \n")

        scanFolder(targetPath, file, targetPath)

        file.write(")")

        targetsList.append(targetPath)
    else:
        # The TARGET was not found so we want to continue searching subdirectories
        generateFileList(targetPath, targetsList)

# Explore all folders in the current directory
def generateFileList(parentDirectoryPath, targetsList):
    # Find each directory in the parentDirectoryPath
    for sourceFile in os.scandir(parentDirectoryPath):
        # The sourceFile is a directory
        if sourceFile.is_dir():
            generateFileListRecursive(sourceFile.path, targetsList)

# Change the order of targets since some targets need to come before others.
def writeTargetsFile(targetsList, srcPath):
    os.chdir(srcPath)

    pchTarget = next(target for target in targetsList if "Pch" in target)

    # Pch must be at the beggining
    targetsList.remove(pchTarget)
    targetsList.insert(0, pchTarget)

    with open("Targets.cmake","w+") as targetsFile:
        for target in targetsList:
            targetsFile.writelines(f"add_subdirectory({Path(target).relative_to(Path(srcPath).parent).as_posix()})\n")



def main():
    # Change the directory to src
    srcPath = os.path.dirname(__file__) + "/../src"
    srcPath = os.path.abspath(srcPath)
    os.chdir(srcPath)

    targetsList = []
    generateFileList(srcPath, targetsList)

    writeTargetsFile(targetsList, srcPath)

if __name__ == "__main__":
    main()