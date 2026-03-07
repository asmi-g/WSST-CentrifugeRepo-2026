# IronOS Operations Documentation

## Change Boot Temperature

1. Find the configuration.h for the PinecilV2. This is located at:
``` tools/IronOS/source/Core/BSP/Pinecilv2/configuration.h```

2. Change the start mode to autostart, in line 42:
Options:
 * Auto start off for safety.
 * Pissible values are:
 *  0 - none
 *  1 - Soldering Temperature
 *  2 - Sleep Temperature
 *  3 - Sleep Off Temperature
 
```#define AUTO_START_MODE 1 // Soldering Temperature```

3. Disable sleep and shutdown mode, in lines 31 and 32:
 * How many seconds/minutes we wait until going to sleep/shutdown.
 * Values -> SLEEP_TIME * 10; i.e. 5*10 = 50 Seconds!
``` #define SLEEP_TIME    0  // x10 Seconds ```
``` #define SHUTDOWN_TIME 0 // Minutes ```

4. Change soldering temp in line 129
``` #define SOLDERING_TEMP             380                       // Default soldering temp is 320.0 °C```

5. Compile and flash, as instructed in ```ironOSBuildFlash.md```

6. Enable "Restore Default Settings" manually on Pinecil
    * Power on the iron.
    * Hold the “–” (minus) button to enter the settings menu.
    * Scroll through the menu until you find: Restore Default Settings
    * Press the “+” button to select it.
    * Confirm if it asks.
