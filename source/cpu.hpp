#pragma once

#include <cstdint>

class MemoryBus;

union StatusFlags {
	struct {
		uint8_t C : 1;
		uint8_t Z : 1;
		uint8_t I : 1;
		uint8_t D : 1;
		uint8_t B : 1;
		uint8_t U : 1;
		uint8_t V : 1;
		uint8_t N : 1;
	};
	uint8_t Byte;
};

enum AddressingMode {
	ADDRESSING_IMMEDIATE,
	ADDRESSING_IMPLIED,
	ADDRESSING_ZEROPAGE,
	ADDRESSING_ZEROPAGE_X,
	ADDRESSING_ZEROPAGE_Y,
	ADDRESSING_RELATIVE,
	ADDRESSING_ABSOLUTE,
	ADDRESSING_ABSOLUTE_X,
	ADDRESSING_ABSOLUTE_Y,
	ADDRESSING_INDIRECT_X,
	ADDRESSING_INDIRECT_Y
};

class CPU {
private:
	uint16_t PC;
	uint8_t SP;
	uint8_t A, X, Y;
	StatusFlags Status;

	uint16_t AddressAbsolute = 0x0000;
	int8_t AddressRelative = 0x00;
	AddressingMode CurrentAddressingMode;

	MemoryBus* Bus = nullptr;

	uint8_t Cycles = 0;

	uint8_t ReadByte(uint16_t address);
	uint16_t ReadWord(uint16_t address);
	void WriteByte(uint16_t address, uint8_t value);
	void WriteWord(uint16_t address, uint16_t value);

	// Addressing modes
	void ADDR_IMM(); void ADDR_IMP(); void ADDR_ZP0(); void ADDR_ZPX();
	void ADDR_ZPY(); void ADDR_REL(); void ADDR_ABS(); void ADDR_ABX();
	void ADDR_ABY(); void ADDR_IND(); void ADDR_IZX(); void ADDR_IZY();

	// Access opcodes
	void OPCODE_LDA(); void OPCODE_STA(); void OPCODE_LDX(); void OPCODE_STX();
	void OPCODE_LDY(); void OPCODE_STY();

	// Transfer opcodes
	void OPCODE_TAX(); void OPCODE_TAY(); void OPCODE_TXA(); void OPCODE_TYA();

	// Arithmetic opcodes
	void OPCODE_ADC(); void OPCODE_SBC(); void OPCODE_INC(); void OPCODE_DEC();
	void OPCODE_INX(); void OPCODE_DEX(); void OPCODE_INY(); void OPCODE_DEY();

	// Shift opcodes
	void OPCODE_ASL(); void OPCODE_LSR(); void OPCODE_ROL(); void OPCODE_ROR();

	// Bitwise opcodes
	void OPCODE_AND(); void OPCODE_ORA(); void OPCODE_EOR(); void OPCODE_BIT();

	// Compare opcodes
	void OPCODE_CMP(); void OPCODE_CPX(); void OPCODE_CPY();

	// Branch opcodes
	void OPCODE_BCC(); void OPCODE_BCS(); void OPCODE_BEQ(); void OPCODE_BNE();
	void OPCODE_BPL(); void OPCODE_BMI(); void OPCODE_BVC(); void OPCODE_BVS();

	// Jump opcodes
	void OPCODE_JMP(); void OPCODE_JSR(); void OPCODE_RTS(); void OPCODE_BRK();
	void OPCODE_RTI();

	// Stack opcodes
	void OPCODE_PHA(); void OPCODE_PLA(); void OPCODE_PHP(); void OPCODE_PLP();
	void OPCODE_TXS(); void OPCODE_TSX();

	// Flag opcodes
	void OPCODE_CLC(); void OPCODE_SEC(); void OPCODE_CLI(); void OPCODE_SEI();
	void OPCODE_CLD(); void OPCODE_SED(); void OPCODE_CLV();

	// Other opcodes
	void OPCODE_NOP();

public:
	CPU();

	void ConnectBus(MemoryBus* bus);
	
	void Reset();
	void Execute();
};