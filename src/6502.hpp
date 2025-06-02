#pragma once

#include <cstdint>
#include <vector>
#include <string>

typedef uint8_t		fsByte;
typedef uint16_t	fsWord;
typedef uint64_t	fsLong;
typedef int8_t		fsSByte;

#define FS_BYTE_MAX 255
#define FS_SIZE_OF_BYTE 8
#define FS_SIZE_OF_WORD 16
#define FS_SIZE_OF_MEMORY 64 * 1024

enum class fsMemoryAddressingMode {
	UNDEFINED,
	IMPLIED,
	ACCUMULATOR,
	IMMEDIATE,
	ZERO_PAGE,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	RELATIVE,
	ABSOLUTE,
	ABSOLUTE_X,
	ABSOLUTE_Y,
	INDIRECT,
	INDIRECT_X,
	INDIRECT_Y
};

#define FS_ADR_UNDEFINED		fsMemoryAddressingMode::UNDEFINED
#define FS_ADR_IMPLIED			fsMemoryAddressingMode::IMPLIED
#define FS_ADR_ACCUMULATOR		fsMemoryAddressingMode::ACCUMULATOR
#define FS_ADR_IMMEDIATE		fsMemoryAddressingMode::IMMEDIATE
#define FS_ADR_ZERO_PAGE		fsMemoryAddressingMode::ZERO_PAGE
#define FS_ADR_ZERO_PAGE_X		fsMemoryAddressingMode::ZERO_PAGE_X
#define FS_ADR_ZERO_PAGE_Y		fsMemoryAddressingMode::ZERO_PAGE_Y
#define FS_ADR_RELATIVE			fsMemoryAddressingMode::RELATIVE
#define FS_ADR_ABSOLUTE			fsMemoryAddressingMode::ABSOLUTE
#define FS_ADR_ABSOLUTE_X		fsMemoryAddressingMode::ABSOLUTE_X
#define FS_ADR_ABSOLUTE_Y		fsMemoryAddressingMode::ABSOLUTE_Y
#define FS_ADR_INDIRECT			fsMemoryAddressingMode::INDIRECT
#define FS_ADR_INDIRECT_X		fsMemoryAddressingMode::INDIRECT_X
#define FS_ADR_INDIRECT_Y		fsMemoryAddressingMode::INDIRECT_Y

class fs6502 {
private:
	
protected:
	struct fsInstruction {
		std::string OpCodeStr;
		fsMemoryAddressingMode AddressingMode;
		fsByte OpCode;
		fsByte Bytes;
		fsByte Cycles;
		void (fs6502::*FunctionPtr)(void);
		
		// Calculated by fetch based on addressing mode
		fsByte Operand = 0;
		fsWord OperandAdr = 0;
		fsByte CycleLeft = 0;
		
		fsInstruction() {}
		fsInstruction(std::string IN_OpCodeStr, fsMemoryAddressingMode IN_AddressingMode, fsByte IN_OpCode, fsByte IN_Bytes, fsByte IN_Cycles, void (fs6502::*IN_FunctionPtr)(void)) {
			OpCodeStr = IN_OpCodeStr;
			AddressingMode = IN_AddressingMode;
			OpCode = IN_OpCode;
			Bytes = IN_Bytes;
			Cycles = IN_Cycles;
			FunctionPtr = IN_FunctionPtr;
		}
	};
	
	fsByte Memory[FS_SIZE_OF_MEMORY] = {0};
	
	
	fsWord PC;		// Program Counter
	fsWord SP;		// Stack Pointer
	
	fsByte A;		// Accumulator
	fsByte X;		// Index Register X
	fsByte Y;		// Index Register Y
	
	fsByte Status = 0b00000000;
	//				  ||||||||
	//				  |||||||+-- Carry
	//				  ||||||+--- Zero
	//				  |||||+---- Interrupt Disable
	//				  ||||+----- Decimal
	//				  |||+------ Break Command
	//				  ||+------- Not used
	//				  |+-------- Overflow
	//				  +--------- Negative
	
	
	fsByte IR = 0;				// Instruction register, hold the value of the current instruction
	fsLong CycleNum = 0;		// Count the number of cycles since last reset
	
