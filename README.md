# WSST-CentrifugeRepo-2026

## Git Version Control
**REQUIRED**: Before doing any development in this repository, read through and follow the Git best practices described in 
```/docs/gitBestPractices.md```

## System Overview
Learn more about the Space Soldering software overview, located in
```/docs/systemOverview.md```

## CubeIDE Alternative Setup
Install the following applications:
* [STM32CubeMX - Ver 6.15.0](https://www.st.com/en/development-tools/stm32cubemx.html)
* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

Start up STM32CubeIDE. Navigate to:
File > STM32 Project Create/Import > Import STM32 Project > STM32CubeMX/STM32CubeIDE Project.

Within Import Source field, navigate to where you cloned this repo, into the following folder:
```WSST-CentrifugeRepo-2026/ide_alternative```

Once imported, navigate to:
Project > Build Project

After its successfully built, flash your STM32 board by hitting "Run" in the IDE.

When your board is flashed, open up the GUI. To do so, open a Git Bash terminal. Navigate to where you cloned this repo, into the following folder:
```WSST-CentrifugeRepo-2026/gui/```

Make sure the COM port defined in the gui.py script matches where your STM32 is connected to (check Device Manager on windows for confirmation). Then in your git bash terminal, run the python script with:
```sh
python gui.py
```

## Dev Environment Setup - Install
Install the following applications:
* [STM32CubeMX - Ver 6.15.0](https://www.st.com/en/development-tools/stm32cubemx.html)
* [CMake - Ver 4.1.1](https://cmake.org/download/)
* [GNU ARM Embedded Toolchain - Ver 14.3, release 1](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads#panel2a)
    * NOTE: Versions older than GCC 11 will present an issue while building/flashing STM32 code. Select arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-arm-none-eabi.exe.
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

## Synching app/ and ide_alternative/
### app/ and ide_alternative/ file mapping

```text
├── app
├── ide_alternative
└── README.md


└── app
   ├── boards/
   │   └── F401RE/
   │       ├── Core/   
   |       |   ├──  Inc/ 
   |       |   |    └── main.h
   │       |   └──  Src/     
   |       |        └── main.c   
   │       └── F401RE.ioc       
   └── drivers/ 
        ├── inc/ 
        |    └── all custom driver header files     
        └── src/     
             └── all custom driver source files    
└── ide_alternative
    ├── Core/               
    |    ├── Inc/ 
    |    |    ├── main.h  
    |    |    └── all custom driver header files           
    |    └── Src/ 
    |         ├── main.c   
    |         └── all custom driver source files  
    └── ide_alternative.ioc     

```


### Changes made in app/ first
#### If Changes Are Made to a .ioc file in app/
1. Delete the contents of ide_alternative/
2. Copy over the .ioc file from app/boards/F401RE into ide_alternative/
3. Rename the .ioc file from F401RE.ioc to ide_alternative.ioc
4. Open the renamed .ioc file in STM32 CubeMX, navigate to Project Manager
5. Change Toolchain/IDE dropdown setting to STM32CubeIDE
6. Check off the "Generate Under Root" box
7. Click the Generate Code button, wait for code to generate in ide_alternative/ folder
8. After the code is generated, copy over all of the files from app/drivers/inc to ide_alternative/Core/Inc
9. Copy over all of the files from app/drivers/src to ide_alternative/Core/Src
10. Copy all of the contents from main.c as well as main.h in the same manner as above, from app/boards/F401RE/Core/Inc/main.h and app/boards/F401RE/Core/Src/main.c into ide_alternative/Core/Inc and ide_alternative/Core/Src, respectively

#### If Changes Are Made to a .c/.h file in app/
Simply replace the corresponding driver/code file in ide_alternative/Core/

### Changes made in ide_alternative/ first
Do minimal development when possible in ide_alternative/ . If changes are made in ide_alternative that need to be synced, you may directly replace the affected lines/sections in app/ , referring to the file mapping above.