#include "cpu.hpp"
#include "memory_bus.hpp"

CPU::CPU() {
	this->A = 0x00;
	this->X = 0x00;
	this->Y = 0x00;

	this->PC = 0x0000;
	this->SP = 0x00;

	this->Status.C = 0;
	this->Status.Z = 0;
	this->Status.I = 1;
	this->Status.D = 0;
	this->Status.B = 0;
	this->Status.U = 1;
	this->Status.V = 0;
	this->Status.N = 0;
}

void CPU::ConnectBus(MemoryBus* bus) {
	this->Bus = bus;
}

void CPU::Reset() {
	this->PC = this->ReadWord(0xFFFC);
	this->SP -= 0x03;
}

void CPU::Execute() {
	this->Cycles = 0;

	uint8_t opcode = this->ReadByte(this->PC);

	switch (opcode) {
		case 0xA9: ADDR_IMM(); OPCODE_LDA(); break;
		case 0xA5: ADDR_ZP0(); OPCODE_LDA(); break;
		case 0xB5: ADDR_ZPX(); OPCODE_LDA(); break;
		case 0xAD: ADDR_ABS(); OPCODE_LDA(); break;
		case 0xBD: ADDR_ABX(); OPCODE_LDA(); break;
		case 0xB9: ADDR_ABY(); OPCODE_LDA(); break;
		case 0xA1: ADDR_IZX(); OPCODE_LDA(); break;
		case 0xB1: ADDR_IZY(); OPCODE_LDA(); break;
	
		case 0x85: ADDR_ZP0(); OPCODE_STA(); break;
		case 0x95: ADDR_ZPX(); OPCODE_STA(); break;
		case 0x8D: ADDR_ABS(); OPCODE_STA(); break;
		case 0x9D: ADDR_ABX(); OPCODE_STA(); break;
		case 0x99: ADDR_ABY(); OPCODE_STA(); break;
		case 0x81: ADDR_IZX(); OPCODE_STA(); break;
		case 0x91: ADDR_IZY(); OPCODE_STA(); break;
	
		case 0xA2: ADDR_IMM(); OPCODE_LDX(); break;
		case 0xA6: ADDR_ZP0(); OPCODE_LDX(); break;
		case 0xB6: ADDR_ZPY(); OPCODE_LDX(); break;
		case 0xAE: ADDR_ABS(); OPCODE_LDX(); break;
		case 0xBE: ADDR_ABY(); OPCODE_LDX(); break;
	
		case 0x86: ADDR_ZP0(); OPCODE_STX(); break;
		case 0x96: ADDR_ZPY(); OPCODE_STX(); break;
		case 0x8E: ADDR_ABS(); OPCODE_STX(); break;
	
		case 0xA0: ADDR_IMM(); OPCODE_LDY(); break;
		case 0xA4: ADDR_ZP0(); OPCODE_LDY(); break;
		case 0xB4: ADDR_ZPX(); OPCODE_LDY(); break;
		case 0xAC: ADDR_ABS(); OPCODE_LDY(); break;
		case 0xBC: ADDR_ABX(); OPCODE_LDY(); break;
	
		case 0x84: ADDR_ZP0(); OPCODE_STY(); break;
		case 0x94: ADDR_ZPX(); OPCODE_STY(); break;
		case 0x8C: ADDR_ABS(); OPCODE_STY(); break;
	
		case 0xAA: ADDR_IMP(); OPCODE_TAX(); break;
		case 0xA8: ADDR_IMP(); OPCODE_TAY(); break;
		case 0x8A: ADDR_IMP(); OPCODE_TXA(); break;
		case 0x98: ADDR_IMP(); OPCODE_TYA(); break;
	
		case 0x69: ADDR_IMM(); OPCODE_ADC(); break;
		case 0x65: ADDR_ZP0(); OPCODE_ADC(); break;
		case 0x75: ADDR_ZPX(); OPCODE_ADC(); break;
		case 0x6D: ADDR_ABS(); OPCODE_ADC(); break;
		case 0x7D: ADDR_ABX(); OPCODE_ADC(); break;
		case 0x79: ADDR_ABY(); OPCODE_ADC(); break;
		case 0x61: ADDR_IZX(); OPCODE_ADC(); break;
		case 0x71: ADDR_IZY(); OPCODE_ADC(); break;
	
		case 0xE9: ADDR_IMM(); OPCODE_SBC(); break;
		case 0xE5: ADDR_ZP0(); OPCODE_SBC(); break;
		case 0xF5: ADDR_ZPX(); OPCODE_SBC(); break;
		case 0xED: ADDR_ABS(); OPCODE_SBC(); break;
		case 0xFD: ADDR_ABX(); OPCODE_SBC(); break;
		case 0xF9: ADDR_ABY(); OPCODE_SBC(); break;
		case 0xE1: ADDR_IZX(); OPCODE_SBC(); break;
		case 0xF1: ADDR_IZY(); OPCODE_SBC(); break;
	
		case 0xE6: ADDR_ZP0(); OPCODE_INC(); break;
		case 0xF6: ADDR_ZPX(); OPCODE_INC(); break;
		case 0xEE: ADDR_ABS(); OPCODE_INC(); break;
		case 0xFE: ADDR_ABX(); OPCODE_INC(); break;
	
		case 0xC6: ADDR_ZP0(); OPCODE_DEC(); break;
		case 0xD6: ADDR_ZPX(); OPCODE_DEC(); break;
		case 0xCE: ADDR_ABS(); OPCODE_DEC(); break;
		case 0xDE: ADDR_ABX(); OPCODE_DEC(); break;
	
		case 0xE8: ADDR_IMP(); OPCODE_INX(); break;
		case 0xCA: ADDR_IMP(); OPCODE_DEX(); break;
		case 0xC8: ADDR_IMP(); OPCODE_INY(); break;
		case 0x88: ADDR_IMP(); OPCODE_DEY(); break;
	
		case 0x0A: ADDR_IMP(); OPCODE_ASL(); break;
		case 0x06: ADDR_ZP0(); OPCODE_ASL(); break;
		case 0x16: ADDR_ZPX(); OPCODE_ASL(); break;
		case 0x0E: ADDR_ABS(); OPCODE_ASL(); break;
		case 0x1E: ADDR_ABX(); OPCODE_ASL(); break;
	
		case 0x4A: ADDR_IMP(); OPCODE_LSR(); break;
		case 0x46: ADDR_ZP0(); OPCODE_LSR(); break;
		case 0x56: ADDR_ZPX(); OPCODE_LSR(); break;
		case 0x4E: ADDR_ABS(); OPCODE_LSR(); break;
		case 0x5E: ADDR_ABX(); OPCODE_LSR(); break;
	
		case 0x2A: ADDR_IMP(); OPCODE_ROL(); break;
		case 0x26: ADDR_ZP0(); OPCODE_ROL(); break;
		case 0x36: ADDR_ZPX(); OPCODE_ROL(); break;
		case 0x2E: ADDR_ABS(); OPCODE_ROL(); break;
		case 0x3E: ADDR_ABX(); OPCODE_ROL(); break;
	
		case 0x6A: ADDR_IMP(); OPCODE_ROR(); break;
		case 0x66: ADDR_ZP0(); OPCODE_ROR(); break;
		case 0x76: ADDR_ZPX(); OPCODE_ROR(); break;
		case 0x6E: ADDR_ABS(); OPCODE_ROR(); break;
		case 0x7E: ADDR_ABX(); OPCODE_ROR(); break;
	
		case 0x29: ADDR_IMM(); OPCODE_AND(); break;
		case 0x25: ADDR_ZP0(); OPCODE_AND(); break;
		case 0x35: ADDR_ZPX(); OPCODE_AND(); break;
		case 0x2D: ADDR_ABS(); OPCODE_AND(); break;
		case 0x3D: ADDR_ABX(); OPCODE_AND(); break;
		case 0x39: ADDR_ABY(); OPCODE_AND(); break;
		case 0x21: ADDR_IZX(); OPCODE_AND(); break;
		case 0x31: ADDR_IZY(); OPCODE_AND(); break;
	
		case 0x09: ADDR_IMM(); OPCODE_ORA(); break;
		case 0x05: ADDR_ZP0(); OPCODE_ORA(); break;
		case 0x15: ADDR_ZPX(); OPCODE_ORA(); break;
		case 0x0D: ADDR_ABS(); OPCODE_ORA(); break;
		case 0x1D: ADDR_ABX(); OPCODE_ORA(); break;
		case 0x19: ADDR_ABY(); OPCODE_ORA(); break;
		case 0x01: ADDR_IZX(); OPCODE_ORA(); break;
		case 0x11: ADDR_IZY(); OPCODE_ORA(); break;
	
		case 0x49: ADDR_IMM(); OPCODE_EOR(); break;
		case 0x45: ADDR_ZP0(); OPCODE_EOR(); break;
		case 0x55: ADDR_ZPX(); OPCODE_EOR(); break;
		case 0x4D: ADDR_ABS(); OPCODE_EOR(); break;
		case 0x5D: ADDR_ABX(); OPCODE_EOR(); break;
		case 0x59: ADDR_ABY(); OPCODE_EOR(); break;
		case 0x41: ADDR_IZX(); OPCODE_EOR(); break;
		case 0x51: ADDR_IZY(); OPCODE_EOR(); break;
	
		case 0x24: ADDR_ZP0(); OPCODE_BIT(); break;
		case 0x2C: ADDR_ABS(); OPCODE_BIT(); break;
	
		case 0xC9: ADDR_IMM(); OPCODE_CMP(); break;
		case 0xC5: ADDR_ZP0(); OPCODE_CMP(); break;
		case 0xD5: ADDR_ZPX(); OPCODE_CMP(); break;
		case 0xCD: ADDR_ABS(); OPCODE_CMP(); break;
		case 0xDD: ADDR_ABX(); OPCODE_CMP(); break;
		case 0xD9: ADDR_ABY(); OPCODE_CMP(); break;
		case 0xC1: ADDR_IZX(); OPCODE_CMP(); break;
		case 0xD1: ADDR_IZY(); OPCODE_CMP(); break;
	
		case 0xE0: ADDR_IMM(); OPCODE_CPX(); break;
		case 0xE4: ADDR_ZP0(); OPCODE_CPX(); break;
		case 0xEC: ADDR_ABS(); OPCODE_CPX(); break;
	
		case 0xC0: ADDR_IMM(); OPCODE_CPY(); break;
		case 0xC4: ADDR_ZP0(); OPCODE_CPY(); break;
		case 0xCC: ADDR_ABS(); OPCODE_CPY(); break;
	
		case 0x90: ADDR_REL(); OPCODE_BCC(); break;
		case 0xB0: ADDR_REL(); OPCODE_BCS(); break;
		case 0xF0: ADDR_REL(); OPCODE_BEQ(); break;
		case 0xD0: ADDR_REL(); OPCODE_BNE(); break;
		case 0x10: ADDR_REL(); OPCODE_BPL(); break;
		case 0x30: ADDR_REL(); OPCODE_BMI(); break;
		case 0x50: ADDR_REL(); OPCODE_BVC(); break;
		case 0x70: ADDR_REL(); OPCODE_BVS(); break;
	
		case 0x4C: ADDR_ABS(); OPCODE_JMP(); break;
		case 0x6C: ADDR_IND(); OPCODE_JMP(); break;
	
		case 0x20: ADDR_ABS(); OPCODE_JSR(); break;
		case 0x60: ADDR_IMP(); OPCODE_RTS(); break;
		case 0x00: ADDR_IMP(); OPCODE_BRK(); break;
		case 0x40: ADDR_IMP(); OPCODE_RTI(); break;
	
		case 0x48: ADDR_IMP(); OPCODE_PHA(); break;
		case 0x68: ADDR_IMP(); OPCODE_PLA(); break;
		case 0x08: ADDR_IMP(); OPCODE_PHP(); break;
		case 0x28: ADDR_IMP(); OPCODE_PLP(); break;
		case 0x9A: ADDR_IMP(); OPCODE_TXS(); break;
		case 0xBA: ADDR_IMP(); OPCODE_TSX(); break;
	
		case 0x18: ADDR_IMP(); OPCODE_CLC(); break;
		case 0x38: ADDR_IMP(); OPCODE_SEC(); break;
		case 0x58: ADDR_IMP(); OPCODE_CLI(); break;
		case 0x78: ADDR_IMP(); OPCODE_SEI(); break;
		case 0xD8: ADDR_IMP(); OPCODE_CLD(); break;
		case 0xF8: ADDR_IMP(); OPCODE_SED(); break;
		case 0xB8: ADDR_IMP(); OPCODE_CLV(); break;
	
		case 0xEA: ADDR_IMP(); OPCODE_NOP(); break;
	}
}

