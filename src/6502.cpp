#include "6502.hpp"

fs6502::fs6502() {
	// The only other reserved locations in the memory map are the
	// very last 6 bytes of memory which must be programmed:
	// ($FFFA/B) the addresses of the non-maskable interrupt handler
	// ($FFFC/D) the power on reset location
	// ($FFFE/F) the BRK/interrupt request handler
	Memory[0xFFFA] = 0;
	Memory[0xFFFB] = 0;
	
	Memory[0xFFFC] = 0xE2;
	Memory[0xFFFD] = 0xFC;
	
	Memory[0xFFFE] = 0;
	Memory[0xFFFF] = 0;
	
	Memory[0xFCE2] = 0x69;
	
	_Internal_BuildInstructionTable();
}

void fs6502::_Internal_BuildInstructionTable() {
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_IMMEDIATE,	0x69, 2, 2, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_ZERO_PAGE,	0x65, 2, 3, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_ZERO_PAGE_X,	0x75, 2, 4, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_ABSOLUTE,	0x6D, 3, 4, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_ABSOLUTE_X,	0x7D, 3, 4, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_ABSOLUTE_Y,	0x79, 3, 4, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_INDIRECT_X,	0x61, 2, 6, &fs6502::_Instruction_ADC));
	InstructionTable.push_back(fsInstruction("ADC", FS_ADR_INDIRECT_Y,	0x71, 2, 5, &fs6502::_Instruction_ADC));
	
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_IMMEDIATE,	0x29, 2, 2, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_ZERO_PAGE,	0x25, 2, 3, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_ZERO_PAGE_X,	0x35, 2, 4, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_ABSOLUTE,	0x2D, 3, 4, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_ABSOLUTE_X,	0x3D, 3, 4, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_ABSOLUTE_Y,	0x39, 3, 4, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_INDIRECT_X,	0x21, 2, 6, &fs6502::_Instruction_AND));
	InstructionTable.push_back(fsInstruction("AND", FS_ADR_INDIRECT_Y,	0x31, 2, 5, &fs6502::_Instruction_AND));
	
	InstructionTable.push_back(fsInstruction("ASL", FS_ADR_ACCUMULATOR,	0x0A, 1, 2, &fs6502::_Instruction_ASL));
	InstructionTable.push_back(fsInstruction("ASL", FS_ADR_ZERO_PAGE,	0x06, 2, 5, &fs6502::_Instruction_ASL));
	InstructionTable.push_back(fsInstruction("ASL", FS_ADR_ZERO_PAGE_X,	0x16, 2, 6, &fs6502::_Instruction_ASL));
	InstructionTable.push_back(fsInstruction("ASL", FS_ADR_ABSOLUTE,	0x0E, 3, 6, &fs6502::_Instruction_ASL));
	InstructionTable.push_back(fsInstruction("ASL", FS_ADR_ABSOLUTE_X,	0x1E, 3, 7, &fs6502::_Instruction_ASL));
	
	InstructionTable.push_back(fsInstruction("BCC", FS_ADR_RELATIVE,	0x90, 2, 2, &fs6502::_Instruction_BCC));
	InstructionTable.push_back(fsInstruction("BCS", FS_ADR_RELATIVE,	0xB0, 2, 2, &fs6502::_Instruction_BCS));
	InstructionTable.push_back(fsInstruction("BEQ", FS_ADR_RELATIVE,	0xF0, 2, 2, &fs6502::_Instruction_BEQ));
	
	InstructionTable.push_back(fsInstruction("BIT", FS_ADR_ZERO_PAGE,	0x24, 2, 3, &fs6502::_Instruction_BIT));
	InstructionTable.push_back(fsInstruction("BIT", FS_ADR_ABSOLUTE,	0x2C, 3, 4, &fs6502::_Instruction_BIT));
	
	InstructionTable.push_back(fsInstruction("BMI", FS_ADR_RELATIVE,	0x30, 2, 2, &fs6502::_Instruction_BMI));
	InstructionTable.push_back(fsInstruction("BNE", FS_ADR_RELATIVE,	0xD0, 2, 2, &fs6502::_Instruction_BNE));
	InstructionTable.push_back(fsInstruction("BPL", FS_ADR_RELATIVE,	0x10, 2, 2, &fs6502::_Instruction_BPL));
	InstructionTable.push_back(fsInstruction("BRK", FS_ADR_IMPLIED,		0x00, 1, 7, &fs6502::_Instruction_BRK));
	InstructionTable.push_back(fsInstruction("BVC", FS_ADR_RELATIVE,	0x50, 2, 2, &fs6502::_Instruction_BVC));
	InstructionTable.push_back(fsInstruction("BVS", FS_ADR_RELATIVE,	0x70, 2, 2, &fs6502::_Instruction_BVS));
	InstructionTable.push_back(fsInstruction("CLC", FS_ADR_IMPLIED,		0x18, 1, 2, &fs6502::_Instruction_CLC));
	InstructionTable.push_back(fsInstruction("CLD", FS_ADR_IMPLIED,		0xD8, 1, 2, &fs6502::_Instruction_CLD));
	InstructionTable.push_back(fsInstruction("CLI", FS_ADR_IMPLIED,		0x58, 1, 2, &fs6502::_Instruction_CLI));
	InstructionTable.push_back(fsInstruction("CLV", FS_ADR_IMPLIED,		0xB8, 1, 2, &fs6502::_Instruction_CLV));
	
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_IMMEDIATE,	0xC9, 2, 2, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_ZERO_PAGE,	0xC5, 2, 3, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_ZERO_PAGE_X,	0xD5, 2, 4, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_ABSOLUTE,	0xCD, 3, 4, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_ABSOLUTE_X,	0xDD, 3, 4, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_ABSOLUTE_Y,	0xD9, 3, 4, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_INDIRECT_X,	0xC1, 2, 6, &fs6502::_Instruction_CMP));
	InstructionTable.push_back(fsInstruction("CMP", FS_ADR_INDIRECT_Y,	0xD1, 2, 5, &fs6502::_Instruction_CMP));
	
	InstructionTable.push_back(fsInstruction("CPX", FS_ADR_IMMEDIATE,	0xE0, 2, 2, &fs6502::_Instruction_CPX));
	InstructionTable.push_back(fsInstruction("CPX", FS_ADR_ZERO_PAGE,	0xE4, 2, 3, &fs6502::_Instruction_CPX));
	InstructionTable.push_back(fsInstruction("CPX", FS_ADR_ABSOLUTE,	0xEC, 3, 4, &fs6502::_Instruction_CPX));
	
	InstructionTable.push_back(fsInstruction("CPY", FS_ADR_IMMEDIATE,	0xC0, 2, 2, &fs6502::_Instruction_CPY));
	InstructionTable.push_back(fsInstruction("CPY", FS_ADR_ZERO_PAGE,	0xC4, 2, 3, &fs6502::_Instruction_CPY));
	InstructionTable.push_back(fsInstruction("CPY", FS_ADR_ABSOLUTE,	0xCC, 3, 4, &fs6502::_Instruction_CPY));
	
	InstructionTable.push_back(fsInstruction("DEC", FS_ADR_ZERO_PAGE,	0xC6, 2, 5, &fs6502::_Instruction_DEC));
	InstructionTable.push_back(fsInstruction("DEC", FS_ADR_ZERO_PAGE_X,	0xD6, 2, 6, &fs6502::_Instruction_DEC));
	InstructionTable.push_back(fsInstruction("DEC", FS_ADR_ABSOLUTE,	0xCE, 3, 6, &fs6502::_Instruction_DEC));
	InstructionTable.push_back(fsInstruction("DEC", FS_ADR_ABSOLUTE_X,	0xDE, 3, 7, &fs6502::_Instruction_DEC));
	
	InstructionTable.push_back(fsInstruction("DEX", FS_ADR_IMPLIED,		0xCA, 1, 2, &fs6502::_Instruction_DEX));
	InstructionTable.push_back(fsInstruction("DEY", FS_ADR_IMPLIED,		0x88, 1, 2, &fs6502::_Instruction_DEY));
	
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_IMMEDIATE,	0x49, 2, 2, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_ZERO_PAGE,	0x45, 2, 3, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_ZERO_PAGE_X,	0x55, 2, 4, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_ABSOLUTE,	0x4D, 3, 4, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_ABSOLUTE_X,	0x5D, 3, 4, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_ABSOLUTE_Y,	0x59, 3, 4, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_INDIRECT_X,	0x41, 2, 6, &fs6502::_Instruction_EOR));
	InstructionTable.push_back(fsInstruction("EOR", FS_ADR_INDIRECT_Y,	0x51, 2, 5, &fs6502::_Instruction_EOR));
	
	InstructionTable.push_back(fsInstruction("INC", FS_ADR_ZERO_PAGE,	0xE6, 2, 5, &fs6502::_Instruction_INC));
	InstructionTable.push_back(fsInstruction("INC", FS_ADR_ZERO_PAGE_X,	0xF6, 2, 6, &fs6502::_Instruction_INC));
	InstructionTable.push_back(fsInstruction("INC", FS_ADR_ABSOLUTE,	0xEE, 3, 6, &fs6502::_Instruction_INC));
	InstructionTable.push_back(fsInstruction("INC", FS_ADR_ABSOLUTE_X,	0xFE, 3, 7, &fs6502::_Instruction_INC));
	
	InstructionTable.push_back(fsInstruction("INX", FS_ADR_IMPLIED,		0xE8, 1, 2, &fs6502::_Instruction_INX));
	InstructionTable.push_back(fsInstruction("INY", FS_ADR_IMPLIED,		0xC8, 1, 2, &fs6502::_Instruction_INY));
	
	InstructionTable.push_back(fsInstruction("JMP", FS_ADR_ABSOLUTE,	0x4C, 3, 3, &fs6502::_Instruction_JMP));
	InstructionTable.push_back(fsInstruction("JMP", FS_ADR_INDIRECT,	0x6C, 3, 5, &fs6502::_Instruction_JMP));
	
	InstructionTable.push_back(fsInstruction("JSR", FS_ADR_ABSOLUTE,	0x20, 3, 6, &fs6502::_Instruction_JSR));
	
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_IMMEDIATE,	0xA9, 2, 2, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_ZERO_PAGE,	0xA5, 2, 3, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_ZERO_PAGE_X,	0xB5, 2, 4, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_ABSOLUTE,	0xAD, 3, 4, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_ABSOLUTE_X,	0xBD, 3, 4, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_ABSOLUTE_Y,	0xD9, 3, 4, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_INDIRECT_X,	0xA1, 2, 6, &fs6502::_Instruction_LDA));
	InstructionTable.push_back(fsInstruction("LDA", FS_ADR_INDIRECT_Y,	0xB1, 2, 5, &fs6502::_Instruction_LDA));
	
	InstructionTable.push_back(fsInstruction("LDX", FS_ADR_IMMEDIATE,	0xA2, 2, 2, &fs6502::_Instruction_LDX));
	InstructionTable.push_back(fsInstruction("LDX", FS_ADR_ZERO_PAGE,	0xA6, 2, 3, &fs6502::_Instruction_LDX));
	InstructionTable.push_back(fsInstruction("LDX", FS_ADR_ZERO_PAGE_Y,	0xB6, 2, 4, &fs6502::_Instruction_LDX));
	InstructionTable.push_back(fsInstruction("LDX", FS_ADR_ABSOLUTE,	0xAE, 3, 4, &fs6502::_Instruction_LDX));
	InstructionTable.push_back(fsInstruction("LDX", FS_ADR_ABSOLUTE_Y,	0xBE, 3, 4, &fs6502::_Instruction_LDX));
	
	InstructionTable.push_back(fsInstruction("LDY", FS_ADR_IMMEDIATE,	0xA0, 2, 2, &fs6502::_Instruction_LDY));
	InstructionTable.push_back(fsInstruction("LDY", FS_ADR_ZERO_PAGE,	0xA4, 2, 3, &fs6502::_Instruction_LDY));
	InstructionTable.push_back(fsInstruction("LDY", FS_ADR_ZERO_PAGE_X,	0xB4, 2, 4, &fs6502::_Instruction_LDY));
	InstructionTable.push_back(fsInstruction("LDY", FS_ADR_ABSOLUTE,	0xAC, 3, 4, &fs6502::_Instruction_LDY));
	InstructionTable.push_back(fsInstruction("LDY", FS_ADR_ABSOLUTE_X,	0xBC, 3, 4, &fs6502::_Instruction_LDY));
	
	InstructionTable.push_back(fsInstruction("LSR", FS_ADR_ACCUMULATOR,	0x4A, 1, 2, &fs6502::_Instruction_LSR));
	InstructionTable.push_back(fsInstruction("LSR", FS_ADR_ZERO_PAGE,	0x46, 2, 5, &fs6502::_Instruction_LSR));
	InstructionTable.push_back(fsInstruction("LSR", FS_ADR_ZERO_PAGE_X,	0x56, 2, 6, &fs6502::_Instruction_LSR));
	InstructionTable.push_back(fsInstruction("LSR", FS_ADR_ABSOLUTE,	0x4E, 3, 6, &fs6502::_Instruction_LSR));
	InstructionTable.push_back(fsInstruction("LSR", FS_ADR_ABSOLUTE_X,	0x5E, 3, 7, &fs6502::_Instruction_LSR));
	
	InstructionTable.push_back(fsInstruction("NOP", FS_ADR_IMPLIED,		0xEA, 1, 2, &fs6502::_Instruction_NOP));
	
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_IMMEDIATE,	0x09, 2, 2, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_ZERO_PAGE,	0x05, 2, 3, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_ZERO_PAGE_X,	0x15, 2, 4, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_ABSOLUTE,	0x0D, 3, 4, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_ABSOLUTE_X,	0x1D, 3, 4, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_ABSOLUTE_Y,	0x19, 3, 4, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_INDIRECT_X,	0x01, 2, 6, &fs6502::_Instruction_ORA));
	InstructionTable.push_back(fsInstruction("ORA", FS_ADR_INDIRECT_Y,	0x11, 2, 5, &fs6502::_Instruction_ORA));
	
	InstructionTable.push_back(fsInstruction("PHA", FS_ADR_IMPLIED,		0x48, 1, 3, &fs6502::_Instruction_PHA));
	InstructionTable.push_back(fsInstruction("PHP", FS_ADR_IMPLIED,		0x08, 1, 3, &fs6502::_Instruction_PHP));
	InstructionTable.push_back(fsInstruction("PLA", FS_ADR_IMPLIED,		0x68, 1, 4, &fs6502::_Instruction_PLA));
	InstructionTable.push_back(fsInstruction("PLP", FS_ADR_IMPLIED,		0x28, 1, 4, &fs6502::_Instruction_PLP));
	
	InstructionTable.push_back(fsInstruction("ROL", FS_ADR_ACCUMULATOR,	0x2A, 1, 2, &fs6502::_Instruction_ROL));
	InstructionTable.push_back(fsInstruction("ROL", FS_ADR_ZERO_PAGE,	0x26, 2, 5, &fs6502::_Instruction_ROL));
	InstructionTable.push_back(fsInstruction("ROL", FS_ADR_ZERO_PAGE_X,	0x36, 2, 6, &fs6502::_Instruction_ROL));
	InstructionTable.push_back(fsInstruction("ROL", FS_ADR_ABSOLUTE,	0x2E, 3, 6, &fs6502::_Instruction_ROL));
	InstructionTable.push_back(fsInstruction("ROL", FS_ADR_ABSOLUTE_X,	0x3E, 3, 7, &fs6502::_Instruction_ROL));
	
	InstructionTable.push_back(fsInstruction("ROR", FS_ADR_ACCUMULATOR,	0x6A, 1, 2, &fs6502::_Instruction_ROR));
	InstructionTable.push_back(fsInstruction("ROR", FS_ADR_ZERO_PAGE,	0x66, 2, 5, &fs6502::_Instruction_ROR));
	InstructionTable.push_back(fsInstruction("ROR", FS_ADR_ZERO_PAGE_X,	0x76, 2, 6, &fs6502::_Instruction_ROR));
	InstructionTable.push_back(fsInstruction("ROR", FS_ADR_ABSOLUTE,	0x6E, 3, 6, &fs6502::_Instruction_ROR));
	InstructionTable.push_back(fsInstruction("ROR", FS_ADR_ABSOLUTE_X,	0x7E, 3, 7, &fs6502::_Instruction_ROR));
	
	InstructionTable.push_back(fsInstruction("RTI", FS_ADR_IMPLIED,		0x40, 1, 6, &fs6502::_Instruction_RTI));
	InstructionTable.push_back(fsInstruction("RTS", FS_ADR_IMPLIED,		0x60, 1, 6, &fs6502::_Instruction_RTS));
	
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_IMMEDIATE,	0xE9, 2, 2, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_ZERO_PAGE,	0xE5, 2, 3, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_ZERO_PAGE_X,	0xF5, 2, 4, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_ABSOLUTE,	0xED, 3, 4, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_ABSOLUTE_X,	0xFD, 3, 4, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_ABSOLUTE_Y,	0xF9, 3, 4, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_INDIRECT_X,	0xE1, 2, 6, &fs6502::_Instruction_SBC));
	InstructionTable.push_back(fsInstruction("SBC", FS_ADR_INDIRECT_Y,	0xF1, 2, 5, &fs6502::_Instruction_SBC));
	
	InstructionTable.push_back(fsInstruction("SEC", FS_ADR_IMPLIED,		0x38, 1, 2, &fs6502::_Instruction_SEC));
	InstructionTable.push_back(fsInstruction("SED", FS_ADR_IMPLIED,		0xF8, 1, 2, &fs6502::_Instruction_SED));
	InstructionTable.push_back(fsInstruction("SEI", FS_ADR_IMPLIED,		0x78, 1, 2, &fs6502::_Instruction_SEI));
	
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_ZERO_PAGE,	0x85, 2, 3, &fs6502::_Instruction_STA));
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_ZERO_PAGE_X,	0x95, 2, 4, &fs6502::_Instruction_STA));
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_ABSOLUTE,	0x8D, 3, 4, &fs6502::_Instruction_STA));
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_ABSOLUTE_X,	0x9D, 3, 5, &fs6502::_Instruction_STA));
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_ABSOLUTE_Y,	0x99, 3, 5, &fs6502::_Instruction_STA));
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_INDIRECT_X,	0x81, 2, 6, &fs6502::_Instruction_STA));
	InstructionTable.push_back(fsInstruction("STA", FS_ADR_INDIRECT_Y,	0x91, 2, 5, &fs6502::_Instruction_STA));
	
	InstructionTable.push_back(fsInstruction("STX", FS_ADR_ZERO_PAGE,	0x86, 2, 3, &fs6502::_Instruction_STX));
	InstructionTable.push_back(fsInstruction("STX", FS_ADR_ZERO_PAGE_Y,	0x96, 2, 4, &fs6502::_Instruction_STX));
	InstructionTable.push_back(fsInstruction("STX", FS_ADR_ABSOLUTE,	0x8E, 3, 4, &fs6502::_Instruction_STX));
	
	InstructionTable.push_back(fsInstruction("STY", FS_ADR_ZERO_PAGE,	0x84, 2, 3, &fs6502::_Instruction_STY));
	InstructionTable.push_back(fsInstruction("STY", FS_ADR_ZERO_PAGE_X,	0x94, 2, 4, &fs6502::_Instruction_STY));
	InstructionTable.push_back(fsInstruction("STY", FS_ADR_ABSOLUTE,	0x8C, 3, 4, &fs6502::_Instruction_STY));
	
	InstructionTable.push_back(fsInstruction("TAX", FS_ADR_IMPLIED,		0xAA, 1, 2, &fs6502::_Instruction_TAX));
	InstructionTable.push_back(fsInstruction("TAY", FS_ADR_IMPLIED,		0xA8, 1, 2, &fs6502::_Instruction_TAY));
	InstructionTable.push_back(fsInstruction("TSX", FS_ADR_IMPLIED,		0xBA, 1, 2, &fs6502::_Instruction_TSX));
	InstructionTable.push_back(fsInstruction("TXA", FS_ADR_IMPLIED,		0x8A, 1, 2, &fs6502::_Instruction_TXA));
	InstructionTable.push_back(fsInstruction("TXS", FS_ADR_IMPLIED,		0x9A, 1, 2, &fs6502::_Instruction_TXS));
	InstructionTable.push_back(fsInstruction("TYA", FS_ADR_IMPLIED,		0x98, 1, 2, &fs6502::_Instruction_TYA));
	
	InstructionTableNum = InstructionTable.size();
}

