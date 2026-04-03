# 🚗 Bluetooth RC Car — Arduino + Servo + L298N

> A real car-style Bluetooth-controlled RC vehicle featuring rear-wheel drive and front-wheel servo steering, built on Arduino UNO and controlled wirelessly from an Android device.

---

## 📋 Table of Contents

- [Project Overview](#-project-overview)
- [Features](#-features)
- [Hardware Components](#-hardware-components)
- [Software & Technologies](#-software--technologies)
- [Circuit Connections](#-circuit-connections)
- [System Architecture](#-system-architecture)
- [Mobile Control Commands](#-mobile-control-commands)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Troubleshooting](#-troubleshooting)
- [Future Roadmap](#-future-roadmap)
- [Author](#-author)

---

## 📌 Project Overview

This project implements a **Bluetooth-controlled RC Car** using an **Arduino UNO** as the brain. The design mimics real-world vehicle mechanics:

- 🔴 **Rear wheels** — driven by 2 DC motors via an L298N motor driver
- 🔵 **Front wheels** — steered by an SG90 servo motor
- 📡 **Wireless control** — HC-05 Bluetooth module receives commands from an Android app

The result is a portable, battery-powered car that responds to directional commands in real time, replicating the feel of an actual car's steering and drive system.

---

## ⚙️ Features

| Feature | Details |
|---|---|
| 📱 Wireless Control | Android Bluetooth app (e.g., Bluetooth RC Controller) |
| ⬆️ Forward / ⬇️ Backward | Rear-wheel DC motor drive |
| ⬅️ Left / ➡️ Right | Front-wheel servo steering |
| ⏹️ Stop | Instant motor cut-off |
| 🚗 Real Steering | Servo-based front axle, not skid-steering |
| 🔋 Portable Power | 7.4V Li-ion battery pack |
| ⚡ PWM Speed Control | ENA/ENB pins allow adjustable motor speed |

---

## 🧰 Hardware Components

| Component | Model / Spec | Purpose |
|---|---|---|
| Microcontroller | Arduino UNO (ATmega328P) | Central processing & control logic |
| Bluetooth Module | HC-05 | Wireless serial communication |
| Motor Driver | L298N Dual H-Bridge | Drives both rear DC motors |
| DC Motors | Generic TT motors (x2) | Rear-wheel propulsion |
| Servo Motor | SG90 (9g) | Front-wheel steering |
| Battery | 7.4V Li-ion / LiPo pack | Main power supply |
| Chassis | 4-wheel RC car frame | Structural base |
| Wheels & Tires | Matching chassis set | Traction & movement |
| Jumper Wires | Male-to-male / Male-to-female | Circuit connections |
| Breadboard (optional) | Half-size | Voltage divider for HC-05 RX |

> ⚠️ **Voltage Note:** The L298N requires a minimum of **6V** on its motor supply pins. Use a **7.4V** battery for reliable motor torque.

---

## 💻 Software & Technologies

| Technology | Purpose |
|---|---|
| [Arduino IDE](https://www.arduino.cc/en/software) | Code compilation & upload |
| C++ (Arduino framework) | Application logic |
| `SoftwareSerial` library | UART communication with HC-05 |
| `Servo` library | PWM signal for servo steering |
| L298N control logic | Digital pin-based motor direction control |
| Android Bluetooth App | Serial command sender (e.g., *Bluetooth RC Controller*) |

---

## 🔌 Circuit Connections

### 🟢 L298N Motor Driver → Arduino UNO

| L298N Pin | Arduino Pin | Notes |
|---|---|---|
| IN1 | D8 | Motor A direction |
| IN2 | D9 | Motor A direction |
| IN3 | D10 | Motor B direction |
| IN4 | D11 | Motor B direction |
| ENA | D5 (PWM) | Motor A speed control |
| ENB | D6 (PWM) | Motor B speed control |
| GND | GND | Common ground — **required** |
| VCC (5V out) | — | Do not use; power Arduino separately |
| 12V IN | Battery (+) | Motor power supply (7.4V) |

> 💡 Remove the ENA/ENB jumpers if you want PWM speed control. Leave them on to run motors at full speed.

---

### 🔵 HC-05 Bluetooth Module → Arduino UNO

| HC-05 Pin | Arduino Pin | Notes |
|---|---|---|
| VCC | 5V | 3.3V logic, 5V power tolerant |
| GND | GND | Common ground |
| TX | D2 (SoftwareSerial RX) | HC-05 sends data → Arduino receives |
| RX | D3 (SoftwareSerial TX) | **Use voltage divider!** (5V → 3.3V) |

> ⚠️ **Voltage Divider Required on RX:**  
> HC-05 RX is 3.3V logic. Connect D3 through a **1kΩ + 2kΩ voltage divider** to safely step down the 5V Arduino signal.

```
Arduino D3 ──[1kΩ]──┬── HC-05 RX
                     │
                   [2kΩ]
                     │
                    GND
```

---

### 🎯 Servo Motor (SG90) → Arduino UNO

| Servo Wire | Arduino Pin | Notes |
|---|---|---|
| Signal (Orange/Yellow) | D4 | PWM signal via `Servo` library |
| VCC (Red) | 5V | Power from Arduino |
| GND (Brown/Black) | GND | Common ground |

> ⚡ If the servo causes Arduino resets, power it from an external 5V regulator rather than the Arduino's onboard regulator.

---

## 🧠 System Architecture

```
┌─────────────────────────────────────────────┐
│              Android Mobile App              │
│         (Bluetooth RC Controller)            │
└──────────────────┬──────────────────────────┘
                   │ Bluetooth (Serial)
                   ▼
┌─────────────────────────────────────────────┐
│              HC-05 Bluetooth Module          │
│         Receives: F / B / L / R / S          │
└──────────────────┬──────────────────────────┘
                   │ UART (SoftwareSerial D2/D3)
                   ▼
┌─────────────────────────────────────────────┐
│                Arduino UNO                   │
│  Parses command → drives motors & servo      │
└────────┬─────────────────────┬──────────────┘
         │ D8-D11, D5, D6      │ D4 (PWM)
         ▼                     ▼
┌─────────────────┐   ┌─────────────────────┐
│  L298N Driver   │   │    SG90 Servo Motor  │
│  (Rear motors)  │   │  (Front steering)    │
└────────┬────────┘   └─────────────────────┘
         │
    ┌────┴────┐
    ▼         ▼
[Motor A]  [Motor B]
(Left rear) (Right rear)
```

---

## 📱 Mobile Control Commands

| Command Sent | Action | Motor State | Servo Angle |
|---|---|---|---|
| `F` | Move Forward | Both motors forward | 90° (center) |
| `B` | Move Backward | Both motors reverse | 90° (center) |
| `L` | Turn Left | Both motors forward | ~60° (left) |
| `R` | Turn Right | Both motors forward | ~120° (right) |
| `S` | Stop | Both motors off | 90° (center) |

> 🔧 Servo center, left, and right angles can be tuned in the code to match your physical steering linkage.

---

## 📁 Project Structure

```
Bluetooth-RC-Car/
│
├── main.ino              # Arduino source code (main logic)
├── README.md             # Project documentation (this file)
└── images/
    └── car_structure.jpg # Circuit diagram / project photos
```

---

## 🚀 Getting Started

### 1. Prerequisites

- Arduino IDE installed ([download here](https://www.arduino.cc/en/software))
- Required libraries (install via Arduino Library Manager):
  - `Servo` (built-in)
  - `SoftwareSerial` (built-in)
- Android phone with a Bluetooth serial app (e.g., **Bluetooth RC Controller** on Play Store)

### 2. Wiring

Follow the [Circuit Connections](#-circuit-connections) section above. Double-check:
- Common GND between Arduino, L298N, and battery
- Voltage divider on HC-05 RX pin
- ENA/ENB jumpers on L298N if not using PWM speed control

### 3. Upload Code

1. Open `main.ino` in Arduino IDE
2. Select **Board:** Arduino UNO
3. Select the correct **COM Port**
4. Click **Upload**

### 4. Pair Bluetooth

1. Power on the car
2. Go to Android **Settings → Bluetooth → Pair new device**
3. Select **HC-05** from the list
4. Enter pairing code: `1234` (or `0000`)

### 5. Connect & Drive

1. Open your Bluetooth RC app
2. Connect to HC-05
3. Use the app's directional buttons to control the car

---

## 🛠️ Troubleshooting

### ❌ Motors not moving

- Verify battery voltage is **≥ 7.4V** (check with multimeter)
- Confirm L298N **GND is connected to Arduino GND** (common ground)
- Check that **ENA and ENB jumpers are installed**, or that D5/D6 are sending a HIGH/PWM signal
- Test each IN pin manually with `digitalWrite()` to isolate the issue

### ❌ Bluetooth won't connect or pair

- Default pairing codes: `1234` or `0000`
- **Pair the device in Android Settings first** before opening the RC app
- HC-05 LED should blink rapidly when unconnected, and slowly (every ~2 sec) when connected
- Ensure HC-05 VCC is connected to **5V**, not 3.3V

### ❌ Servo twitches or doesn't respond

- Confirm signal wire is connected to **D4**
- If the servo causes Arduino instability, power it from a separate regulated 5V source
- Check that the `Servo` library is included at the top of `main.ino`

### ❌ Car moves but steering doesn't work

- Confirm servo signal is reaching D4 using `Serial.print()` debug output
- Manually test servo with the `Servo` library sweep example to rule out a hardware fault
- Check wiring polarity (signal ≠ VCC ≠ GND)

### ❌ Car steers but motors don't respond

- Check IN1–IN4 wiring matches the pin definitions in the code
- Verify L298N motor power supply rail is connected (the 12V/VIN terminal, not the 5V logic pin)

---

## 🔭 Future Roadmap

| Milestone | Description |
|---|---|
| 📱 Custom Flutter App | Joystick-based UI with variable speed slider |
| ⚡ PWM Speed Zones | Slow / Medium / Fast modes via ENA/ENB |
| 📷 Live Video | ESP32-CAM module for first-person view streaming |
| 🤖 Obstacle Avoidance | HC-SR04 ultrasonic sensor with auto-stop / auto-steer |
| 🌐 Wi-Fi Control | Migrate from HC-05 to ESP32 for web-based control |
| 🔋 Battery Monitor | Voltage divider + OLED display for live battery level |
| 📊 Telemetry | Speed estimation via wheel encoder + Serial dashboard |

---

## 👨‍💻 Author

**Vinura Deelaka**  
Arduino Robotics & Embedded Systems Project

Built to explore and learn **embedded systems**, **IoT communication protocols**, and **robotics control systems** using accessible and affordable hardware.

---

## 📄 License

This project is open-source and free to use for educational and personal purposes.  
Attribution appreciated — feel free to fork, modify, and build upon it! 🚀

---

*Built with ❤️ using Arduino — because real engineers start with hardware.*