uint8_t CPU::ReadByte(uint16_t address) {
	return this->Bus->Read(address);
}

uint16_t CPU::ReadWord(uint16_t address) {
	uint8_t low = this->Bus->Read(address);
	uint8_t high = this->Bus->Read(address + 1);
	
	return (high << 8) | low;
}

void CPU::WriteByte(uint16_t address, uint8_t value) {
	this->Bus->Write(address, value);
}

void CPU::WriteWord(uint16_t address, uint16_t value) {
	uint8_t low = value & 0x00FF;
	uint8_t high = (value >> 8) & 0x00FF;

	this->Bus->Write(address, low);
	this->Bus->Write(address + 1, high);
}

void CPU::ADDR_IMM() {
	this->CurrentAddressingMode = ADDRESSING_IMMEDIATE;
	this->AddressAbsolute = this->PC;
	
	this->PC++;
}

void CPU::ADDR_IMP() {
	this->CurrentAddressingMode = ADDRESSING_IMPLIED;
}

void CPU::ADDR_ZP0() {
	this->CurrentAddressingMode = ADDRESSING_ZEROPAGE;
	this->AddressAbsolute = this->ReadByte(this->PC);

	this->PC++;
	this->Cycles += 1;
}

void CPU::ADDR_ZPX() {
	this->CurrentAddressingMode = ADDRESSING_ZEROPAGE_X;
	this->AddressAbsolute = (this->ReadByte(this->PC) + this->X) & 0xFF;

	this->PC++;
	this->Cycles += 2;
}

