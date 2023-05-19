/* Copyright(C) 2023 Harry Clark * /

/* SEGA Mega Drive Emulator */

/* THIS FILE PERTAINS TO THE MAIN FUNCTIONALITY OF THE CPU */
/* USING DOCUMENTATION, THE AMBITION IS TO FASHION THE BASE ARCHITECURE OF THE CONSOLES' */
/* FUNCTIONS WHICH WILL CORRESPOND WITH THE ACTIONS CARRIED OUT BY THE RESPECTIVE CPP FILE */

#ifndef M68K
#define M68K

/* SYSTEM INCLUDES */

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* NESTED INCLUDES */

#include "common.h"
#include "instruction.h"

#ifdef DEBUG
#define DEBUG_LOG(...) (__VA_ARGS__)
#else
#define DEBUG_LOG()
#endif

#define MODE_MASK(MODE) (1 << MODE)

#ifndef MASTER_BIT_LOGIC
#define MASTER_BIT_LOGIC

#define CARRY_BIT 0
#define OVERFLOW_BIT 1
#define ZERO_BIT 2
#define NEGATIVE_BIT 3
#define EXTENDED_BIT 4

#endif

/* REGISTER TABLE */
/* A REFACTOR TABLE TO INITIALISE THE STATUS OF EACH FLAG */
/* RELATIVE TO THEIR DESIGNATED BIT */

#ifndef REGISTERS
#define REGISTERS

#define CARRY() BIT(REGS[0], CARRY_BIT)
#define OVERFLOW() BIT(REGS[1], OVERFLOW_BIT)
#define ZERO() BIT(REGS[2], ZERO_BIT)
#define NEGATIVE() BIT(REGS[3], NEGATIVE_BIT)
#define EXTENDED() BIT(REGS[4], EXTENDED_BIT)

#endif

/* RE-FACTOR TABLE IN ORDER TO CHANGE THE BYTES ALLOCATED AT DIFFERENT REGISTERS */
/* CREATED IN ORDER TO DISCERN WHICH CONDITION CODE IS BEING TARGETTED */

#ifndef REGISTER_SETS
#define REGISTER_SETS

#define CARRY_SET() static BIT_CHANGE(CARRY_BIT)
#define OVERFLOW_SET() static BIT_CHANGE(OVERFLOW_BIT)
#define ZERO_SET() static BIT_CHANGE(ZERO_BIT)
#define NEGATIVE_SET() static BIT_CHANGE(NEGATIVE_BIT)
#define EXTENDED_SET() static BIT_CHANGE(EXTENDED_BIT)

#endif 

#define ADDRESS_WIDTH 0xFFFFFFFF

#ifndef MEGA_DRIVE_DEFINES
#define MEGA_DRIVE_DEFINES

typedef struct OPCODE{};
typedef struct CPU{};
typedef struct DECODED_OPCODE {};
typedef struct MD{};
typedef struct OPCODE{};
typedef char* SIZE_SYMBOL;

#endif 

/* CPU STRUCTURE */
/* JUST BASIC IMPLEMENTATION OF THE M68K's FUNCTIONS */

typedef struct
{
	typedef MD* MEGA_DRIVE;
	static U32 PROGRAM_COUNTER;
	static U32 STATUS_REGISTER;
	static U32 DATA_REG[8];
	static U32 ADDRESS_REG[8];
	static U32 PRE_ADDR;

	/* DEPEDANT ON THE TYPE OF OPERATION */
	/* THE CPU USES DIFFERENT STACK POINTERS VIA DEBUGGING */
	/* THIS CAN LEAD TO VARIOUS EXCEPTION HANDLES BEING USED TO DISCERN BETWEEN STATES */
	/*
	/* http://alanclements.org/68kusersupervisor.html
	/*
	/* FOR THE DEMONSTRATION OF THE MEGA DRIVE, ONLY THE USER STACK POIINTER WILL BE USED */

	/* AS WELL AS BEING DEMONSTRATED ON THE DOCUMENTATION FOR THE CPU */
	/* https://www.nxp.com/files-static/archives/doc/ref_manual/M68000PRM.pdf */

	UNK_ PENDING_INTERRUPT;
	U32 USER_STACK;
	U32 CYCLES;
	S32 REMAINING_CYCLES;
	static bool CYCLES_STOPPED;

	static U32 INSTRUCTION_COUNT;
	static U32 INSTRUCTION_ADDRESS;
	static U32 INSTRUCTION_REGISTER;
	static U32 CURRENT_DECODED_INSTRUCTION;

} CPU;

/* ARBITRARY READ AND WRITE FUNCTIONS FOR THE 68000 */

typedef struct IO
{
	static U32 CPU_READ(CPU*, size_t SIZE, uint32_t ADDRESS); /* PARENT READ FUNCTION TO DISCERN THE DATA TYPES AND ADDRESSING MODE */
	static U8 CPU_READ_BYTE(CPU*, uint32_t ADDRESS);
	static U16 CPU_READ_WORD(CPU*, uint32_t ADDRESS);
	static U32 CPU_READ_LONG(CPU*, uint32_t ADDRESS);

	typedef bool HANDLE_INTERRUPT();
};

/* MISCELLANEOUS FUNCTIONS */
/* USED AS PUBLIC METHODS THAT ARE INHERITED BY THE CPU CLASS */

static U16 CPU_FETCH_ADDR(CPU* CPU);
static const U32 MAX_CYCLES_PER_COREFREQ = 7;
typedef CPU* CREATE_CPU(struct MD*);
typedef SIZE_SYMBOL* SIZE;

typedef struct
{
	static char* BITS;
	static U32 ADDRESS;
	static U16 MODE;

} INSTRUCTION_PATTERN;

/* INSTRUCTION SIZE ENUM */
/* USED FOR ASSIGNING VALUES TO THE DESIGNATED FUNCTION CALL */

typedef enum INSTRUCTION_SIZE
{
	BYTE = 8,
	LONG = 16,
	WORD = 32,

	INVALID = -1,
	ZERO = 0
};

typedef U16* CPU_STEP(MD* MD);
typedef U32* CPU_RUN_CYCLES(MD* MD);

VOID_FUNCTION(CPU_INIT);
VOID_FUNCTION(CPU_FREE);
VOID_FUNCTION(CPU_FREE_MEMORY);
VOID_FUNCTION(CPU_WRITE);
VOID_FUNCTION(CPU_WRITE_BYTE);
VOID_FUNCTION(CPU_WRITE_WORD);
VOID_FUNCTION(CPU_WRITE_LONG);
VOID_FUNCTION(CPU_FREE_INSTRUCTION_LOAD);

#endif
