# WSST-CentrifugeRepo-2026

## Git Best Practices
Read through Git Best Practices, [linked Here](https://nvie.com/posts/a-successful-git-branching-model/#the-main-branches).

### MAIN IDEA:
* Master/Main branch:
    * Main, FUNCTIONAL source code. I.e, code in main is assumed to be in a working state at ALL times
* Develop branch:
    * This is the branch where the source code of HEAD always reflects a state with the latest delivered development changes for the next release. When the source code in the develop branch reaches a stable point and is ready to be released, all of the changes should be merged back into master. 
* Feature branches:
    * Branch OFF of develop
    * When functional, merge back INTO develop

Thus, at a high level, the development workflow looks like:
1. Start feature: main > develop > feature 
    * Implement changes into a copy of the "develop" branch's source code, which is a copy of the "main" branch's source code
2. Merge feature: main < develop < feature 
    * When done, merge changes into the "develop" branch's source code, then into "main"

### Commit Messages

Make sure your commit messages follow the following template:
"branch_name": "overview of commit functionality"
ex. 
```
develop: Merged feature into develop branch
```

**ALWAYS REMEMBER TO MAKE A MERGE REQUEST INTO BOTH, THE DEVELOP AND MAIN BRANCH**

### Checklist Before Pushing
Ask yourself:
1. Am I pushing my code to a FEATURE branch, and not directly to develop or main?
2. Does my commit message follow the established convention?
3. If my feature works, have I rebased it with the develop branch before making a merge request into develop?
4. If my develop branch works, have I rebased it with main before making a merge request into main?
5. If I've implemented any code which has changed how a user might use it, have I added the adequate documentation to do so?
6. Is the repo structure more or less the same (i.e, clean) after my changes?

## Install
Install the following applications:
* [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
* [CMake](https://cmake.org/download/)
* [GNU ARM Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)
* [OpenOCD](https://openocd.org/)
* [Putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/) - not useful immediately but will use at some point

Setup the following tools and add to your device's PATH environment variables. 
* CMake
* GNU ARM Embedded Toolchain
* OpenOCD

If you've never done this before, I recommend checking out a section from [this tutorial](https://mcuoneclipse.com/2021/05/01/visual-studio-code-for-c-c-with-arm-cortex-m-part-1/), particulary for CMake and GNU Arm Embedded Toolchain. Do not forget to do something similar for OpenOCD as well (it might ask you if you want to add to PATH in the setup GUI). If not, do the following:
* Find the path where you installed OpenOCD's executable, something like: ..\OpenOCD-20251211-0.12.0\bin\openocd.exe
* Add it to your PATH environment variable

After your setup, confirm the following commands are recognized in your bash terminal (preferably in VS Code):
```
arm-none-eabi-gcc --version
cmake --version
openocd --version
```

## Initialize
Run the environment variable initialization. BEFORE YOU DO, check the paths in the bash script to see if they match where you installed your Arm GNU Toolchain components. IF NOT, change them to match, and add your own script to /app/init for your own environment. DO NOT modify the existing .bashrc. Call it something like .bashrc-asmi, and replace your own name:
````
cd app/init
source .bashrc
````

## Configure the Build
Run the following command in the /app/ directory, and REMEMBER TO CHANGE THE BOARD PARAMETER TO MATCH WHICH BOARD YOU'RE WORKING WITH:
```
cmake -S . -B build -G "Unix Makefiles"   -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake"   -DCMAKE_C_COMPILER="C:/Program Files (x86)/GNU Arm Embedded Toolchain/14.3 2023.07/bin/arm-none-eabi-gcc.exe"   -DCMAKE_CXX_COMPILER="C:/Program Files (x86)/GNU Arm Embedded Toolchain/14.3 2023.07/bin/arm-none-eabi-g++.exe"   -DBOARD=F411RE
```

## Build
Run the following from the /app/ directory.
```
cmake --build build/
```

## Flash
To flash your code, connect your board. Then, start a debug session. In the top left of the VS Code Window, navigate to Run>Start Debugging.


## Clean Build
To clean a build, run the following from the app directory:
```
rm -rf build/
```