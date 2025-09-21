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

## What is going on into the X11 disassembly

The goal of these tools are just to print the content of the CPU Registers and RAM each time an instruction is exectuted

Let's breaking down:

We are on `OnUserCreate()` in [main_test.cpp]('../tools/cpu_and_ram_x11_visualizer/main_test.cpp')

This is an initialization function, called once when the demo starts.

---

### 1. The program in assembly

In the comment, we see a small program written in **6502 assembly**:


```asm
    *=$8000        ; The assembler places the program at address $8000
    LDX #10        ; X = 10
    STX $0000      ; $0000 = 10
    LDX #3         ; X = 3
    STX $0001      ; $0001 = 3
    LDY $0000      ; Y = [$0000] = 10
    LDA #0         ; A = 0
    CLC            ; Clear Carry
loop:
    ADC $0001      ; A = A + [$0001] (so add 3 each time)
    DEY            ; Y = Y - 1
    BNE loop       ; While Y ≠ 0, loop
    STA $0002      ; When finished: store A into $0002
    NOP            ; NOP (empty instructions for padding)
    NOP
    NOP
```

It's the program we want to load on the RAM and execute

This program basically computes `10 * 3` and stores the result (`30`) in memory at `$0002`.
It’s a small **multiplication by repeated addition** loop.

The `*=$8000` indicates we want our `program_counter` to start at 0x8000 and the filling of the ram with ours bytes from assembly to start at

---

### 2. Conversion to hexadecimal

The next block:

```cpp
std::stringstream ss;
ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
```

This is the same program, but translated into **6502 opcodes**:

Each byte is an opcode indicating the instruction to be called, the addressing mode, the clock cycles through `_opcode` array

* `A2 0A` → `LDX #10`
* `8E 00 00` → `STX $0000`
* … etc.
* `EA` → `NOP`

Each byte will be loaded on the ram starting to the offset

---

### 3. Loading the program into memory

```cpp
uint16_t nOffset = 0x8000;
while (!ss.eof())
{
    std::string b;
    ss >> b;
    nes._ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
}
```

Here:

* We start at address **\$8000** (as defined by `*=$8000`).
* We read each hexadecimal byte from the string.
* We store them in the NES’s simulated RAM (`nes._ram`), beginning at `$8000`.

 Result: the assembly program is now loaded into the NES’s memory.

---

### 4. Reset vector

```cpp
nes._ram[0xFFFC] = 0x00;
nes._ram[0xFFFD] = 0x80;
```

On a real 6502, when it **resets**, it reads the reset vector from `$FFFC/$FFFD`.
Here, we put `0x8000` Because this is where we were supposed to start

So after a reset, the CPU will begin execution at `$8000`.

(If we wanted to use IRQ or NMI, we’d also need to fill their vectors at the proper addresses.)

---

### 5. Disassembly

```cpp
mapAsm = nes._cpu.disassemble(0x0000, 0xFFFF);
```

We ask the CPU to **disassemble the whole memory range**.
This generates a map `address → human-readable instruction`, which is useful to display the source code in the debug window.

---

### 6. Resetting the CPU

```cpp
nes._cpu.reset();
```

We reset the CPU:

* Registers are restored to their default state.
* The PC is set to the reset vector (0x8000).
* It’s ready to run our program.

---

### In nutshell

In `OnUserCreate()`:

1. We write a small 6502 assembly program.
2. We translate it into machine code bytes and load it into RAM at `$8000`.
3. We set the reset vector so the CPU starts at `$8000`.
4. We generate a disassembly map for debugging.
5. We reset the CPU.

After that, the CPU is ready to execute the small program (which computes `10 * 3`).

---

