import sys
import serial
from serial.tools import list_ports
from PyQt5.QtWidgets import QApplication, QWidget, QTextEdit, QLineEdit, QPushButton, QVBoxLayout, QHBoxLayout, QLabel
from PyQt5.QtWidgets import QSplitter
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import Qt
import os
import csv
import re
from datetime import datetime

class UARTGui(QWidget):

    MAX_RPM = 300  
    G_TO_RPM = {
        "0G": 0,
        "1G": 97,
        "3G": 167,
        "5G": 216,
        "7.5G": 265,
    }

    def __init__(self, com_port, baudrate=115200):
        super().__init__()

        self.preferred_com_port = com_port
        self.com_port = com_port
        self.baudrate = baudrate
        self.ser = None

        self.setWindowTitle("STM32 UART Monitor")
        self.resize(900, 400)   

        # GUI widgets
        self.tc_log = QTextEdit(self)
        self.tc_log.setReadOnly(True)

        self.temp_log = QTextEdit(self)
        self.temp_log.setReadOnly(True)

        self.enc_log = QTextEdit(self)
        self.enc_log.setReadOnly(True)

        self.tx_log = QTextEdit(self)
        self.tx_log.setReadOnly(True)
        self.tx_log.setMaximumHeight(100)

        self.input_line = QLineEdit(self)
        self.send_btn = QPushButton("Send", self)

        self.status_label = QLabel("Disconnected", self)

        layout = QVBoxLayout()

        # buffer for partial lines from serial
        self._rx_buffer = ""

        # --- CSV logging setup ---
        run_timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

        self.tc_dir = os.path.join("data", "thermocouple")
        self.temp_dir = os.path.join("data", "temp_sensor")
        self.enc_dir = os.path.join("data", "encoder")
        for d in (self.tc_dir, self.temp_dir, self.enc_dir):
            os.makedirs(d, exist_ok=True)

        self.start_time = datetime.now()

        self.tc_file = open(os.path.join(self.tc_dir, f"{run_timestamp}.csv"), "w", newline="")
        self.tc_writer = csv.writer(self.tc_file)
        self.tc_writer.writerow(["time_s", "TC1", "TC2"])

        self.temp_file = open(os.path.join(self.temp_dir, f"{run_timestamp}.csv"), "w", newline="")
        self.temp_writer = csv.writer(self.temp_file)
        self.temp_writer.writerow(["time_s", "temp"])

        self.enc_file = open(os.path.join(self.enc_dir, f"{run_timestamp}.csv"), "w", newline="")
        self.enc_writer = csv.writer(self.enc_file)
        self.enc_writer.writerow(["time_s", "counts", "degrees", "rpm"])

        telemetry_layout = QHBoxLayout()

        tc_col = QVBoxLayout()
        tc_col.addWidget(QLabel("Thermocouple Telemetry:"))
        tc_col.addWidget(self.tc_log)
        telemetry_layout.addLayout(tc_col)

        temp_col = QVBoxLayout()
        temp_col.addWidget(QLabel("Temp Sensor Telemetry:"))
        temp_col.addWidget(self.temp_log)
        telemetry_layout.addLayout(temp_col)

        enc_col = QVBoxLayout()
        enc_col.addWidget(QLabel("BLDC Encoder Telemetry:"))
        enc_col.addWidget(self.enc_log)
        telemetry_layout.addLayout(enc_col)

        layout.addLayout(telemetry_layout)

        layout.addWidget(QLabel("Commands Sent (TX):"))
        layout.addWidget(self.tx_log)

        layout.addWidget(QLabel("Commands:"))
        quick_btn_layout = QHBoxLayout()
        quick_commands = [
            # "SYSTEM ON",   # Re-enable only if the automatic sequence returns.
            # "SYSTEM OFF",  # Re-enable only if the automatic sequence returns.
            "HEATER ON",
            "HEATER OFF",
            "QUARTER MOVE",
            # "SOLDER",
            # "NEXT POSITION",
            # "CLEAN",
            # "RETRACT TIPS",  # Re-enable when manual retraction is needed.
        ]
        for label in quick_commands:
            btn = QPushButton(label, self)
            btn.clicked.connect(lambda checked, cmd=label: self.send_string(cmd))
            quick_btn_layout.addWidget(btn)
        layout.addLayout(quick_btn_layout)

        layout.addWidget(QLabel("BLDC Motor Commands:"))
        g_btn_layout = QHBoxLayout()
        for label, rpm in self.G_TO_RPM.items():
            btn = QPushButton(label, self)
            btn.clicked.connect(lambda checked, l=label, r=rpm: self.send_pwm_command(l, r))
            g_btn_layout.addWidget(btn)
        layout.addLayout(g_btn_layout)

        layout.addWidget(QLabel("Custom Command Input:"))
        layout.addWidget(self.input_line)
        layout.addWidget(self.send_btn)

        layout.addWidget(self.status_label)

        self.setLayout(layout)

        # Serial setup
        self.open_serial()

        self.send_btn.clicked.connect(self.send_command)

        self.timer = QTimer()
        self.timer.timeout.connect(self.read_uart)
        self.timer.start(50)

        self.reconnect_timer = QTimer()
        self.reconnect_timer.timeout.connect(self.open_serial)
        self.reconnect_timer.start(1000)

    def open_serial(self):
        if self.ser and self.ser.is_open:
            return

        self.com_port = self.find_stm32_port()

        try:
            self.ser = serial.Serial(self.com_port, self.baudrate, timeout=0.1)
            self.status_label.setText(f"Connected: {self.com_port}")
        except (serial.SerialException, OSError) as e:
            self.ser = None
            self.status_label.setText(f"Waiting for {self.com_port}: {e}")

    def find_stm32_port(self):
        ports = list(list_ports.comports())

        for port in ports:
            if port.device.upper() == self.preferred_com_port.upper():
                return port.device

        stm32_markers = ("STMICRO", "STM32", "ST-LINK", "STLINK")
        for port in ports:
            port_info = " ".join(
                str(value or "")
                for value in (
                    port.description,
                    port.manufacturer,
                    port.product,
                    port.hwid,
                )
            ).upper()
            if any(marker in port_info for marker in stm32_markers):
                return port.device

        return self.preferred_com_port

    def handle_serial_error(self, error):
        if self.ser:
            try:
                self.ser.close()
            except (serial.SerialException, OSError):
                pass

        self.ser = None
        self.status_label.setText(
            f"Disconnected from {self.com_port}; reconnecting: {error}"
        )

    def write_command(self, cmd):
        if not self.ser or not self.ser.is_open:
            self.status_label.setText(
                f"Not connected to {self.com_port}; command not sent"
            )
            return False

        try:
            self.ser.write((cmd + "\r\n").encode())
            self.tx_log.append(cmd)
            return True
        except (serial.SerialException, OSError) as e:
            self.handle_serial_error(e)
            return False

    def send_command(self):
        cmd = self.input_line.text().strip()
        if cmd:
            if self.write_command(cmd):
                self.input_line.clear()

    def read_uart(self):
        if not self.ser or not self.ser.is_open:
            return

        try:
            waiting = self.ser.in_waiting
            if waiting:
                data = self.ser.read(waiting).decode(errors="ignore")
                if data:
                    self._rx_buffer += data
                    while "\n" in self._rx_buffer:
                        line, self._rx_buffer = self._rx_buffer.split("\n", 1)
                        line = line.strip()
                        if not line:
                            continue
                        self.route_line(line)
        except (serial.SerialException, OSError) as e:
            self.handle_serial_error(e)

    def send_string(self, cmd):
        self.write_command(cmd)

    # def route_line(self, line):
    #     if line.startswith("TC1:"):
    #         self.tc_log.append(line)
    #     elif line.startswith("ENC:"):
    #         self.enc_log.append(line[len("ENC:"):].strip())
    #     elif line.startswith("TEMP:"):
    #         self.temp_log.append(line[len("TEMP:"):].strip())
    #     else:
    #         self.tc_log.append(f"[unrouted] {line}")

    def route_line(self, line):
        elapsed = (datetime.now() - self.start_time).total_seconds()

        if line.startswith("TC1:"):
            self.tc_log.append(line)
            m = re.search(r"TC1:\s*(-?\d+\.?\d*).*?TC2:\s*(-?\d+\.?\d*)", line)
            if m:
                self.tc_writer.writerow([elapsed, m.group(1), m.group(2)])
                self.tc_file.flush()

        elif line.startswith("ENC:"):
            val = line[len("ENC:"):].strip()
            self.enc_log.append(val)
            m = re.search(r"(-?\d+)\s*counts\s*\|\s*(-?\d+\.?\d*)\s*deg\s*\|\s*(-?\d+\.?\d*)\s*RPM", val)
            if m:
                self.enc_writer.writerow([elapsed, m.group(1), m.group(2), m.group(3)])
                self.enc_file.flush()

        elif line.startswith("TEMP:"):
            val = line[len("TEMP:"):].strip()
            self.temp_log.append(val)
            m = re.search(r"(-?\d+\.?\d*)", val)
            if m:
                self.temp_writer.writerow([elapsed, m.group(1)])
                self.temp_file.flush()

        else:
            self.tc_log.append(f"[unrouted] {line}")

    def rpm_to_pwm(self, rpm):
        """Convert an RPM value to a 0-255 PWM scale based on MAX_RPM."""
        pwm = int(round((rpm / self.MAX_RPM) * 255))
        return max(0, min(255, pwm))  # clamp to valid range

    def send_pwm_command(self, label, rpm):
        pwm_val = self.rpm_to_pwm(rpm)
        cmd = f"{pwm_val}"
        self.write_command(cmd)
    
    def closeEvent(self, event):
        self.timer.stop()
        self.reconnect_timer.stop()

        if self.ser:
            try:
                self.ser.close()
            except (serial.SerialException, OSError):
                pass

        for f in (self.tc_file, self.temp_file, self.enc_file):
            if f:
                f.close()
        event.accept()


if __name__ == "__main__":
    # Optional override: python gui.py COM7
    com_port_str = sys.argv[1] if len(sys.argv) > 1 else "COM7"
    baudrate = 115200

    app = QApplication(sys.argv)
    gui = UARTGui(com_port=com_port_str, baudrate=baudrate)
    gui.show()
    sys.exit(app.exec_())
