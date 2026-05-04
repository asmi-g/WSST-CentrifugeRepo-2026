# Logs thermocouple data to CSV file in app/logs, formatted as:
# time,TC1_C,TC2_C
# 02:02:35.095,0.00,4095.99
# 02:02:35.598,0.00,4095.99
# 02:02:36.102,0.00,4095.99

# Replace PORT with your STM's port
# Run python file: python3 app/logs/serial_logger.py
# CSV file saves to app/logs/

import serial
from datetime import datetime

PORT = "/dev/cu.usbmodem21103" # REPLACE THIS
BAUD = 115200

filename = f"app/logs/temperature_{datetime.now():%Y%m%d_%H%M%S}.csv"

with serial.Serial(PORT, BAUD, timeout=1) as ser, open(filename, "w") as f:
    print(f"Logging to {filename}")
    print("Press Ctrl+C to stop.")

    # Updated header
    f.write("time,TC1_C,TC2_C\n")

    try:
        while True:
            line = ser.readline().decode(errors="ignore").strip()

            if line and not line.startswith("time_ms"):
                parts = line.split(",")

                if len(parts) == 3:
                    # HH:MM:SS.mmm (milliseconds)
                    now = datetime.now().strftime("%H:%M:%S.%f")[:-3]

                    new_line = f"{now},{parts[1]},{parts[2]}"
                    print(new_line)
                    f.write(new_line + "\n")

    except KeyboardInterrupt:
        print(f"\nSaved: {filename}")