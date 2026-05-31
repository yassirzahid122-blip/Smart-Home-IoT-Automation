# Smart Home & IoT Automation System

An embedded C++ application designed for real-time smart home environmental management and automated perimeter security frameworks. The system integrates an Arduino microcontroller backend tracking sensor data inputs with a custom Android mobile application built using MIT App Inventor to establish seamless wireless control and state monitoring.

## 🚀 Key Architectural Features
* **Perimeter Security Engine:** Monitors digital inputs from simulated door and window switches. Features an active security state-machine that triggers emergency visual patterns (LED strobing) and logs high-priority Serial transmission alerts if unauthorized entry points are tripped.
* **Intelligent Climate Control:** Reads live data arrays from a DHT11 temperature and humidity sensor. Utilizes dynamic mathematical thresholds to control multi-stage heating/cooling hardware elements and employs Pulse Width Modulation (PWM) to scale system fan speeds variable to environmental shifts.
* **Asynchronous Remote Command Parser:** Integrates a structured Serial data parsing layer optimized for Bluetooth communication modules. Interprets custom-delimited string codes (e.g., `T:[val]`, `LD`, `UD`) from the mobile client to programmatically configure target room variations and master lock arrays.

## 🛠️ Tech Stack & Components
* **Core Software Languages:** Embedded C++ (Arduino IDE), MIT App Inventor Block Architecture
* **Hardware Protocols:** Serial Communications (UART / Bluetooth Client Profile), Pulse Width Modulation (PWM), General Purpose Input/Output (GPIO)
* **Monitored Hardware Ecosystem:** DHT11 Temperature & Humidity Sensor, Mechanical Limit Contacts, Dual-Stage Transistorized LED Circuit Matrices, Variable DC Motor Drives

## 📲 Serial & Bluetooth Command Interface Specs
The embedded parser processes incoming strings with the following operational matrices:
| Command String | System State Transformation |
| :--- | :--- |
| `T:[Float]` | Recalibrates target environmental equilibrium temperature |
| `LD` / `UD` | Sets / clears active software alarm state for perimeter door vectors |
| `LW` / `UW` | Sets / clears active software alarm state for perimeter window vectors |
| `LB` / `UB` | Executes master operational state locking/unlocking for all vectors |
