# nes_emulator_cpp

A nes emulator in C++

# Overview
This project implements the core of a MOS 6502-based system:

* A CPU class supporting a wide set of opcodes and addressing modes.
* A RAM module emulating the 16-bit address space (0x0000-0xFFFF).
* A simple bus mechanism to connect CPU ↔ RAM (and in the future other devices/peripherals).

The goal is to build a foundation for retro consoles (such as the Nintendo Entertainment System) or for educational/emulation purposes.

# Features

* Implementation of **CPU registers** **(A, X, Y, SP, PC, Status)**
* Fetch-decode-execute cycle emulation
* Memory read/write abstraction via a Bus class
* Support for key 6502 addressing modes (Immediate, Zero Page, Absolute, Indirect, etc.)
* Unit-testable core architecture
* **Visualization Tools** for **Ram** and **CPU** components in **ncurses**,**X11**, and **console mode**

# Architecture

This is how component interact

![image](./images/arch_v1.png "arch v1")

The CPU uses the BUS interface to read or write to memory addresses.

The RAM class handles the memory array behind the scenes.

In future, the BUS may route reads/writes to ROM, PPU registers, controllers, etc.


## How to compile and run ?

### NES Binary

Go at the root of the directory and type:

    make re

It will compile the nes emulator exec


### CPU X11 Visualizer

Go at the root of the directory and type:

    make viz_cpu_ram_x11

It will compile the viz binary

# Contribution

Contributions are welcome!
Please follow these guidelines:

* Fork the repository and create a feature branch.
* Write clear, well-documented code and tests.
* Ensure you update documentation if you modify or add behaviour.
* Submit a pull request with a description of your changes and why they help.

# Acknowledgements

[OneLoneCoder](https://github.com/OneLoneCoder) for the olc6502 inspiration

The NESDev community for technical references