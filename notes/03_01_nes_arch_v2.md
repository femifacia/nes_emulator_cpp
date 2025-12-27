# A correction on the NES Arch

## Ram Addressable range and Mirrorring

The Ram in fact has only **2KB** of memory with a range starting from **0x0000** to **0x1FFF**.  But **0x1FFF** is **8191** so you could say that there is **4X** more space
left BUT there is the special trick: **mirrorring** 

**Mirrorring** is a principle that duplicate **2KB** of memory available on three other semgents. Ranges of the memory are reapting. If I modify The 1Kb of memory, it modify the 3Kb, the 5KB and the 7th KB of memory. To be more precise, the first 2KB of RAM is mapped on the hardware. Each 2KB segments of the RAM is mapped on this original segment linked to the Hardware

So, modifying box 455 is the same as modifying box 2455 and 4455 and 6455. Same for reading from. This will be handled by a bitwise logic and (modulo)

## PPU

Picture process Unit. It has its own bus. Attached to this bus, there is a **8KB addressable range** that reflects the **ppattern memory**. It stores what the graphics will look like (sprite etc ..). Like the **CPU** has it **RAM** going from **0x0000** to **0x1FFF**, the **PPU** has its **Pattern memory** also going from **0x0000** to **1xFFFF**

There is an **additional** dedicated **RAM** from **0x2000** to **0x2FFF**: **The nametable**. It is a 2 dimensional array that stores the IDs of which pattern to show in the background.

The PPU has a small RAM attached on it, **The PALETTES** that describe which **color** should be displayed on the screen, when you combine the **sprites** and the **background**. It start from **0x3000** to **0x3FFF**

The PPU does not has the same addressable range addresse than the CPU. Its is much lower.

The PPU contain the graphical informations while the cpu the logical calculs.

Here we will implement the PPU **2C02** created by **Ricoh**. **2C02** is the model of the chip.

To address the PPU, the CPU can target 8 locations, **8 registers**, into its own ram. They are dedicated to the ppu


| CPU Adress  | PPU Register |
| ----------- | ------------ |
| `$2000`     | PPUCTRL      |
| `$2001`     | PPUMASK      |
| `$2002`     | PPUSTATUS    |
| `$2003`     | OAMADDR      |
| `$2004`     | OAMDATA      |
| `$2005`     | PPUSCROLL    |
| `$2006`     | PPUADDR      |
| `$2007`     | PPUDATA      |

The CPU, **CANNOT** read the PPU's VRAM directly and **CANNOT** access sprites directly. It must go through these registers


Example:

To write to VRAM:

* write the address via $2006
* write the data via $2007

To read the PPU status:
* read $2002 (PPUSTATUS)

**The PPU is like a standalone coprocessor, not a conventional RAM.**

Those adresses are being **mirrorred**

* $2000 - $2007  ← PPU registers
* $2008 - $3FFF  ← mirrors (repetitions)



## The program ROM (cartridge)

The program is store on the cartridge. The Program ROM is adressed via the CPU BUS and is adress range is from **0x4020** to **0xFFFF**, the entire second half range of the CPU Bus. The cartridge, does not simply contain the **program**, it also contain all the graphical informations required to render the game. So, the **Pattern table** is aso stocked on the cartridge. Here on our implementation, we will assume for simplicity that the **Nametable** is stocked on the **NES** itself. Technically some of the name table object can be stored in the cartridge

## Mappers

Sometimes the ammount of program data required, exceeds the adressable range of the CPU, so come the **Mapper** system. It allow different bands of the ROM to be connected to the CPU BUS. So when this ranges are requested, they are throught the same range of the program ROM. The **Mapper** can be **written** or **read** while the ROM can only be **read**



Here is how those components are linked


![image](../images/arch_v2.png "arch v2")


## More about the PPU registers

| CPU Address | Register Name | Read / Write  | Purpose                    |
| ----------- | ------------- | ------------- | -------------------------- |
| `$2000`     | **PPUCTRL**   | Write         | PPU control settings       |
| `$2001`     | **PPUMASK**   | Write         | Rendering enable & effects |
| `$2002`     | **PPUSTATUS** | Read          | PPU status flags           |
| `$2003`     | **OAMADDR**   | Write         | OAM address (sprites)      |
| `$2004`     | **OAMDATA**   | Read/Write    | Sprite RAM access          |
| `$2005`     | **PPUSCROLL** | Write (twice) | Scroll position            |
| `$2006`     | **PPUADDR**   | Write (twice) | VRAM address               |
| `$2007`     | **PPUDATA**   | Read/Write    | VRAM data access           |

### `$2000` — PPUCTRL (PPU Control Register)

**Write-only** register. It controls high-level PPU behavior.

``` Plain text
7 6 5 4 3 2 1 0
N N S B P H I I

```

