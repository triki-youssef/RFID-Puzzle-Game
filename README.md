<h1>🔐 RFID Puzzle Game with ESP8266 Clients & Central Hub </h1>
This project is an interactive RFID-based puzzle system using 4 Wemos D1 Mini boards and MFRC522 RFID readers. Each RFID reader must scan a specific card in the correct sequence. A central D1 Mini controls the logic, checks if all clients scanned their correct UIDs, and provides a reset mechanism to restart the puzzle.

<h2>🧠 Project Idea</h2>
Imagine a smart escape room puzzle: 4 RFID stations, each connected to a Wemos D1 Mini. Players must scan the correct cards on each station. Once all are scanned correctly, a central hub acknowledges success. Pressing the reset button puts all devices into learn mode, ready to accept new RFID tags.

<h2>📌 Project Overview</h2>
This project is an interactive RFID-based puzzle game using 4 Wemos D1 Mini boards and MFRC522 RFID readers. One board acts as the Central, which includes a reader and behaves like a client, while the other three boards are Clients. Each board scans RFID tags and communicates via UDP over Wi-Fi. When the correct RFID tags are scanned in all readers, a success action is triggered. A physical reset button on the central resets the game.

<h2>🚀 Features</h2>
- 🔄 Resettable RFID puzzle logic

- 📡 Central + 3 Client communication over Wi-Fi (UDP)

- 💾 Save & verify UID for each client

- 🔄 Learning Mode: change the correct RFID cards at any time

- 💡 LED indicators for feedback

- 🧩 Ideal for puzzle rooms, interactive installations, or smart locks


 <h2> 🧠 How It Works</h2>
- The Central ESP8266 acts as both:

   - Wi-Fi Access Point

   - Main controller and RFID reader

- The 3 Clients connect to the Central's Wi-Fi.

- Each board reads an RFID tag.

- When the correct UID is scanned:

    - The client sends "1" to the Central via UDP.

    - If all 4 devices (including the Central) have sent "1", a success signal is triggered.

- Pressing the physical reset button on the Central sends "0" to all clients to reset the state and allow new scans.




<h2>🧰 Components Used</h2>

- Wemos D1 Mini (ESP8266) :	4

  
- MFRC522 RFID Module :	4


- RFID Key Tags/Cards :	4


- AMS1117 3.3V Regulator :	4


- XL4015 5V Reg :	1


- Push Button :	1


- 12V 5A Power Supply


<h2>📶 Communication Architecture</h2>
- UDP Protocol

- Central IP: 192.168.4.1

- UDP Port: 4210

- Clients send status via UDP

- Central sends reset commands to clients

<h2/📁 Files Provided : </h2>


- central_code.ino — Code for the central board with RFID reader and reset logic

- clients_code.ino — Code for clients


- Media/wiring_diagram.png — Full wiring diagram of the system

- Media/demo_video.mp4 — Demo video showing the project in action


https://github.com/user-attachments/assets/37af8e38-01d0-454a-96ad-15f6f2fc6734






