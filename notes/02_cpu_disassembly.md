# Disassembly

Here we will implement a disassembly tool to visualize the state of the CPU and its RAM

## A quick reminder

### Relation between CPU and RMU

The **CPU** is the unit that execute instructions from the **RAM**, manage registers (X, Y, Accumulator, Stack_pointer, program_counter, status Register)

RAM is simply a big array of 64 KB (0x0000 → 0xFFFF) that the CPU can address.

RAM :

* Holds machine instructions (opcodes)
* Holds the data manipulated by programs
* Can also include reserved zones for memory-mapped peripherals (e.g. controllers, the PPU in the NES).


They’re connected by a bus:

* Address bus (16 bits): the CPU puts an address (0x0000–0xFFFF) on it.
* Data bus (8 bits): transfers the corresponding byte.
* Control signals: specify whether it’s a read or a write.

Pseudo-code example:

CPU wants to execute an instruction
→ PC = 0x8000
→ Puts 0x8000 on the address bus
→ Requests a "read"
→ RAM returns the value stored at 0x8000 (e.g. 0xA9 = LDA #imm)
→ CPU interprets the instruction and continues


Conversely, if the CPU executes STA $3000, A:

→ Puts 0x3000 on the address bus
→ Places the contents of register A on the data bus
→ Activates the "write" signal
→ RAM stores this value at address 0x3000

### Opcodes and clock

Instructions are loaded in RAM. Each time the `clock()` function is called, the CPU, if it has no instruction in process read from the ram at `program_counter` position
the byte of the intruction.

This Byte (8bits) is the index of the next instruction to be processed and have its reference in an array called `opcodes`.
At each index of this array, we have the instruction to be performed, the addressing mode (How the parameter will be passed) and the number of clock cycles needed

We call the function linked to the addressing mode to compute the address at which the parameter is on the ram and put it into `_addr_abs` and then we `fetch()` it (We read) from the **RAM** at the `_addr_abs`

Then we execute the instruction

After that we increment the `program_counter` and `clock` to the following instruction on the RAM
