On the cartridge, there is a **program memory** (PRG) and a **Pattern memory** (character memory, CHR Memory).

The **PPU** access **CHR MEMORY** directly. The CHR memory is called like this because each tile is a one  character of 8 pixel width, 8 pixel height and a 4 possibility of color (2 bits). Each tile has so a size of 8 x 8 x 2bits = 128 bits = 16 bytes. Each tile is save as 2 bitplane (A bitplane is a layer of bits that represents one bit of color information for all pixels.)

```
Bytes 0–7   → Bitplane 0 (LSB)
Bytes 8–15  → Bitplane 1 (MSB)

```

## A tile ?

A small fixed-size graphic block of 8×8 pixels used as the basic building unit for backgrounds and sprites.

Think of tiles as graphic “letters” that the PPU assembles to build the screen, just like characters form words. Tiles live in pattern memory, also called:

CHR memory (Character Memory)

## Memory organization

The cartridge may consists of many different memories and physically differents chips. OR it may consist of one LARGE Memory but with a **Mapper** which will divide it and make associations. The Cartridge is connected to the PPU and the CPU.

When the PPU want to access to a location on the **PRG Memory**, it is the Mapper which has the convenience  to define what place on the **PRG Memory** will be referring by this requested address.

You can also assume that the **PRG Memory** and the **CHR Memory** can be divided into **"banks"** or **"Chunks"** of subsections

A **real cartridge** often has **headers** containing the requered **PRG Memory** size and the required **CHR memory**

To sum up, the **CPU** configure the **Mapper** and the Mapper translate the requesting addresses

## iNes File format

iNES was an early NES emulator developed by Marat Fayzullin. Its most lasting contribution to the NES scene was its popularization of the iNES ROM file format and mapper numbering system.

This file format is commonly referred to as the iNES file format/iNES header format. The file extension is .nes, so it is sometimes referred to as the .nes file format, and files in it as .nes files. Now that the NES 2.0 file format exists, which uses the same .nes extension, a .nes file/the .nes file format could mean the iNES file format or NES 2.0 format, so the full format names should be used where the differences in the formats are relevant, like specifications or format support.

An iNES file consists of the following sections, in order:

* Header (16 bytes)
* Trainer, if present (0 or 512 bytes)
* PRG ROM data (16384 * x bytes)
* CHR ROM data, if present (8192 * y bytes)
* PlayChoice INST-ROM, if present (0 or 8192 bytes)
* PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)
* Some ROM-Images additionally contain a 128-byte (or sometimes 127-byte) title at the end of the file.

The format of the header is as follows:


|  Bytes  |	Description                                                                                   |
|---------|-----------------------------------------------------------------------------------------------|
|  0-3	  | Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)                         |
|  4	  | Size of PRG ROM in 16 KB units                                                                |
|  5	  | Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)                          |
|  6	  | Flags 6 - Mapper, mirroring, battery, trainer                                                 |
|  7	  | Flags 7 - Mapper, VS/Playchoice, NES 2.0                                                      |
|  8	  | Flags 8 - PRG-RAM size (rarely used extension)                                                |
|  9	  | Flags 9 - TV system (rarely used extension)                                                   |
|  10	  | Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)                    |
|  11-15  |	Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)|


After that we read the content of the **PRG Memory** and the **CHR Memory**

Don't forget we know the size of the PRG and CHR thanks to the header (We have the number of banks for each memory band)

**REMEMBER** We cannot access the content of the Cartridge directly. We have to pass through some **MAPPERS** and there is a bunch of them !

## More about Mappers

NES games come in cartridges, and inside of those cartridges are various circuits and hardware. Different games use different circuits and hardware, and the configuration and capabilities of such cartridges is commonly called their mapper. Mappers are designed to extend the system and bypass its limitations, such as by adding RAM to the cartridge or even extra sound channels. More commonly though, mappers are designed to allow games larger than 40K to be made.

The term "mapper" arises from the concept of memory mapping: translating memory hardware into the CPU's and PPU's address spaces. A memory map describes which addresses correspond to which physical locations of memory.

If you want to learn more about them, you have [this link](https://www.nesdev.org/wiki/Mapper)

We will first focus on the [Mapper 00](https://www.nesdev.org/wiki/NROM).

It is an interesting blank template. It can come with 16kb or 32kb of PRG memory. If it is 32k we have 2 banks ok memory for this part ottherwise, the CPU $C000-$FFFF is just a mirror of the main 16kb bank