void CPU::ADDR_ZPY() {
	this->CurrentAddressingMode = ADDRESSING_ZEROPAGE_Y;
	this->AddressAbsolute = (this->ReadByte(this->PC) + this->Y) & 0xFF;

	this->PC++;
	this->Cycles += 2;
}

void CPU::ADDR_REL() {
	this->CurrentAddressingMode = ADDRESSING_RELATIVE;
	this->AddressRelative = this->ReadByte(this->PC);

	this->PC++;
	this->Cycles += 1;
}

void CPU::ADDR_ABS() {
	this->CurrentAddressingMode = ADDRESSING_ABSOLUTE;
	this->AddressAbsolute = this->ReadWord(this->PC);

	this->PC += 2;
	this->Cycles += 2;
}

void CPU::ADDR_ABX() {
	this->CurrentAddressingMode = ADDRESSING_ABSOLUTE_X;
	
	uint16_t base = this->ReadWord(this->PC);
	uint16_t address = base + this->X;

	this->AddressAbsolute = address;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		this->Cycles += 1;
	}

	this->PC += 2;
	this->Cycles += 3;
}

void CPU::ADDR_ABY() {
	this->CurrentAddressingMode = ADDRESSING_ABSOLUTE_Y;
	
	uint16_t base = this->ReadWord(this->PC);
	uint16_t address = base + this->Y;
	this->AddressAbsolute = address;

	if ((base & 0xFF00) != (address & 0xFF00)) {
		this->Cycles += 1;
	}

	this->PC += 2;
	this->Cycles += 3;
}

