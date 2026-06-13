# WSST-CentrifugeRepo-2026

## Git Version Control
**REQUIRED**: Before doing any development in this repository, read through and follow the Git best practices described in 
```/docs/gitBestPractices.md```

## System Overview
Learn more about the Space Soldering software overview, located in
```/docs/systemOverview.md```

## Install
Install the following applications:
* [STM32CubeMX - Ver 6.15.0](https://www.st.com/en/development-tools/stm32cubemx.html)
* [CMake - Ver 4.1.1](https://cmake.org/download/)
* [GNU ARM Embedded Toolchain - Ver 14.3, release 1](https://developer.arm.com/downloads/-/gnu-rm)
    * NOTE: Versions older than GCC 11 will present an issue while building/flashing STM32 code
* [OpenOCD - Ver 20251211-0.12.0](https://gnutoolchains.com/arm-eabi/openocd/)
* [MSYS2 - 64 Bit for Windows](https://www.msys2.org/)
* [Putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/) - not useful immediately but will use at some point

Setup the following tools and add to your device's PATH environment variables. 
* CMake
    * System Variables, PATH: ``C:\Program Files\CMake\bin``
* GNU ARM Embedded Toolchain
    * User Variables, PATH: ``C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\14.3 rel1\bin`` for example
* OpenOCD
    * System Variables, PATH: ``C:\Users\asmig\OpenOCD-20251211-0.12.0\bin`` for example
    * User Variables, PATH: ``C:\Users\asmig\OpenOCD-20251211-0.12.0\bin\openocd.exe`` for example
* MSYS2
    * User Variables, PATH: ``C:\msys64\usr\bin`` for example

If you've never done this before, I recommend checking out a section from [this tutorial](https://mcuoneclipse.com/2021/05/01/visual-studio-code-for-c-c-with-arm-cortex-m-part-1/), particulary for CMake and GNU Arm Embedded Toolchain. Do not forget to do something similar for OpenOCD as well (it might ask you if you want to add to PATH in the setup GUI). If not, do the following:
* Find the path where you installed OpenOCD's executable, something like: ..\OpenOCD-20251211-0.12.0\bin\openocd.exe
* Add it to your PATH environment variable

After your setup, confirm the following commands are recognized in your bash terminal (preferably in VS Code):
```
arm-none-eabi-gcc --version
cmake --version
openocd --version
```

## Install Make
After installing MSYS2 (MSYS2-64 bit if on Windows), open the MINGW64 version (search up MSYS2 and open up MINGW64 labelled command prompt) and install make with the following command:
```sh
pacman -S make
```
Check its installation in both command prompts (git bash and MSYS2) with the following:
```sh
which make
```

## Install VS Code Extentions
Install the following:
* CMake Tools by Microsoft
* C/C++ by Microsoft
* CMake by twxs
* Cortex-Debug by marus25
* STM32CubeIDE for Visual Studio Code

## Customize (.bashrc, .env)
### .bashrc
Navigate to ``app/init`` and make a copy of the file called .bashrc-template. Name it ``.bashrc``. Go to where your environment variables under PATH are located (on Windows, enter the search prompt, type in "Edit the system environment variables", navigate to Environment Variables>User Variables>Path, then copy the path to your gnu toolchain binary folder. Example: ``C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\14.3 rel1\bin``).

Paste the copied path from above in the environment variable in .bashrc, labelled ``ARM_TOOLCHAIN``. Check that the location of the CC (gnu gcc compiler) and CXX (gnu g++ compiler) also exists. To do so, run the following:
```sh
"$CC" --version
"$CXX" --version
```

### .env
Navigate to ``app/init`` and make a copy of the file called .env.template. Name it ``.env``. Go to where your environment variables under PATH are located (on Windows, enter the search prompt, type in "Edit the system environment variables", navigate to Environment Variables>User Variables>Path, then copy the path to your gnu toolchain binary folder. Example: ``C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/14.3 rel1/bin``). Paste the copied path from above in the environment variable in .env, labelled ``ARM_TOOLCHAIN``. 

Similarly, within the User Variables>Path window from above, copy the path to your openocd executable. Example: ``C:/Users/asmig/OpenOCD-20251211-0.12.0/bin/openocd.exe``. Paste the copied path from above in the environment variable in .env, labelled ``OPENOCD_PATH``. 

Close the VS Code window and re-open it just to be safe.

### Paths
Make sure that the slashes in any of the paths above, are forward-slashes, or right-hand facing. We do not want backward slashes. This means, replace any slashes in the paths with `/` and **NOT** `\`.

## Initialize
Run the environment variable initialization:
````sh
cd app/init
source .bashrc
````

## Configure the Build
Run the following command in the /app/ directory, and REMEMBER TO CHANGE THE BOARD PARAMETER TO MATCH WHICH BOARD YOU'RE WORKING WITH:

```sh
cd ..
cmake -S . -B build -G "Unix Makefiles"   -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" -DCMAKE_C_COMPILER=CC   -DCMAKE_CXX_COMPILER=CXX   -DBOARD=F401RE
```

## Build
Run the following from the /app/ directory.
```sh
cmake --build build/
```

## Flash
To flash your code, connect your board. Then, start a debug session. In the top left of the VS Code Window, navigate to Run>Start Debugging.


## Clean Build
To clean a build, run the following from the app directory:
```sh
rm -rf build/
```