	std::vector<fsInstruction> InstructionTable;
	int InstructionTableNum = 0;
	fsInstruction CurrInstruction;
	// Used by Step Cycle
	bool isFetchNeeded = true;
	
	void _Internal_BuildInstructionTable();
	
	// Decode the current Instruction
	// IR expected to be already populated
	// as this function evaluates the current content of IR
	fs6502::fsInstruction _Internal_DecodeInstruction();
	
	// Decoding Memory Addressing and load data if needed
	// CurrInstruction expected to be populated 
	// as this function evaluates the current content of CurrInstruction
	void _Internal_DecodeAddressing();
	
	void _Internal_SetFlag(fsByte IN_Flag, bool IN_isSet);
	
	void _Instruction_ADC(); void _Instruction_AND();
	void _Instruction_ASL(); void _Instruction_BCC(); void _Instruction_BCS();
	void _Instruction_BEQ(); void _Instruction_BIT(); void _Instruction_BMI();
	void _Instruction_BNE(); void _Instruction_BPL(); void _Instruction_BRK();
	void _Instruction_BVC(); void _Instruction_BVS(); void _Instruction_CLC();
	void _Instruction_CLD(); void _Instruction_CLI(); void _Instruction_CLV();
	void _Instruction_CMP(); void _Instruction_CPX(); void _Instruction_CPY();
	void _Instruction_DEC(); void _Instruction_DEX(); void _Instruction_DEY();
	void _Instruction_EOR(); void _Instruction_INC(); void _Instruction_INX();
	void _Instruction_INY(); void _Instruction_JMP(); void _Instruction_JSR();
	void _Instruction_LDA(); void _Instruction_LDX(); void _Instruction_LDY();
	void _Instruction_LSR(); void _Instruction_NOP(); void _Instruction_ORA();
	void _Instruction_PHA(); void _Instruction_PHP(); void _Instruction_PLA();
	void _Instruction_PLP(); void _Instruction_ROL(); void _Instruction_ROR();
	void _Instruction_RTI(); void _Instruction_RTS(); void _Instruction_SBC();
	void _Instruction_SEC(); void _Instruction_SED(); void _Instruction_SEI();
	void _Instruction_STA(); void _Instruction_STX(); void _Instruction_STY();
	void _Instruction_TAX(); void _Instruction_TAY(); void _Instruction_TSX();
	void _Instruction_TXA(); void _Instruction_TXS(); void _Instruction_TYA();
	
	
	static const fsByte FLAG_CARRY		= 0b00000001;
	static const fsByte FLAG_ZERO		= 0b00000010;
	static const fsByte FLAG_INTERUPT	= 0b00000100;
	static const fsByte FLAG_DECIMAL	= 0b00001000;
	static const fsByte FLAG_BREAK		= 0b00010000;
	//static const fsByte FLAG_NOT_USED	= 0b00100000;
	static const fsByte FLAG_OVERFLOW	= 0b01000000;
	static const fsByte FLAG_NEGATIVE	= 0b10000000;
public:
	fs6502();
	
	// Step a specified amount of instruction (regardless of how many cycles it take)
	void StepInstruction(int IN_Num = 1);
	
	// Step a specified amaount of cycles
	void StepCycle(int IN_Num = 1);
	
	fsByte Read(fsWord IN_Address);
	void Write(fsWord IN_Address, fsByte IN_Value);
	void Write(fsWord IN_Address, fsWord IN_Value);
	
	// Fetches the next instruction and any additional bytes it may need
	void Fetch();
	void Reset();
	
	// -------------
	fsByte GetA() { return A; }
	fsByte GetX() { return X; }
	fsByte GetY() { return Y; }
	fsByte GetIR() { return IR; }
	fsByte GetStatus() { return Status; }
	fsWord GetPC() { return PC; }
	fsWord GetSP() { return SP; }
};







