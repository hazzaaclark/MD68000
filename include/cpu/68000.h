#ifndef M68K_H
#define M68K_H

#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>

#define REGISTERS
uint32_t DATA_REG[8]{};
uint32_t ADDR_REG[8]{};
uint32_t PROGRM_COUNTER{};
uint32_t STATUS_REG{};
uint8_t INDEX_REGISTER{};
uint8_t NEGATIVE_REG{};
uint8_t ZERO_REG{};
uint8_t OVERFLOW_REG{};
uint8_t CARRY_OP_REG{};
unsigned int NON_MASKABLE_IR{};
unsigned int INTERRUPT_REQUEST{};


#define STACK
unsigned int CURRENT_STACK_POINTER;
uint32_t STACK_POINTER{};

#define CPU_FUNC
static void BUS_INIT();
static void CPU_READ();
inline void CPU_RESET();
inline void REG_INTERRUPT();
inline void TIMER();
extern bool CYCLECOMPLETE();
extern bool RESET_JAMMED;
extern bool RESETTING;

#define FLAGS
const char* C;
const char* X;
const char* Z;
const char* V;
const char* N;

#endif