| Bit | Name                 | Meaning                |
| --- | -------------------- | ---------------------- |
| 7   | NMI Enable           | Generate NMI at VBlank |
| 6   | PPU Master           | Always 0 on NES        |
| 5   | Sprite Size          | 0 = 8×8, 1 = 8×16      |
| 4   | BG Pattern Table     | 0 = $0000, 1 = $1000   |
| 3   | Sprite Pattern Table | 0 = $0000, 1 = $1000   |
| 2   | VRAM Increment       | 0 = +1, 1 = +32        |
| 1–0 | Base Nametable       | Select nametable       |


This register defines how rendering behaves.

### `$2001` — PPUMASK (PPU Mask Register)

Write-only Register.

Controls rendering visibility and color effects.
```
7 6 5 4 3 2 1 0
B G R S B S G M
```

| Bit | Meaning                         |
| --- | ------------------------------- |
| 7–5 | Color emphasis (RGB)            |
| 4   | Show sprites                    |
| 3   | Show background                 |
| 2   | Show sprites (left 8 pixels)    |
| 1   | Show background (left 8 pixels) |
| 0   | Greyscale mode                  |


It is Used to turn rendering on/off and apply visual effects.

### `$2002` — PPUSTATUS (PPU Status Register)

**Read-only**

Reading this register also resets internal PPU latches.
```
7 6 5 4 3 2 1 0
V S O - - - - -
```
| Bit | Meaning         |
| --- | --------------- |
| 7   | VBlank flag     |
| 6   | Sprite 0 hit    |
| 5   | Sprite overflow |
| 4–0 | Unused          |


⚠️ **WARNING**, Reading $2002:

* Clears the VBlank flag
* Resets the $2005/$2006 write toggle

### `$2003` — OAMADDR (OAM Address)

**Write-only**

Sets the address into **Object Attribute Memory (OAM)**

OAM holds sprite data (256 bytes total)

### `$2004` — OAMDATA (OAM Data)

**Read / Write**

Reads or writes sprite data at the OAM address

Address auto-increments after access

Each sprite uses 4 bytes:

* Y position
* Tile index
* Attributes
* X position

### `$2005` — PPUSCROLL (Scroll Register)

Write-only, two writes required

* First write → horizontal scroll (X)
* Second write → vertical scroll (Y)

Uses an internal latch toggle.
```
PPUSCROLL = X; // first write
PPUSCROLL = Y; // second write
```

👉 Controls camera scrolling.

### `$2006` — PPUADDR (VRAM Address Register)

**Write-only**, two writes required

* First write → high byte
* Second write → low byte

Sets the 14-bit VRAM address.
```
PPUADDR = high;
PPUADDR = low;
```

Used before accessing $2007.

### `$2007` — PPUDATA (VRAM Data Register)

**Read / Write**

Reads or writes VRAM at the address set by `$2006`

Address auto-increments by 1 or 32 (from $2000 bit 2)

⚠️ Reads are buffered:

* First read returns old data (except palette memory)
* Second read returns correct data


## VRAM In a nutshell


VRAM stands for Video RAM.

In the context of the NES, VRAM is the memory used by the PPU (Ricoh 2C02) to store graphics-related data, such as:

* Background tile maps
* Attribute tables (palette selection per area)
* Background palettes
* Sprite palettes

⚠️ Important:
*VRAM is not CPU RAM.
*It is addressed and managed by the PPU, not directly by the CPU.

It has a **16kb** size in the nes and as Address range: `0x0000 – 0x3FFF (PPU address space)`

This is the **PPU Addressing Space (0x0000–0x3FFF)**

```
0x0000–0x0FFF  Pattern Table 0 (tiles, CHR-ROM/RAM)
0x1000–0x1FFF  Pattern Table 1 (tiles, CHR-ROM/RAM)

0x2000–0x23FF  Name Table 0
0x2400–0x27FF  Name Table 1
0x2800–0x2BFF  Name Table 2
0x2C00–0x2FFF  Name Table 3
               (mirrored depending on cartridge wiring)

0x3000–0x3EFF  Mirrors of 0x2000–0x2EFF

0x3F00–0x3F1F  Palette RAM
0x3F20–0x3FFF  Mirrors of palette RAM
```

### Why is VRAM separated from CPU's RAM ?


* The PPU needs high-speed access during rendering
* CPU and PPU run in parallel
* Graphics memory layout is very different from program memory

### Writting into the Vram
Because the CPU cannot directly write and read from the **VRAM**, it uses some registers into its own RAM

| CPU Address | Register | Purpose              |
| ----------- | -------- | -------------------- |
| `$2006`     | PPUADDR  | Set VRAM address     |
| `$2007`     | PPUDATA  | Read/write VRAM data |


```asm

LDA #$20
STA $2006   ; high byte of VRAM address
LDA #$00
STA $2006   ; low byte → address = $2000

LDA #$12
STA $2007   ; write data to VRAM[$2000]


```