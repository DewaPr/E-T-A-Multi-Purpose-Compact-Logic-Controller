# Multi-Purpose Compact Logic Controller (MPCLC)

![Hardware: EasyEDA](https://img.shields.io/badge/Hardware-EasyEDA-blue)
![Firmware: C/C++](https://img.shields.io/badge/Firmware-C%2FSTM32_HAL-green)
![Status: Prototype Phase](https://img.shields.io/badge/Status-Prototype-orange)

A robust, fault-tolerant Electronic Control Unit (ECU) designed for high-noise industrial and automotive environments. This repository contains the mixed-signal schematic architecture, PCB layout strategies, and bare-metal STM32 firmware for a hybrid-diagnostic control module.

## System Overview

This project bridges the gap between high-voltage power electronics and precision microcontrollers. It features a heavy-duty 30A low-side drive, heavily protected analog/digital front ends, and a custom Hybrid ASCII/Hex UART communication protocol optimized for both human debugging and high-speed PLC data streaming.

### Key Hardware Features
* **Cascaded Power Tree:** 9-36V input regulated via a 400kHz TI LMR51606 Synchronous Buck (5.0V), cascaded into a Richtek RT9193 Ultra-Low Noise LDO (70dB PSRR) for pristine 3.3V MCU ADC references.
* **Transient & Fault Protection:** * "Crowbar" 5.5V TVS + PTC fuse architecture for external sensor excitation.
  * BAT54S Schottky clamping for 0-5V analog ADCs.
* **Galvanic Isolation:** * Digital inputs isolated via PC817 optocouplers.
  * High-speed UART interface isolated utilizing the TI ISO7721 digital magnetic isolator to prevent edge-rate degradation and ground loops.
* **Heavy-Duty Output Stage:** 30A N-Channel MOSFET low-side switch featuring floating gate-drive opto-isolation and an active SMCJ36A TVS kickback-clamping topology, strictly eliminating external flyback dependence.
* **Star-Grounding Topology:** Layout utilizes physical Net Ties to isolate `GND_LOGIC` from `GND_POWER` at a single point, preventing 30A return currents from inducing logic-level ground bounce.

### Key Firmware Features (Hybrid UART Engine)
The STM32 firmware features a custom UART parser utilizing Hardware DMA and IDLE Line Detection. It actively routes incoming packets to either a human-readable ASCII terminal or a tightly packed hexadecimal machine-to-machine interface.

* **ASCII Mode:** Allows field technicians to execute commands (e.g., `enable output 0`) and read formatted strings directly via a standard serial terminal (PuTTY/TeraTerm).
* **Binary Packet Mode:** Packs 12 bits of state logic (Input Modes, Digital States, Output States) and four 16-bit Analog values into a highly optimized 13-byte array.
* **Mathematical Integrity:** Implements a custom multi-byte CRC-8 (Polynomial `0x07`) check over the binary payload to guarantee data integrity in high-EMI factory environments.

---

## 📂 Repository Structure

```text
├── Hardware/
│   ├── Schematics/          # EasyEDA Schematic PDFs and source files
│   ├── BOM/                 # Bill of Materials (.csv) with manufacturer part numbers
│   └── Layout/              # Gerber files, drill files, and pick & place data
├── Firmware/
│   ├── Core/Src/            # Main application logic (main.c, stm32g0xx_it.c)
│
