#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
class Bus;

class CPU_6502
{
private:
    /* data */

    Bus *_bus;

    uint8_t _accumulator_register; //accumulator register
    uint8_t _x; //x register
    uint8_t _y; //y register
    uint8_t _stack_pointer; // stack pointer: Point to location on the bus
    uint16_t _program_counter;
    uint8_t _status_register; // status register


public:
    /// @brief is an enumeration of the bits on the status register
    /// As a reminder, (8 bits) Status Register (contains bit which let us interogate about the state of the CPU) 
    /// for ex: Was the last result equal to 0 ?Has there been a carry operation, Instruct the CPU to do Thng (Interruptions etc....)
    enum FLAGS6502 { 
        C =  (1 << 0), // carry bit which is set when there were an operation and a carry was needed
        Z =  (1 << 1), // Zero, set whenever the result of an operation equal to 0
        I =  (1 << 2), //  Disable Interrupts
        D =  (1 << 3), // Decimal Mode (won't be used)
        B =  (1 << 4), // Break
        U =  (1 << 5), // Unused
        V =  (1 << 6), // Overflow
        N =  (1 << 7)  // Negative

    };

private:
    // function to access status register flag
    void Setflag(FLAGS6502 f, bool v);
    uint8_t GetFlag(FLAGS6502 f);
    // depending on the adressing mode we might want to read from a particular adress
    uint16_t _addr_abs;
    // when there is a JMP (JUMP), ptr jump to a relative address 
    uint16_t _addr_relative;

    // current opcode we are reading
    uint8_t _opcode;
    // number of cycles left
    uint8_t _cycles;

public:
    CPU_6502(/* args */);
    ~CPU_6502();

    /// @brief connect to the BUS
    /// @param bus 
    void ConnectBus(Bus *bus);


    /// @brief write data on the bus
    /// @param addr 
    /// @param data 
    void write(uint16_t addr, uint8_t data);

    /// @brief read data from the bus
    /// @param addr 
    /// @return 
    uint8_t read(uint16_t addr);

    
    // addressing mode
    uint8_t ABS();
    uint8_t ABX();
    uint8_t ABY();
    uint8_t IMM();
    uint8_t IMP();
    uint8_t IND();
    uint8_t IZX();
    uint8_t IZY();
    uint8_t REL();
    uint8_t ZP0();
    uint8_t ZPX();
    uint8_t ZPY();

    //opcodes
    uint8_t ADC();
    uint8_t AND();
    uint8_t ASL();
    uint8_t BCC();
    uint8_t BCS();
    uint8_t BEQ();
    uint8_t BIT();
    uint8_t BMI();
    uint8_t BNE();
    uint8_t BPL();
    uint8_t BRK();
    uint8_t BVC();
    uint8_t BVS();
    uint8_t CLC();
    uint8_t CLD();
    uint8_t CLI();
    uint8_t CLV();
    uint8_t CMP();
    uint8_t CPX();
    uint8_t CPY();
    uint8_t DEC();
    uint8_t DEX();
    uint8_t DEY();
    uint8_t EOR();
    uint8_t INC();
    uint8_t INX();
    uint8_t INY();
    uint8_t JMP();
    uint8_t JSR();
    uint8_t LDA();
    uint8_t LDX();
    uint8_t LDY();
    uint8_t LSR();
    uint8_t NOP();
    uint8_t ORA();
    uint8_t PHA();
    uint8_t PHP();
    uint8_t PLA();
    uint8_t PLP();
    uint8_t ROL();
    uint8_t ROR();
    uint8_t RTI();
    uint8_t RTS();
    uint8_t SBC();
    uint8_t SEC();
    uint8_t SED();
    uint8_t SEI();
    uint8_t STA();
    uint8_t STX();
    uint8_t STY();
    uint8_t TAX();
    uint8_t TAY();
    uint8_t TSX();
    uint8_t TXA();
    uint8_t TXS();
    uint8_t TYA();


    uint8_t XXX(); // UNKNOWN_OPCODE

    // This structure is to debbug and reverse engineer

    struct INSTRUCTION {
        
        /// @brief  the name of the instruction
        std::string name;
        /// @brief the ptr to the opcode
        uint8_t(CPU_6502::*operate)(void) = nullptr;
        /// @brief ptr to the address mode
        uint8_t(CPU_6502::*addrmode)(void) = nullptr;
        /// @brief nmbr of cycles the instruction requires to execute
        uint8_t cycles = 0;
        

        /* data */
    };

    std::vector<INSTRUCTION> _lookup;
    


    void clock();

    //signals
    // all these 03 functions can occur every time and can interupt the work of the cpu.
    // They are asynchronous
    void reset();
    void interupt_required(); //irq (interupr required signal) can be ignored wether the interrupt enable flag is setle
    void non_masquable_interupt_request(); // can never been disabled

    /// @brief To fetch data for the cpu
    /// @return 
    uint8_t fetch();

    uint8_t _fetched;


    

};

