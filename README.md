# Star Trek Reliant - Kirk vs Kahn

An Arduino-based project that replays the iconic Star Trek II scene where Kirk and Khan engage in a tense space battle.

## 💡 Project Purpose
A 3D-printed ship model houses the system. LEDs and sound effects recreate shield activity and phaser blasts for a cinematic re-enactment.

## 🔧 Features

- **LED lights** Representing both the shield display on the console and the Relient ship
- **Push button** To move the action along (in certain modes)
- **Speaker** To listen to the Kirk and Kahn war with words
- **EEPROM** To remember the volume after restart
- **Power** Supplied by wall adapter

## 🧰 Hardware

- **Microcontroller**: Arduino Nano (or compatible)
- **Button**: Connected to digital pin 3 with internal pull-up
- **LED Strip**: WS2812 or compatible RGB LEDs
- **Audio Module**: DFPlayer Mini + microSD card
- **Speaker**: 8Ω, 1–3W
- **Perf Board**: circuit soldered on board

## 📁 File Structure

| File               | Description                                  |
|--------------------|----------------------------------------------|
| `main.cpp`         | Main application logic and state machine     |
| `ButtonHandler.*`  | Handles button input, debouncing, press types |
| `LEDStrip.*`       | Controls ship and shield LED patterns        |
| `Sound.*`          | Manages audio playback and volume control    |
| `Global.*`         | Holds constants, pin definitions, and state  |

## 🧪 Behavior Summary

| Action                         | Effect                             |
|--------------------------------|-------------------------------------|
| Short press (Normal mode)      | Advance to next scene segment              |
| Long press (Normal mode)       | Enter volume setting mode           |
| Short press (SET_VOLUME)       | Increase volume, green LED flash    |
| Long press (SET_VOLUME)        | Decrease volume, red LED flash      |
| Inactivity (SET_VOLUME)        | Exit volume mode                    |       |

## ⚙️ Configuration

There are **two versions** of this project:
- **Automatic Playback** – Scene runs start to finish
- **Interactive Mode** – Requires button input to progress

## 🔄 Future Enhancements
- Add support for multiple audio tracks or scenes
- Implement serial command interface for remote control/debugging
- Add LED feedback to improve volume level visually

## 📸 Preview

_When the prototype is avaliable a picture will appear here._


## 📜 License

This is a personal project, but feel free to fork and modify. No warranty provided.