fs6502::fsInstruction fs6502::_Internal_DecodeInstruction() {
	fsInstruction Result;
	
	for (int X = 0; X < InstructionTableNum; X++) {
		if (InstructionTable[X].OpCode == IR) { return InstructionTable[X]; }
	}
	
	return Result;
}
void fs6502::_Internal_DecodeAddressing() {
	// Declare variable here as declareing them
	// within switch block will error out :(
	// Some addressing mode may need them
	fsByte L = 0;
	fsByte H = 0;
	fsWord Temp = 0;
	
	// Clear Out Operand Before
	CurrInstruction.Operand = 0;
	
	switch (CurrInstruction.AddressingMode) {
		case fsMemoryAddressingMode::IMMEDIATE:
			// Allows the programmer to directly specify an 8 bit constant within the instruction
			CurrInstruction.OperandAdr = PC++;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::ZERO_PAGE:
			// An instruction using zero page addressing mode has only
			// an 8 bit address operand. This limits it to addressing
			// only the first 256 bytes of memory (e.g. $0000 to $00FF)
			// where the most significant byte of the address is always zero.
			CurrInstruction.OperandAdr = Memory[PC++];
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::ZERO_PAGE_X:
			// The address to be accessed by an instruction
			// using indexed zero page addressing is calculated by
			// taking the 8 bit zero page address from the instruction
			// and adding the current value of the X register to it
			// Note: The address calculation wraps around if the sum of the base address and the register exceed $FF.
			CurrInstruction.OperandAdr = (Memory[PC++] + X) % FS_BYTE_MAX;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::ZERO_PAGE_Y:
			// The address to be accessed by an instruction using
			// indexed zero page addressing is calculated by taking
			// the 8 bit zero page address from the instruction and
			// adding the current value of the Y register to it.
			// This mode can only be used with the LDX and STX instructions.
			// Note: Documentation does not explicitly states but assuming
			//		 this wraps around same as ZERO_PAGE_X
			CurrInstruction.OperandAdr = (Memory[PC++] + Y) % FS_BYTE_MAX;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::RELATIVE:
			// Relative addressing mode is used by branch instructions (e.g. BEQ, BNE, etc.)
			// which contain a signed 8 bit relative offset (e.g. -128 to +127) which is added
			// to program counter if the condition is true. As the program counter itself is
			// incremented during instruction execution by two the effective address range for
			// the target instruction must be with -126 to +129 bytes of the branch.
			// NOTE: Here we only grab the number and let the change of the PC handled by 
			//		 the instruction handler
			CurrInstruction.OperandAdr = PC++;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::ABSOLUTE:
			// Instructions using absolute addressing contain a full 16 bit address to identify the target location.
			L = Memory[PC++];
			H = Memory[PC++];
			Temp = L;
			Temp |= (fsWord)(H << FS_SIZE_OF_BYTE);
			CurrInstruction.OperandAdr = Temp;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::ABSOLUTE_X:
			// The address to be accessed by an instruction using X register
			// indexed absolute addressing is computed by taking the 16 bit address
			// from the instruction and added the contents of the X register.
			// For example if X contains $92 then an STA $2000,X instruction will
			// store the accumulator at $2092 (e.g. $2000 + $92).
			L = Memory[PC++];
			H = Memory[PC++];
			Temp = L;
			Temp |= (fsWord)(H << FS_SIZE_OF_BYTE);
			CurrInstruction.OperandAdr = Temp + X;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::ABSOLUTE_Y:
			// The Y register indexed absolute addressing mode is
			// the same as the previous mode only with the contents
			// of the Y register added to the 16 bit address from the instruction.
			L = Memory[PC++];
			H = Memory[PC++];
			Temp = L;
			Temp |= (fsWord)(H << FS_SIZE_OF_BYTE);
			CurrInstruction.OperandAdr = Temp + Y;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			break;
		case fsMemoryAddressingMode::INDIRECT:
			// JMP is the only 6502 instruction to support indirection.
			// The instruction contains a 16 bit address which identifies the
			// location of the least significant byte of another 16 bit memory address 
			// which is the real target of the instruction.
			// For example if location $0120 contains $FC and location $0121 contains $BA
			// then the instruction JMP ($0120) will cause the next instruction execution
			// to occur at $BAFC (e.g. the contents of $0120 and $0121).
			L = Memory[PC++];
			H = Memory[PC++];
			Temp = L;
			Temp |= (fsWord)(H << FS_SIZE_OF_BYTE);
			CurrInstruction.OperandAdr = Temp;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			CurrInstruction.Operand |= fsWord(Memory[CurrInstruction.OperandAdr + 1]) << FS_SIZE_OF_BYTE;
			break;
		case fsMemoryAddressingMode::INDIRECT_X:
			// Indexed indirect addressing is normally used in conjunction with a
			// table of address held on zero page. The address of the table is taken
			// from the instruction and the X register added to it (with zero page wrap around)
			// to give the location of the least significant byte of the target address.
			L = (Memory[PC++] + X) % FS_BYTE_MAX;
			Temp = Memory[L];
			CurrInstruction.OperandAdr = Temp;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			CurrInstruction.Operand |= fsWord(Memory[CurrInstruction.OperandAdr + 1]) << FS_SIZE_OF_BYTE;
			break;
		case fsMemoryAddressingMode::INDIRECT_Y:
			// Indirect indirect addressing is the most common
			// indirection mode used on the 6502. In instruction contains
			// the zero page location of the least significant byte of 16 bit address.
			// The Y register is dynamically added to this value to generated the actual
			// target address for operation.
			L = Memory[PC++];
			Temp = Memory[L] + Y;
			CurrInstruction.OperandAdr = Temp;
			CurrInstruction.Operand = Memory[CurrInstruction.OperandAdr];
			CurrInstruction.Operand |= fsWord(Memory[CurrInstruction.OperandAdr + 1]) << FS_SIZE_OF_BYTE;
			break;
		default: break;
	}
}

