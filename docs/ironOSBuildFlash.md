# Pinecil V2 Firmware Build & Flash Guide

This project integrates **IronOS** (firmware) and **blisp** (flashing tool) to build and flash firmware onto the Pinecil V2 (BL70x).

The workflow is:
1. One-Time Setup (New Machine, build blisp)
2. Making Changes to IronOS: follow [IronOS Development Workflow](#ironos-development-workflow)
3. Testing on Pinecil: [Building and Flashing IronOS](#Building-and-Flashing-IronOS) (Generates a `.bin` artifact and flashes the firmware using `blisp`)

---

# Prerequisites

Make sure to have the following:

- All applications/pre-reqs mentioned in main README
- `docker` (recommended for [IronOS builds](https://github.com/Ralim/IronOS/blob/dev/Documentation/Development.md#building-pinecil-v2))
- USB access to the Pinecil V2

---

# Repository Structure

```sh
your-main-repo/
├── ...
├── app
├── docs
├── scripts/ # Build + flash scripts
├── IronOS/ # IronOS source (forked, submodule) TODO: Fork + add submodule
├───── source/Core/... # make your IronOS changes here
├───── ...
├── tools/ 
├───── blisp/ # blisp source (submodule) TODO: Add submodule
├───── bin/ # Built tools (generated, not committed)
├── out/ironos/ # Artifacts (generated, not committed)
└── README.md
```

Generated directories (`tools/bin/`, `out/`) should not be committed to Git.
TODO: cleaner way to organize this?

---

# IronOS Development Workflow

### 1. Initialize Submodules (One-Time Setup)
This repository contains submodules (e.g., for `tools/blisp`, `IronOS`):
```sh
git submodule update --init --recursive
```

### 2. Create/switch to your feature branch inside IronOS
Follow `gitBestPractices.md`.
```sh
cd IronOS
git checkout -b "<your-feature-branch>"
```

### 3. Modify IronOS source code

Edit files in: `IronOS/source/Core...`.

Note – TODOs:
<br>
1) Implement handler functions for incoming UART commands (get current temperature and set temperature controls), see PIDThread and GUIThread for how temp controls are set currently (in `IronOS/source/Core/Threads/GUIThread.cpp`)
2) Heating Profile Thread? (to track iron states, may not be required)
<br>

### 4. Test your changes: Build + Flash to Pinecil

Go to section: [Building and Flashing IronOS](#Building-and-Flashing-IronOS) for more details

```sh
./scripts/build_ironos_pinecilv2.sh EN
# Put Pinecil in Flash Mode
./scripts/flash_pinecilv2.sh out/ironos/Pinecilv2_EN.bin
```

Fallback/Default Pinecil firmware in: `/IronOS/???` (TODO: add this). If you want to revert back to the default Pinecil/IronOS firmware, move the .bin file to ... and run the flash command.

Repeat edit → build → flash until satisfied. Continue to step 5 when ready AFTER testing. 

<b>Leave the Pinecil in a working state when you're done with testing (e.g. flash the default firmware to revert back to initial state)</b>

### 5. Commit and push your IronOS changes

```sh
git add "source/<files-you-changed>"
git commit -m "Modify IronOS: <description>"
git push -u origin <your-feature-branch> # TODO: how would this fit into our current git workflow
```

Generated directories (`tools/bin/`, `out/`) should not be committed to Git. (TODO: add these to gitignore)

### 6. Update the main repo to point to this new commit

This updates the submodule pointer in the main repo.

```sh
cd ..
git add "/IronOS"
git commit -m "Bump IronOS submodule"
git push # TODO: how would this fit into our current git workflow
```

### <b> 7.  ** Teammate Workflow ** </b>
When someone pulls the main repo:
```sh
git pull
git submodule update --init --recursive
```
That ensures they’re on the exact IronOS commit the main repo expects.

## Building and Flashing IronOS

### 2. Build IronOS (Pinecil v2 Firmware)

This builds IronOS using its Docker-based environment and outputs a `.bin` file into `out/ironos/`, run from the main project directory:
```
./scripts/build_ironos_pinecilv2.sh EN
```
You should see a firmware file at: `out/ironos/Pinecilv2_EN.bin`

### 2. Flash firmware to Pinecil

Put the Pinecil into flash mode: <br>
1. Connect the Pinecil V2 to your PC
2. Long hold the [-] button before plugging the USB-C cable into the back of the Pinecil. Keep holding down the [-] for ~10-15 seconds after plugging in the cable, then release the button.
3. Screen should be black/empty which means Pinecil is in Flashing Mode. If you have issues, try again, do not plug the USB-C cable into Pinecil until you first press & hold the [-] button. Flip the cable over or try another port/cable/PC if you still have issues.
4. Pinecil will not show in the PC as a USB data drive. On Windows, you will hear a single beep when connected in flash mode.

To flash, run:
```
./scripts/flash_pinecilv2.sh out/ironos/Pinecilv2_EN.bin
```

Note: `flash_pinecilv2.sh` checks if blisp exists, copies the `.bin` file next to the blisp executable, then flashes with the command `sudo ./blisp write -c bl70x --reset <firmware>.bin` (as per [IronOS docs](https://pine64.org/documentation/Pinecil/Firmware/#option-2-blisp)). Refer back to these docs if you run into issues.

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
cmake -S tools/blisp -B tools/blisp/build -DBLISP_BUILD_CLI=ON

cmake --build tools/blisp/build --config Release

mkdir -p tools/bin

cp tools/blisp/build/tools/blisp tools/bin/blisp 2>/dev/null || cp tools/blisp/build/Release/blisp tools/bin/blisp # copy blisp executable, TODO: Might have to change this path

chmod +x tools/bin/blisp
```

You normally do not need to rebuild blisp unless:
- The tools/blisp submodule is updated
- You delete tools/bin/blisp
- Your toolchain changes

TODO: Add blisp build artifacts to gitignore, [blisp reference](https://github.com/pine64/blisp#how-to-update-pinecil-v2)

# Updating from IronOS Upstream
Do this periodically to stay in sync with IronOS's latest changes.
TODO: Add instructions