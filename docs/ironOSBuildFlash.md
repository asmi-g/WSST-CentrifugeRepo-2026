# Pinecil V2 Firmware Build & Flash Guide

This project integrates **IronOS** (firmware) and **blisp** (flashing tool) to build and flash firmware onto the Pinecil V2 (BL70x).

The workflow is:
1. [One-Time Setup (New Machine, build blisp)](#one-time-setup-new-machine)
2. Making Changes to IronOS: follow [IronOS Development Workflow](#ironos-development-workflow)
3. Testing on Pinecil: [Flashing IronOS Firmware](#flashing-current-ironos-firmware) 

---

# Prerequisites

Make sure to have the following:

- All applications/pre-reqs mentioned in main README
- `docker` (recommended for [IronOS builds](https://github.com/Ralim/IronOS/blob/dev/Documentation/Development.md#building-pinecil-v2), [Install Docker Desktop](https://docs.docker.com/desktop/setup/install/windows-install/))
- USB access to the Pinecil V2

---

# Repository Structure

```sh
your-main-repo/
├── ...
├── app
├── docs
├───── ironOSBuildFlash.md
├───── gitBestPractices.md
├───── ...
├── tools/ 
├───── firmware
├──────── ironos
├────────── default # Fallback/default ironos executable
├────────── out # Local ironos exectuable (built by you, not tracked or committed)
├────────── tracked # Our tracked ironos executable
├───── scripts/ # Build + flash scripts
├───── IronOS/ # IronOS source (forked, submodule)
├──────── source/Core/... # make your IronOS changes here
├───── blisp/ # blisp source (forked, submodule)
├───── bin/ # Built tools (generated, not committed)
└── README.md
```

Generated directories (`tools/bin/`, `tools/firmware/ironos/out/`) should not be committed to Git.

---
## Flashing Current IronOS Firmware
To flash the current IronOS executable,`tools/firmware/ironos/current/Pinecilv2_EN.bin`:

### 0. One-Time Setup (if applicable)
If not already done so, go through the [One-Time Setup](#one-time-setup-new-machine) section to setup and build blisp.

### 1. Pull Most Recent IronOS Changes:

<b>Option A - Testing from a feature branch:</b> Checkout feature branch with the IronOS executable you want to test and pull, from the main project directory:
```sh
git fetch origin
git checkout <feature-branch>
git pull
git submodule update --init --recursive
```
This ensures you're on the feature branch with the IronOS executable you want to test and you're on the exact IronOS commit the main repo expects.

<b>Option B - Testing from the main branch:</b>

Same as above but checkout the main branch. The current IronOS executable from the main branch should always be stable:
```sh
git fetch origin
git checkout main
git pull
git submodule update --init --recursive
```

### 2. Flash Mode

Put the Pinecil into flash mode: <br>
1. Connect the Pinecil V2 to your PC
2. Long hold the [-] button before plugging the USB-C cable into the back of the Pinecil. Keep holding down the [-] for ~10-15 seconds after plugging in the cable, then release the button.
3. Screen should be black/empty which means Pinecil is in Flashing Mode. If you have issues, try again, do not plug the USB-C cable into Pinecil until you first press & hold the [-] button. Flip the cable over or try another port/cable/PC if you still have issues.
4. Pinecil will not show in the PC as a USB data drive. On Windows, you will hear a single beep when connected in flash mode.

### 3. Flash Executable
Flashes the ironOS executable found in `tools/firmware/ironos/current/Pinecilv2_EN.bin`:

```sh
cd tools
chmod +x ./scripts/*.sh # only have to do once
./scripts/flash_pinecilv2.sh
```

<b>Note:</b>

 `flash_pinecilv2.sh` checks if blisp exists, copies the `.bin` file next to the blisp executable, then flashes with the command `sudo ./blisp write -c bl70x --reset <firmware>.bin` (as per [IronOS docs](https://pine64.org/documentation/Pinecil/Firmware/#option-2-blisp)). Refer back to these docs if you run into issues. If a current IronOS executable is not found, the script fallsback to the default executable in `tools/firmware/ironos/fallback/Pinecilv2_EN.bin`.

---

# One-Time Setup (New Machine)

### 1. Initialize Git Submodules

This repository contains submodules (e.g., for `tools/blisp`, `IronOS`):

```sh
git submodule update --init --recursive
```

### 2. Build blisp (Flashing Tool)

We build blisp once and store the executable at: `tools/bin/blisp`

Run:
```sh
source app/init/.bashrc

cmake -S tools/blisp -B tools/blisp/build -G "MinGW Makefiles"  -DBLISP_BUILD_CLI=ON -DCMAKE_C_COMPILER=CC   -DCMAKE_CXX_COMPILER=CXX

cmake --build tools/blisp/build --config Release

mkdir -p tools/bin

cp tools/blisp/build/tools/blisp/blisp tools/bin/blisp

chmod +x tools/bin/blisp
```

You normally do not need to rebuild blisp unless:
- The tools/blisp submodule is updated
- You delete tools/bin/blisp
- Your toolchain changes

---

# IronOS Development Workflow

### 1. Initialize Submodules (One-Time Setup)
This repository contains submodules (e.g., for `tools/blisp`, `tools/IronOS`):
```sh
git submodule update --init --recursive
```

### 2. Create/switch to your feature branch inside IronOS
Follow `gitBestPractices.md`.
```sh
cd IronOS
git checkout -b "<your-feature-branch>" # follow gitBestPractices.md
```

### 3. Modify IronOS source code

Edit files in: `tools/IronOS/source/Core...`.

Note – TODOs:
<br>
1) Implement handler functions for incoming UART commands (get current temperature and set temperature controls), see PIDThread and GUIThread for how temp controls are set currently (in `IronOS/source/Core/Threads/GUIThread.cpp`)
2) Make sure iron doesn't go into sleep mode under microgravity (currently sleeps after setting the iron down after about a minute)m see MOVThread.cpp for accelerometer settings
3) Change start-up behaviour to "S=heat to soldering temp", see Settings.md
4) Heating Profile Thread? (to track iron states, may not be required)
<br>

### 4. Test your changes: Build IronOS Executable + Flash to Pinecil

When you make changes to IronOS (submodule), build using Docker, test your IronOS build by flashing to the Pinecil, update the current IronOS executable then push your changes to your feature branch so everyone else can flash and test your new firmware without building themselves. These instructions are as follows:

#### a) Start Docker Desktop
First ensure that Docker Desktop is installed, as indicated in [Prerequisites](#Prerequisites).

Then in your device's search bar, look up and click on the "Docker Desktop" app to start the Docker engine on your device.

#### b) Build your firmware locally (Docker build)
This builds IronOS using its Docker-based environment and outputs your IronOS Executable (a `.bin` file) into `tools/firmware/ironos/out/`, run from the main project directory:

```sh
cd tools
chmod +x ./scripts/*.sh # only have to do once
./scripts/build_ironos_pinecilv2.sh
```
Re-running the build script will overwrite existing `Pinecilv2_EN.bin` in the `tools/firmware/ironos/out/` directory.

#### c) Confirm you have a `.bin` in:
```sh
ls firmware/ironos/out
# expect Pinecilv2_EN.bin (ensure the filename is Pinecilv2_EN.bin)
```
#### d) Test your changes locally (flash your IronOS build)
Put the Pinecil into [Flash Mode](#2-flash-mode), then flash your local IronOS build:

```sh
./scripts/flash_pinecilv2.sh firmware/ironos/out/Pinecilv2_EN.bin
```
<b>Important Note:</b>

There is a fallback/default Pinecil firmware in: `tools/firmware/fallback`

If you want to revert back to the default Pinecil/IronOS firmware, replace the `.bin` path above with `tools/firmware/ironos/fallback/Pinecilv2_EN.bin` and run that flash command.

Repeat edit → build → flash until satisfied. Continue to step 5 when ready AFTER testing.

<b>Leave the Pinecil in a working state when you're done with testing (e.g. flash the default firmware to revert back to initial state)</b>

 To do a clean build:
```sh
# from tools/
rm -rf firmware/ironos/out bin/Pinecilv2_EN.bin
./scripts/build_ironos_pinecilv2.sh
```

#### e) Update current IronOS executable

When you're ready, copy your tested `Pinecilv2_EN.bin` into `tools/firmware/ironos/current` (replace the current one that's there), still from `/tools`:
```sh
cp firmware/ironos/out/Pinecilv2_EN.bin firmware/ironos/current
```

### 5. Commit and push your IronOS changes

```sh
git add "IronOS/source/<files-you-changed>" # remember to add your tools/firmware/ironos/current/Pinecilv2_EN.bin here
git commit -m "Modify IronOS: <description>" 
git push -u origin <your-feature-branch>
```

Generated directories (`tools/bin/`, `tools/firmware/ironos/out/`) should not be committed to Git (already included in .gitignore)

### 6. Update the main repo to point to this new commit

This updates the submodule pointer in the main repo.

```sh
cd ..
git add "tools/IronOS"
git commit -m "Bump IronOS submodule"
git push -u origin <your-feature-branch>
```

---

# To-Dos:

## Updating from IronOS Upstream
Do this periodically to stay in sync with IronOS's latest changes.
TODO: Add instructions