void CPU::ADDR_IND() {
	this->CurrentAddressingMode = ADDRESSING_INDIRECT_X;

	uint16_t pointer = this->ReadWord(this->PC);
	this->PC += 2;

	uint8_t low = this->ReadByte(pointer);
	uint8_t high;

	if ((pointer & 0x00FF) == 0x00FF) {
		// If on a page boundary then high byte wraps
		// around to the page start
		high = this->ReadByte(pointer & 0xFF00);
	}
	else {
		high = this->ReadByte(pointer + 1);
	}
	this->AddressAbsolute = (high << 8) | low;

	this->Cycles += 4;
}

void CPU::ADDR_IZX() {
	this->CurrentAddressingMode = ADDRESSING_INDIRECT_X;

	uint8_t pointer = this->ReadByte(this->PC) + this->X;
	this->PC++;

	uint16_t address = this->ReadByte(pointer) | (this->ReadByte((pointer + 1) & 0xFF) << 8);
	this->AddressAbsolute = address;

	this->Cycles += 4;
}

void CPU::ADDR_IZY() {
	this->CurrentAddressingMode = ADDRESSING_INDIRECT_Y;

	uint8_t pointer = this->ReadByte(this->PC);
	this->PC++;

	uint16_t base = this->ReadByte(pointer) | (this->ReadByte((pointer + 1) & 0xFF) << 8);
	uint16_t address = base + this->Y;
	this->AddressAbsolute = address;

	if ((base & 0xFF00) != (address & 0xFF00)) {
		this->Cycles += 1;
	}

	this->Cycles += 4;
}

