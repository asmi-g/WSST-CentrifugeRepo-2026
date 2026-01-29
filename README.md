# WSST-CentrifugeRepo-2026

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