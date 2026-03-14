import sys
import serial
from PyQt5.QtWidgets import QApplication, QWidget, QTextEdit, QLineEdit, QPushButton, QVBoxLayout, QLabel
from PyQt5.QtCore import QTimer

class UARTGui(QWidget):
    def __init__(self, com_port, baudrate=115200):
        super().__init__()

        self.setWindowTitle("STM32 UART Monitor")
        self.resize(400, 300)

        # GUI widgets
        self.log = QTextEdit(self)
        self.log.setReadOnly(True)

        self.input_line = QLineEdit(self)
        self.send_btn = QPushButton("Send", self)

        self.status_label = QLabel("Disconnected", self)

        layout = QVBoxLayout()
        layout.addWidget(QLabel("UART Output:"))
        layout.addWidget(self.log)
        layout.addWidget(QLabel("Command:"))
        layout.addWidget(self.input_line)
        layout.addWidget(self.send_btn)
        layout.addWidget(self.status_label)
        self.setLayout(layout)

        # Serial setup
        try:
            self.ser = serial.Serial(com_port, baudrate, timeout=0.1)
            self.status_label.setText(f"Connected: {com_port}")
        except serial.SerialException as e:
            self.status_label.setText(f"Error opening {com_port}: {e}")
            self.ser = None

        # Button callback
        self.send_btn.clicked.connect(self.send_command)

        # Timer for reading UART
        self.timer = QTimer()
        self.timer.timeout.connect(self.read_uart)
        self.timer.start(50)  # check every 50ms

    def send_command(self):
        if self.ser and self.ser.is_open:
            cmd = self.input_line.text().strip()
            if cmd:
                self.ser.write((cmd + "\r\n").encode())  # send CR terminated
                self.log.append(f"TX -> {cmd}")
                self.input_line.clear()

    def read_uart(self):
        if self.ser and self.ser.in_waiting:
            data = self.ser.read(self.ser.in_waiting).decode(errors="ignore")
            if data:
                self.log.append(f"RX <- {data.strip()}")

if __name__ == "__main__":
    # Change this to the COM port your STM32 is using
    com_port_str = "COM9"  # e.g., "COM4" on Windows or "/dev/ttyUSB0" on Linux
    baudrate = 115200

    app = QApplication(sys.argv)
    gui = UARTGui(com_port=com_port_str, baudrate=baudrate)
    gui.show()
    sys.exit(app.exec_())