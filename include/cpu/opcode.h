/* COPYRIGHT (C) HARRY CLARK 2024 */

/* SEGA MEGA DRIVE EMULATOR */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY SURROUNDING THE CPU'S OPCODES */
/* PROVIDING MODULARISATION OF FUNCTIONS TO DECLUTTER RELEVANT HEADER FILES */

#ifndef M68K_OPCODES
#define M68K_OPCODES

/* SYSTEM INCLUDES */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(USE_68000_OP)
#define USE_68000_OP
#else
#define USE_68000_OP

#define M68K_MAKE_OPCODE(OP, SIZE, MODE, REG) \
    void OP##_##SIZE##_##MODE##_##REG(void)

#define M68K_CONCAT(A, B) A##B
#define M68K_STRINGIFY(X) #X
#define M68K_IS_DIGIT(X) ((X) >= '0' && (X) <= '9')

/* HELPER MACROS FOR CONCATENATING TYPES OUT OF BITWISE ELEMENTS */
/* THIS WILL BE USEFUL FOR THE VARIDATIC ARGS FOR HANDLING BINARY EXCEPTIONS */


#define M68K_MAKE_EXCE_OP(OPCODE, SIZE, MODE, REG) \
    void M68K_##OPCODE##_##SIZE##_##MODE##_##REG(void)

#endif
#endif