void fs6502::_Internal_SetFlag(fsByte IN_Flag, bool IN_isSet) {
	if (IN_isSet) { Status = (Status | IN_Flag); }
	else {Status = (Status & ~IN_Flag);}
}

void fs6502::_Instruction_ADC() {
	// This instruction adds the contents of a memory location
	// to the accumulator together with the carry bit. If overflow
	// occurs the carry bit is set, this enables multiple byte addition
	// to be performed.
	
	// C	Carry Flag	Set if overflow in bit 7
	// Z	Zero Flag	Set if A = 0
	// I	Interrupt Disable	Not affected
	// D	Decimal Mode Flag	Not affected
	// B	Break Command	Not affected
	// V	Overflow Flag	Set if sign bit is incorrect
	// N	Negative Flag	Set if bit 7 set
	fsWord Temp = CurrInstruction.Operand + (Status & FLAG_CARRY);
	
	// This will be needed for calculating Overflow flag
	fsByte A7 = A >> 7;
	fsByte T7 = Temp >> 7;
	fsByte C6 =  (A & 0b01000000) >> 6 & ((fsByte)Temp & 0b01000000) >> 6;
	
	A += Temp;
	
	_Internal_SetFlag(FLAG_CARRY, (Temp & 0b0000000100000000));
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, (A & 0b10000000));
	
	// https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
	// V = (!A7&!T7&C6) | (A7&T7&!C6)
	_Internal_SetFlag(FLAG_OVERFLOW, ((~A7 & ~T7 & C6) | (A7 & T7 & ~C6)));
}
void fs6502::_Instruction_AND() {
	// A logical AND is performed, bit by bit,
	// on the accumulator contents using the contents of a byte of memory.
	
	// C	Carry Flag	Not affected
	// Z	Zero Flag	Set if A = 0
	// I	Interrupt Disable	Not affected
	// D	Decimal Mode Flag	Not affected
	// B	Break Command	Not affected
	// V	Overflow Flag	Not affected
	// N	Negative Flag	Set if bit 7 set
	
	A = (A & CurrInstruction.Operand);
	
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, (A & 0b10000000));
}
void fs6502::_Instruction_ASL() {
	// This operation shifts all the bits of the accumulator
	// or memory contents one bit left.
	// Bit 0 is set to 0 and bit 7 is placed in the carry flag.
	// The effect of this operation is to multiply the memory contents by 2
	// (ignoring 2's complement considerations), setting the carry if the
	// result will not fit in 8 bits.
	
	//C		Carry Flag	Set to contents of old bit 7
	//Z		Zero Flag	Set if A = 0
	//I		Interrupt Disable	Not affected
	//D		Decimal Mode Flag	Not affected
	//B		Break Command	Not affected
	//V		Overflow Flag	Not affected
	//N		Negative Flag	Set if bit 7 of the result is set
	
	_Internal_SetFlag(FLAG_CARRY, (A & 0b10000000));
	A = A << 1;
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, (A & 0b10000000));
}
void fs6502::_Instruction_BCC() {
	// If the carry flag is clear then add
	// the relative displacement to the program counter
	// to cause a branch to a new location.
	if (!(Status & FLAG_CARRY)) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BCS() {
	// If the carry flag is set then add the relative
	// displacement to the program counter to cause a
	// branch to a new location.
	if (Status & FLAG_CARRY) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BEQ() {
	// If the zero flag is set then add the
	// relative displacement to the program counter
	/// to cause a branch to a new location.
	if (Status & FLAG_ZERO) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BIT() {
	// This instructions is used to test if one or more bits
	// are set in a target memory location. The mask pattern
	// in A is ANDed with the value in memory to set or clear
	// the zero flag, but the result is not kept. Bits 7 and 6 of
	// the value from memory are copied into the N and V flags.
	fsByte Temp = A & CurrInstruction.Operand;
	_Internal_SetFlag(FLAG_ZERO, Temp == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, CurrInstruction.Operand & 0b10000000);
	_Internal_SetFlag(FLAG_OVERFLOW, CurrInstruction.Operand & 0b01000000);
}
void fs6502::_Instruction_BMI() {
	//If the negative flag is set then add
	// the relative displacement to the program
	// counter to cause a branch to a new location.
	if (Status & FLAG_NEGATIVE) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BNE() {
	// If the zero flag is clear then
	// add the relative displacement to the program
	// counter to cause a branch to a new location.
	if (!(Status & FLAG_ZERO)) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BPL() {
	// If the negative flag is clear then
	// add the relative displacement to the program
	// counter to cause a branch to a new location.
	if (!(Status & FLAG_NEGATIVE)) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BRK() {
	// The BRK instruction forces the generation of an interrupt request.
	// The program counter and processor status are pushed on the stack
	// then the IRQ interrupt vector at $FFFE/F is loaded into the PC and
	// the break flag in the status set to one.
	
	fsByte L = PC;
	fsByte H = PC >> FS_SIZE_OF_BYTE;
	
	Memory[SP++] = L;
	Memory[SP++] = H;
	Memory[SP++] = Status;
	
	PC = Memory[0xFFFE];
	PC |= Memory[0xFFFF] << FS_SIZE_OF_BYTE;
	_Internal_SetFlag(FLAG_BREAK, true);
}
void fs6502::_Instruction_BVC() {
	// If the overflow flag is clear then add the relative
	// displacement to the program counter to cause a branch to a new location.
	if (Status & FLAG_OVERFLOW) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_BVS() {
	// If the overflow flag is set then add the relative displacement
	// to the program counter to cause a branch to a new location.
	if (!(Status & FLAG_OVERFLOW)) {
		fsSByte Temp = CurrInstruction.Operand;
		fsByte Rel = Temp & 0b01111111;
		if (Temp < 0) { PC -= Rel; }
		else if (Temp > 0) { PC += Rel; }
	}
}
void fs6502::_Instruction_CLC() {
	// Set the carry flag to zero.
	_Internal_SetFlag(FLAG_CARRY, false);
}
void fs6502::_Instruction_CLD() {
	// Sets the decimal mode flag to zero.
	_Internal_SetFlag(FLAG_DECIMAL, false);
}
void fs6502::_Instruction_CLI() {
	// Clears the interrupt disable flag allowing
	// normal interrupt requests to be serviced.
	_Internal_SetFlag(FLAG_INTERUPT, false);
}
void fs6502::_Instruction_CLV() {
	// Clears the overflow flag.
	_Internal_SetFlag(FLAG_OVERFLOW, false);
}
void fs6502::_Instruction_CMP() {
	// This instruction compares the contents of the accumulator
	// with another memory held value and sets the zero and carry
	// flags as appropriate.
	
	// C	Carry Flag	Set if A >= M
	// Z	Zero Flag	Set if A = M
	// N	Negative Flag	Set if bit 7 of the result is set
	_Internal_SetFlag(FLAG_CARRY, A >= CurrInstruction.Operand);
	_Internal_SetFlag(FLAG_ZERO, A == CurrInstruction.Operand);
	_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
}
void fs6502::_Instruction_CPX() {
	// This instruction compares the contents of the X register with another memory held
	// value and sets the zero and carry flags as appropriate.
	// C	Carry Flag	Set if X >= M
	// Z	Zero Flag	Set if X = M
	// N	Negative Flag	Set if bit 7 of the result is set
	_Internal_SetFlag(FLAG_CARRY, X >= CurrInstruction.Operand);
	_Internal_SetFlag(FLAG_ZERO, X == CurrInstruction.Operand);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_CPY() {
	// This instruction compares the contents of the Y register with another memory held
	// value and sets the zero and carry flags as appropriate.
	// C	Carry Flag	Set if Y >= M
	// Z	Zero Flag	Set if Y = M
	// N	Negative Flag	Set if bit 7 of the result is set
	_Internal_SetFlag(FLAG_CARRY, Y >= CurrInstruction.Operand);
	_Internal_SetFlag(FLAG_ZERO, Y == CurrInstruction.Operand);
	_Internal_SetFlag(FLAG_NEGATIVE, Y & 0b10000000);
}
void fs6502::_Instruction_DEC() {
	// Subtracts one from the value held at a specified memory
	// location setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if result is zero
	// N	Negative Flag	Set if bit 7 of the result is set
	CurrInstruction.Operand--;
	_Internal_SetFlag(FLAG_ZERO, CurrInstruction.Operand == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, CurrInstruction.Operand & 0b10000000);
	Memory[CurrInstruction.OperandAdr]--;
}
void fs6502::_Instruction_DEX() {
	// Subtracts one from the X register setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if X is zero
	// N	Negative Flag	Set if bit 7 of X is set
	X--;
	_Internal_SetFlag(FLAG_ZERO, X == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_DEY() {
	// Subtracts one from the Y register setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if Y is zero
	// N	Negative Flag	Set if bit 7 of Y is set
	Y--;
	_Internal_SetFlag(FLAG_ZERO, Y == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, Y & 0b10000000);
}
void fs6502::_Instruction_EOR() {
	// An exclusive OR is performed, bit by bit,
	// on the accumulator contents using the contents of a byte of memory.
	// Z	Zero Flag	Set if A = 0
	// N	Negative Flag	Set if bit 7 set
	A = A ^ CurrInstruction.Operand;
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
}
void fs6502::_Instruction_INC() {
	// Adds one to the value held at a specified memory
	// location setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if result is zero
	// N	Negative Flag	Set if bit 7 of the result is set
	CurrInstruction.Operand++;
	_Internal_SetFlag(FLAG_ZERO, CurrInstruction.Operand == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, CurrInstruction.Operand & 0b10000000);
	Memory[CurrInstruction.OperandAdr]++;
}
void fs6502::_Instruction_INX() {
	// Adds one from the X register setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if X is zero
	// N	Negative Flag	Set if bit 7 of X is set
	X++;
	_Internal_SetFlag(FLAG_ZERO, X == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_INY() {
	// Adds one from the Y register setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if Y is zero
	// N	Negative Flag	Set if bit 7 of Y is set
	Y++;
	_Internal_SetFlag(FLAG_ZERO, Y == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, Y & 0b10000000);
}
void fs6502::_Instruction_JMP() {
	PC = CurrInstruction.OperandAdr;
}
void fs6502::_Instruction_JSR() {
	// The JSR instruction pushes the address
	// (minus one) of the return point on to the
	// stack and then sets the program counter to the target memory address.
	fsWord Temp = CurrInstruction.OperandAdr - 1;
	fsByte L = Temp;
	fsByte H = Temp >> FS_SIZE_OF_BYTE;
	Memory[SP++] = L;
	Memory[SP++] = H;
	PC = CurrInstruction.OperandAdr;
}
void fs6502::_Instruction_LDA() {
	// Loads a byte of memory into the accumulator setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if A = 0
	// N	Negative Flag	Set if bit 7 of A is set
	
	A = CurrInstruction.Operand;
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
}
void fs6502::_Instruction_LDX() {
	// Loads a byte of memory into the X register setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if X = 0
	// N	Negative Flag	Set if bit 7 of X is set
	X = CurrInstruction.Operand;
	_Internal_SetFlag(FLAG_ZERO, X == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_LDY() {
	// Loads a byte of memory into the Y register setting the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if Y = 0
	// N	Negative Flag	Set if bit 7 of Y is set
	X = CurrInstruction.Operand;
	_Internal_SetFlag(FLAG_ZERO, X == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_LSR() {
	// Each of the bits in A or M is shift one place to the right.
	// The bit that was in bit 0 is shifted into the carry flag. Bit 7 is set to zero.
	// C	Carry Flag	Set to contents of old bit 0
	// Z	Zero Flag	Set if result = 0
	// N	Negative Flag	Set if bit 7 of the result is set
	
	if (CurrInstruction.AddressingMode == fsMemoryAddressingMode::ACCUMULATOR) {
		_Internal_SetFlag(FLAG_CARRY, A & 0b00000001);
		A = A >> 1;
		_Internal_SetFlag(FLAG_ZERO, A == 0);
		_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
	}
	else {
		_Internal_SetFlag(FLAG_CARRY, CurrInstruction.Operand & 0b00000001);
		CurrInstruction.Operand = CurrInstruction.Operand >> 1;
		Memory[CurrInstruction.OperandAdr] = CurrInstruction.Operand;
		_Internal_SetFlag(FLAG_ZERO, CurrInstruction.Operand == 0);
		_Internal_SetFlag(FLAG_NEGATIVE, CurrInstruction.Operand & 0b10000000);
	}
}
void fs6502::_Instruction_NOP() {
	// The NOP instruction causes no changes to the processor other
	// than the normal incrementing of the program counter to the next instruction.
	
	// Hardest one to implement yet o_O
}
void fs6502::_Instruction_ORA() {
	// An inclusive OR is performed, bit by bit,
	// on the accumulator contents using the contents of a byte of memory.
	// Z	Zero Flag	Set if A = 0
	// N	Negative Flag	Set if bit 7 set
	
	A = (A | CurrInstruction.Operand);
	
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, (A & 0b10000000));
}
void fs6502::_Instruction_PHA() {
	// Pushes a copy of the accumulator on to the stack.
	Memory[SP++] = A;
}
void fs6502::_Instruction_PHP() {
	// Pushes a copy of the status flags on to the stack.
	Memory[SP++] = Status;
}
void fs6502::_Instruction_PLA() {
	// Pulls an 8 bit value from the stack and into the accumulator.
	// The zero and negative flags are set as appropriate.
	// Z	Zero Flag	Set if A = 0
	// N	Negative Flag	Set if bit 7 of A is set
	A = Memory[--SP];
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, (A & 0b10000000));
}
void fs6502::_Instruction_PLP() {
	// Pulls an 8 bit value from the stack and
	// into the processor flags. The flags will take on new states
	// as determined by the value pulled.
	Status = Memory[--SP];
}
void fs6502::_Instruction_ROL() {
	// Move each of the bits in either A or M one place to the left.
	// Bit 0 is filled with the current value of the carry flag whilst the
	// old bit 7 becomes the new carry flag value.
	if (CurrInstruction.AddressingMode == fsMemoryAddressingMode::ACCUMULATOR) {
		A = A << 1;
		if (Status & FLAG_CARRY) { A |= 0x00000001; }
		_Internal_SetFlag(FLAG_ZERO, A == 0);
		_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
	}
	else {
		CurrInstruction.Operand = CurrInstruction.Operand << 1;
		if (Status & FLAG_CARRY) { CurrInstruction.Operand |= 0x00000001; }
		Memory[CurrInstruction.OperandAdr] = CurrInstruction.Operand;
		_Internal_SetFlag(FLAG_ZERO, CurrInstruction.Operand == 0);
		_Internal_SetFlag(FLAG_NEGATIVE, CurrInstruction.Operand & 0b10000000);
	}
}
void fs6502::_Instruction_ROR() {
	// Move each of the bits in either A or M one place to the right.
	// Bit 7 is filled with the current value of the carry flag whilst
	// the old bit 0 becomes the new carry flag value.
	if (CurrInstruction.AddressingMode == fsMemoryAddressingMode::ACCUMULATOR) {
		A = A >> 1;
		if (Status & FLAG_CARRY) { A |= 0x10000000; }
		_Internal_SetFlag(FLAG_ZERO, A == 0);
		_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
	}
	else {
		CurrInstruction.Operand = CurrInstruction.Operand >> 1;
		if (Status & FLAG_CARRY) { CurrInstruction.Operand |= 0x10000000; }
		Memory[CurrInstruction.OperandAdr] = CurrInstruction.Operand;
		_Internal_SetFlag(FLAG_ZERO, CurrInstruction.Operand == 0);
		_Internal_SetFlag(FLAG_NEGATIVE, CurrInstruction.Operand & 0b10000000);
	}
}
void fs6502::_Instruction_RTI() {
	// The RTI instruction is used at the end of an interrupt processing routine.
	// It pulls the processor flags from the stack followed by the program counter.
	Status = Memory[--SP];
	fsByte H = Memory[--SP];
	fsByte L = Memory[--SP];
	
	PC = L;
	PC |= H << FS_SIZE_OF_BYTE;
}
void fs6502::_Instruction_RTS() {
	// The RTS instruction is used at the end of a subroutine
	// to return to the calling routine.
	// It pulls the program counter (minus one) from the stack.
	fsByte H = Memory[--SP];
	fsByte L = Memory[--SP];
	
	PC = L;
	PC |= H << FS_SIZE_OF_BYTE;
}
void fs6502::_Instruction_SBC() {
	// This instruction subtracts the contents of a memory
	// location to the accumulator together with the not of the
	// carry bit. If overflow occurs the carry bit is clear, this
	// enables multiple byte subtraction to be performed.
	
	// C	Carry Flag	Clear if overflow in bit 7
	// Z	Zero Flag	Set if A = 0
	// V	Overflow Flag	Set if sign bit is incorrect
	// N	Negative Flag	Set if bit 7 set
	
	fsWord Temp = CurrInstruction.Operand + (Status & FLAG_CARRY);
	
	// This will be needed for calculating Overflow flag
	fsByte A7 = A >> 7;
	fsByte T7 = Temp >> 7;
	fsByte C6 =  (A & 0b01000000) >> 6 & ((fsByte)Temp & 0b01000000) >> 6;
	
	A -= Temp;
	
	_Internal_SetFlag(FLAG_CARRY, (Temp & 0b0000000100000000));
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, (A & 0b10000000));
	
	// https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
	// V = (!A7&!T7&C6) | (A7&T7&!C6)
	_Internal_SetFlag(FLAG_OVERFLOW, ((~A7 & ~T7 & C6) | (A7 & T7 & ~C6)));
}
void fs6502::_Instruction_SEC() {
	// Set the carry flag to one.
	_Internal_SetFlag(FLAG_CARRY, true);
}
void fs6502::_Instruction_SED() {
	// Set the decimal mode flag to one.
	_Internal_SetFlag(FLAG_DECIMAL, true);
}
void fs6502::_Instruction_SEI() {
	// Set the interrupt disable flag to one.
	_Internal_SetFlag(FLAG_INTERUPT, true);
}
void fs6502::_Instruction_STA() {
	// Stores the contents of the accumulator into memory.
	Memory[CurrInstruction.OperandAdr] = A;
}
void fs6502::_Instruction_STX() {
	// Stores the contents of the X register into memory.
	Memory[CurrInstruction.OperandAdr] = X;
}
void fs6502::_Instruction_STY() {
	// Stores the contents of the Y register into memory.
	Memory[CurrInstruction.OperandAdr] = Y;
}
void fs6502::_Instruction_TAX() {
	// Copies the current contents of the accumulator
	// into the X register and sets the zero and negative flags as appropriate.
	// Z	Zero Flag	Set if X = 0
	// N	Negative Flag	Set if bit 7 of X is set
	X = A;
	
	_Internal_SetFlag(FLAG_ZERO, X == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_TAY() {
	// Copies the current contents of the accumulator
	// into the Y register and sets the zero and negative flags as appropriate.
	Y = A;
	
	_Internal_SetFlag(FLAG_ZERO, Y == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, Y & 0b10000000);
}
void fs6502::_Instruction_TSX() {
	// Copies the current contents of the stack
	// register into the X register and sets the
	// zero and negative flags as appropriate.
	X = SP;
	
	_Internal_SetFlag(FLAG_ZERO, X == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, X & 0b10000000);
}
void fs6502::_Instruction_TXA() {
	// Copies the current contents of the X register into
	// the accumulator and sets the zero and negative flags as appropriate.
	A = X;
	
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
}
void fs6502::_Instruction_TXS() {
	//Copies the current contents of the X register into the stack register.
	SP = X;
}
void fs6502::_Instruction_TYA() {
	// Copies the current contents of the
	// Y register into the accumulator and
	// sets the zero and negative flags as appropriate.
	A = Y;
	
	_Internal_SetFlag(FLAG_ZERO, A == 0);
	_Internal_SetFlag(FLAG_NEGATIVE, A & 0b10000000);
}

void fs6502::StepInstruction(int IN_Num) {
	// Fetch next instruction
	Fetch();
	
	// Decode Instruction
	(*this.*CurrInstruction.FunctionPtr)();
	CycleNum += CurrInstruction.Cycles;
}

void fs6502::StepCycle(int IN_Num) {
	if (isFetchNeeded) {
		Fetch();
		isFetchNeeded = false;
		CurrInstruction.CycleLeft = CurrInstruction.Cycles;
	}
	
	if (CurrInstruction.CycleLeft > IN_Num) {
		CurrInstruction.CycleLeft -= IN_Num;
		CycleNum += IN_Num;
		return;
	}
	else {
		CurrInstruction.CycleLeft = 0;
		isFetchNeeded = true;
		(*this.*CurrInstruction.FunctionPtr)();
		CycleNum += IN_Num;
		return;
	}
}

fsByte fs6502::Read(fsWord IN_Address) { return Memory[IN_Address]; }
void fs6502::Write(fsWord IN_Address, fsByte IN_Value) { Memory[IN_Address] = IN_Value; }
void fs6502::Write(fsWord IN_Address, fsWord IN_Value) {
	
}
	
void fs6502::Fetch() {
	// Grab Next Instruction
	IR = Read(PC++);
	CurrInstruction = _Internal_DecodeInstruction();
	
	// Handle Addressing Mode
	_Internal_DecodeAddressing();
}

void fs6502::Reset() {
	// 
	CycleNum = 0;
	
	// 6502_Internal.html
	// Cycle 0: When a 6502 is turned on, the stack pointer is initialized with zero.
	// The BRK/IRQ/NMI/RESET sequence pulls the instruction register (IR) to 0.
	// #0 AB:00FF D:00 R/W:1 PC:00FF A:AA X:00 Y:00 SP:00 P:02 IR:00  READ $00FF = $00
	PC = 0x00FF;
	A = 0xAA;
	X = 0;
	Y = 0;
	SP = 0;
	IR = 0;
	CycleNum++;
	
	//Cycle 3: The first stack access happens at address $0100
	// – a push first stores the value at $0100 + SP,
	// then decrements SP. In the BRK/IRQ/NMI case,
	// this would have stored the high-byte of the PC.
	// But for RESET, it is a read cycle, not a write cycle, and the result is discarded.
	//#1 AB:00FF D:00 R/W:1 PC:00FF A:AA X:00 Y:00 SP:00 P:02 IR:00  READ $00FF = $00
	//#2 AB:00FF D:00 R/W:1 PC:00FF A:AA X:00 Y:00 SP:00 P:02 IR:00  READ $00FF = $00
	//#3 AB:0100 D:00 R/W:1 PC:00FF A:AA X:00 Y:00 SP:00 P:02 IR:00  READ $0100 = $00
	CycleNum += 3;
	
	// Cycle 4: SP is now 0xFF (even if the internal state does not reflect that),
	// so the second stack access (which would have been the low-byte of PC) targets 0x01FF.
	// Again, the result is discarded, and SP decremented.
	//#4 AB:01FF D:00 R/W:1 PC:00FF A:AA X:00 Y:00 SP:00 P:02 IR:00  READ $01FF = $00
	CycleNum++;
	
	// Cycle 5: SP is now 0xFE, and the third stack access, (the status register) happens at 0x01FE.
	// SP is decremented again.
	// #5 AB:01FE D:00 R/W:1 PC:00FF A:AA X:00 Y:00 SP:00 P:02 IR:00  READ $01FE = $00
	CycleNum++;
	
	//Cycle 6: The internal state of the CPU now shows that SP is 0xFD,
	// because it got decremented 3 times for the three fake push operations.
	// The low-byte of the vector is read.
	//#6 AB:FFFC D:E2 R/W:1 PC:00FF A:AA X:00 Y:00 SP:FD P:06 IR:00  READ $FFFC = $E2
	SP = 0xFD;
	fsByte L = Read(0xFFFC);
	CycleNum++;
	
	// Cycle 7: The high-byte of the vector is read.
	// #7 AB:FFFD D:FC R/W:1 PC:00FF A:AA X:00 Y:00 SP:FD P:16 IR:00  READ $FFFD = $FC
	fsByte H = Read(0xFFFD);
	CycleNum++;
	
	// Cycle 8: The first actual instruction is fetched.
	// #8 AB:FCE2 D:A2 R/W:1 PC:FCE2 A:AA X:00 Y:00 SP:FD P:16 IR:00  READ $FCE2 = $A2
	CycleNum++;
	PC = L;
	PC |= H << FS_SIZE_OF_BYTE;
}