void CPU::OPCODE_LDA() {
	this->A = this->ReadByte(this->AddressAbsolute);

	this->Status.Z = this->A == 0;
	this->Status.N = (this->A & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_STA() {
	this->WriteByte(this->AddressAbsolute, this->A);
	
	this->Cycles += 1;
}

void CPU::OPCODE_LDX() {
	this->X = this->ReadByte(this->AddressAbsolute);

	this->Status.Z = this->X == 0;
	this->Status.N = (this->X & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_STX() {
	this->WriteByte(this->AddressAbsolute, this->X);
	
	this->Cycles += 1;
}

void CPU::OPCODE_LDA() {
	this->Y = this->ReadByte(this->AddressAbsolute);

	this->Status.Z = this->Y == 0;
	this->Status.N = (this->Y & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_STY() {
	this->WriteByte(this->AddressAbsolute, this->Y);
	this->Cycles += 1;
}

void CPU::OPCODE_TAX() {
	this->X = this->A;

	this->Status.Z = this->X == 0;
	this->Status.N = (this->X & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_TAY() {
	this->Y = this->A;

	this->Status.Z = this->Y == 0;
	this->Status.N = (this->Y & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_TXA() {
	this->A = this->X;

	this->Status.Z = this->A == 0;
	this->Status.N = (this->A & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_TYA() {
	this->A = this->Y;

	this->Status.Z = this->A == 0;
	this->Status.N = (this->A & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_ADC() {
	uint8_t memory = this->ReadByte(this->AddressAbsolute);
	uint16_t result = this->A + memory + this->Status.C;

	this->Status.C = result > 0x00FF;
	this->Status.Z = result == 0x00;
	this->Status.V = (((~(this->A ^ memory)) & (this->A ^ (uint8_t)result)) & 0x80) == 0x80;
	this->Status.N = (result & (1 << 7)) != 0;

	this->A = result & 0xFF;
	
	this->Cycles += 1;
}

void CPU::OPCODE_SBC() {
	uint8_t memory = this->ReadByte(this->AddressAbsolute);
	uint16_t result = this->A - memory - (1  - this->Status.C);

	this->Status.C = result < 0x0100;
	this->Status.Z = result == 0x00;
	this->Status.V = ((this->A ^ memory) & (this->A ^ (uint8_t)(result & 0xFF)) & 0x80) != 0;
	this->Status.N = (result & (1 << 7)) != 0;

	this->A = result & 0xFF;

	this->Cycles += 1;
}

void CPU::OPCODE_INC() {
	uint8_t value = this->ReadByte(this->AddressAbsolute);
	uint8_t result = value + 1;

	this->WriteByte(this->AddressAbsolute, value);
	this->WriteByte(this->AddressAbsolute, result);

	this->Status.Z = result == 0x00;
	this->Status.N = (result & (1 << 7)) != 0;

	this->Cycles += 3;
}

void CPU::OPCODE_DEC() {
	uint8_t value = this->ReadByte(this->AddressAbsolute);
	uint8_t result = value - 1;

	this->WriteByte(this->AddressAbsolute, value);
	this->WriteByte(this->AddressAbsolute, result);

	this->Status.Z = result == 0x00;
	this->Status.N = (result & (1 << 7)) != 0;

	this->Cycles += 3;
}

void CPU::OPCODE_INX() {
	this->X += 1;

	this->Status.Z = this->X == 0x00;
	this->Status.N = (this->X & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_DEX() {
	this->X -= 1;

	this->Status.Z = this->X == 0x00;
	this->Status.N = (this->X & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_INY() {
	this->Y += 1;

	this->Status.Z = this->Y == 0x00;
	this->Status.N = (this->Y & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_DEY() {
	this->Y -= 1;

	this->Status.Z = this->Y == 0x00;
	this->Status.N = (this->Y & (1 << 7)) != 0;

	this->Cycles += 1;
}

void CPU::OPCODE_ASL() {
	uint8_t value;
	if (this->CurrentAddressingMode == ADDRESSING_IMPLIED) {
		value = this->A;
	}
	else {
		value = this->ReadByte(this->AddressAbsolute);
	}
}