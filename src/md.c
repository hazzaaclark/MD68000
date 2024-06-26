/* COPYRIGHT (C) HARRY CLARK 2024 */

/* SEGA MEGA DRIVE EMULATOR */

/* THIS FILE PERTAINS TOWARDS THE MAIN FUNCTIONALITY OF THE CONSOLE */

/* NESTED INCLUDES */

#include "md.h"
#include "68000.h"
#include "vdp.h"
#include "common.h"

#ifdef USE_MD

/* INITIALISE THE CONSOLE THROUGH THE PRE-REQUISTIES */
/* ESTABLISHED IN THE CORRESPONDING HEADER FILES */

/* THIS FUNCTION ENCOMPASSES THE FUNCTIONALITY OF ENABLING */
/* THE MEMORY MANAGEMENT UNIT FOR ALLOWING THE CONSOLE TO BEGIN */
/* IT'S INITIAL COMMUNICATIONS BETWEEN M68K AND Z80 ON STARTUP */

void MD_INIT(void)
{
    struct MD* MD_CONSOLE = malloc(sizeof(MD));
    struct CPU_68K* CPU_68K = malloc(sizeof(CPU_68K));

    if((MD_CONSOLE->SYSTEM_TYPE = SYSTEM_MD))
    {
        /* M68K_INIT(); */
        CPU_68K->ADDRESS_RT_CHECK = malloc(sizeof(CPU_68K->ERROR_ADDRESS));

        /* ASSUMING THAT THE ABOVE COROUTINE HAVE BEEN ESTABLISHED */
        /* THE MEMORY MAP WILL NOW BE INITIALISED */

        /* THIS FUNCTIONALITY IS SIMILAR TO THE WAY IN WHICH IT WORKS */
        /* ON REAL HARDWARE WITH THE WAY IN WHICH THE VECTOR TABLE INITIALISES */ 

        for (int i = 0; i < 0xFF; i++)
        {
            CPU_68K->MEMORY_MAP[i].BASE = malloc(sizeof(MD_CONSOLE->BOOT_RAM));
            CPU_68K->MEMORY_MAP[i].MEMORY_READ_8 += M68K_READ_8(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
            CPU_68K->MEMORY_MAP[i].MEMORY_WRITE_8 += M68K_WRITE_8(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
            CPU_68K->MEMORY_MAP[i].MEMORY_READ_16 += M68K_READ_16(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
            CPU_68K->MEMORY_MAP[i].MEMORY_WRITE_16 += M68K_WRITE_16(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);

            CPU_68K->Z80_MEM[i].READ = Z80_READ(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
            CPU_68K->Z80_MEM[i].WRITE = Z80_WRITE(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
        }

        /* VDP ENTRY POINTS */

        for (int i = 0xC; i < 0xFF; i++)
        {
            CPU_68K->MEMORY_MAP[i].MEMORY_READ_8 +=  VDP_READ_BYTE(CPU_68K->MEMORY_DATA, (U8*)CPU_68K->MEMORY_ADDRESS);
            CPU_68K->MEMORY_MAP[i].MEMORY_WRITE_8 += VDP_WRITE_BYTE(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
            CPU_68K->MEMORY_MAP[i].MEMORY_READ_16 += VDP_READ_WORD(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
            CPU_68K->MEMORY_MAP[i].MEMORY_WRITE_16 += VDP_WRITE_WORD(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
        }

        /* EVALUABLE RAM HEADER */

        for (int i = 0; i < 0xc100; i++)
        {
            CPU_68K->MEMORY_MAP[i].BASE = malloc(sizeof(MD_CONSOLE->BOOT_RAM));
            MD_CONSOLE->ZBANK[i] = NULL;
        }

        /* IO CONTROL REGISTERS */

        for (int i = 0; i < 0xA100; i++)
        {
            CPU_68K->MEMORY_MAP[0xA1].MEMORY_READ_8 = CTRL_READ_BYTE(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
            CPU_68K->MEMORY_MAP[0xA1].MEMORY_WRITE_8 = CTRL_WRITE_BYTE(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
            CPU_68K->MEMORY_MAP[0xA1].MEMORY_READ_8 = CTRL_READ_WORD(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
            CPU_68K->MEMORY_MAP[0xA1].MEMORY_WRITE_8 = CTRL_WRITE_WORD(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);

        }
    }

    M68K_INIT();
    MD_CART_INIT();
    MD_MAKE();
}

/* AFTER THE INITIALISATION OF THE MEMORY MAP */
/* THE CONSOLE EMULATION WILL COMMENCE BY ALLOCATING CORRESPONDING MEMORY */
/* TO THE STACK */

void MD_MAKE()
{
    /* ALLOCATE MEMORY FOR THE BASE STRUCTURE WHICH HOUSES */
    /* THE CORRESPONDENCE OF EACH COMPONENT, CPU AND CONSOLE */

    struct MD* MD_CONSOLE = malloc(sizeof(MD_CONSOLE));

    MD_CONSOLE->SYS_ROM = (U8*)malloc(sizeof(MD_CONSOLE->SYS_ROM));
    MD_CONSOLE->SYS_RAM = (U8*)malloc(sizeof(MD_CONSOLE->SYS_RAM));
}

/* NOW COMES THE COROUTINE FOR RESETTING THE CONSOLE */
/* THIS WILL DETERMINE BY AN NUMERICAL VALUE TO DISCERN THE RESET TYPE */

/* WHEN IT COMES TO RESET METHODS ON THE MEGA DRIVE, ESPECIALLY IN THE HEADER */
/* OF THE MAIN ASSEMBLY FILE, IT INVOLVES THE MOVE INSTRUCTION OF THE VALUE FROM D7 */
/* INTO ONE OF THREE ADDRESSING MODES */

/* A1, A2 & A3 ENCOMPASS THE INITIAL STEPS FOR HARDWARE COROUTINE CHECKS */
/* AS THESE ARE THE MAIN 3 REGISTERS THAT COMMUNICATE WITH THE BUS */

/* D7 ACTS AS THE STACK POINTER TO DETERMINE WHERE THE DATA SHOULD GO TOWARDS */

/* SEE 68K INSTRUCTION REF. https://md.railgun.works/index.php?title=68k_Instruction_Reference */

void MD_RESET(void)
{

    struct MD* MD_CONSOLE = malloc(sizeof(MD)); 
    struct CPU_68K* CPU_68K = malloc(sizeof(CPU_68K));
    MD_RESET_MODE MODE = 0;

    switch (MODE)
    {
        /* SOFT RESET EVOKES THE METHODS USED TO */
        /* RESET THE CONSOLE FROM A SOFTWARE */
        /* THIS WILL BE GOVERNED BY THE STACK POINTER */
        /* STORING THE LOCATION OF THE BOOT RAM CACHE */

        /* FROM THERE, BOOT BACK TO THE START OF THE PROGRAM EXECUTION */
        /* IN RELATION TO THE BOOT RAM GOVERNED BY IT'S DESIGNATED DATA REGISTER */

        case MODE_SOFT:
            CPU_68K->PC = MD_CONSOLE->BOOT_RAM;
            CPU_68K->STACK_POINTER = 0x2700;
            CPU_68K->REGISTER_BASE[7] = MD_CONSOLE->BOOT_RAM;
            break;

        /* HARD RESET ENVOKES THAT ALL ASPECTS OF THE CONSOLE NEED TO BE */
        /* RESET INDICATIVE OF THE PRESSING OF THE RESET BUTTON */

        /* IN ASSEMBLY, THIS WOULD TYPICALLY ENTAIL USING THE RESET FLAG */
        /* TO RESET CPU EXECUTION BASED ON THEIR REGISTER COUNT */

        /* THE DIFFERENCE BEING IS THAT WE EVOKE MEMSET TO ASSERT ALL VALUES */
        /* BACK TO DEFAULT */

        case MODE_HARD:
            CPU_68K->PC = MD_CONSOLE->BOOT_RAM;
            CPU_68K->STACK_POINTER = 0x2700;
            CPU_68K->REGISTER_BASE[7] = MD_CONSOLE->BOOT_RAM;
            memset(MD_CONSOLE->BOOT_RAM, 0x00, sizeof(MD_CONSOLE->BOOT_RAM));
            memset(MD_CONSOLE->ZRAM, 0x00, sizeof(MD_CONSOLE->ZRAM));
            break;

        default:
            free(&MD_CONSOLE->MD_CART);
            free(&CPU_68K->Z80_MEM);
            free(CPU_68K);
    } 
}

/* THE BANK SWITCH FUNCTIONS LOOKS INTO THE CORRESPODENCE STORED IN */
/* THE ZBUFFER TO DETERMINE THE OFFSET OF MEMORY ALLOCATIONS */

/* THIS WILL CHECK TO SEE IF THE BOOT ROM HAS BEEN LOADED AND IF SO */
/* MIMMICK THE FUNCTIONALITY OF THE JUMP COROUTINE TO INITIALISE THE START OF THE CART */

U32* MD_BANKSWITCH()
{
    struct MD* MD_CONSOLE = malloc(sizeof(MD));
    struct CPU_68K* CPU_68K = malloc(sizeof(CPU_68K));

    /* IS THE BOOT ROM INITIALISED? */

    switch (MD_CONSOLE->SYSTEM_BIOS)
    {

        case 0:
            CPU_68K->MEMORY_MAP[0].BASE = (char*)MD_CONSOLE->BOOT_ROM;

        /* IS THE ROM LOADED IN THE MEMORY MAP? */

        case 1:
            CPU_68K->MEMORY_MAP[0].BASE = (char*)MD_CONSOLE->MD_CART->ROM_BASE;
            break;
        
        default:
            if(MD_CONSOLE->SYSTEM_BIOS == SYSTEM_MD)
                return (CPU_68K->MEMORY_MAP[0].BASE = (char*)MD_CONSOLE->MD_CART->ROM_BASE);
    }

    return ZBUFFER_MAX;
}

/* INITIALISE THE FUNCTIONALITY PERTAINING TOWARDS THE Z80 AS IT COMMUNICATES */
/* WITH THE CONSOLE */

/* THE FOLLOWING WILL ENCOMPASS ACCESS TO READ AND WRITE FROM THE 68K MEMORY MAP */
/* TO UPDATE IT'S STATUS IN RELATION TO THE COMMUNICATION WITH THE PC */

void MD_BUS_REQ(unsigned STATE, unsigned CYCLES)
{
    struct CPU_68K* CPU_68K = malloc(sizeof(CPU_68K));
    struct MD* MD_CONSOLE = malloc(sizeof(MD));

    assert(&STATE); /* EVALUATE THE INITIAL STATE */

    /* BEGIN BY SYNCHRONISING WITH THE 68K */
    /* THERE WILL BE A DIFFERENT FUNCTION IN PLACE BY IN THE MEANTIME */
    /* WE WILL EVALUATE IT'S PROBABLE MEMORY */

    if(MD_CONSOLE->ZSTATE == 1)
    {
        memset(&MD_CONSOLE, STATE, CYCLES);

        /* EVALUATE THE ACCESS BETWEEN THE 68K AND Z80 AFTER THE SYNC */

        CPU_68K->MEMORY_MAP[0xA0].MEMORY_READ_8 = Z80_READ(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
        CPU_68K->MEMORY_MAP[0xA0].MEMORY_WRITE_8 = Z80_WRITE(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
        CPU_68K->MEMORY_MAP[0xA0].MEMORY_READ_16 = Z80_READ(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS);
        CPU_68K->MEMORY_MAP[0xA0].MEMORY_WRITE_16 = Z80_WRITE(CPU_68K->MEMORY_DATA, CPU_68K->MEMORY_ADDRESS, CPU_68K->MEMORY_POINTER);
    }

    STATE |= 2;
}

/* USING RUNTIME OF THE CONSOLE, STORE RELEVANT REGISTERS AND THEIR STATES AND CONDITIONS */
/* SUCH THAT THE APPROPRIATE CONTEXT CAN BE MET DEPENDING OF WHICH OPERANDS ARE USING */
/* SAID REGISTERS */

/* SEE opcode.c & opcode.h FOR FURTHER READING */

void MD_SAVE_REGISTER_STATE(struct CPU_68K* CPU_68K)
{
    int INDEX;

    /* STORE THE MAIN 15 REGISTERS; DATA AND ADDRESS (MAIN M68000 CPU) */

    for (INDEX = 0; INDEX < 8; INDEX++)
        CPU_68K->REGISTER_BASE[INDEX] = CPU_68K->DATA_REGISTER[INDEX];

    for (INDEX = 0; INDEX < 7; INDEX++)
        CPU_68K->REGISTER_BASE[INDEX] = CPU_68K->ADDRESS_REGISTER[INDEX];

    /* THIS CHECKS TO SEE IF AND WHEN THE STACK POINTER COMES INTO INITIALISATION */
    /* THE STACK POINTER GOVERNS A 32 BIT UNSIGNED VALUE WHICH INCLUDES A 16 BIT SIGNED OPERAND */
    /* AND AN ARBITRARY MANTISSA */

    /* ONCE THE STACK HAS BEEN INITIALISED ON STARTUP (0x2000), A7 WILL */
    /* BEGIN TO LOAD CONTENTS FROM THE HEADER */

    if(CPU_68K->STACK_POINTER && 0x2000)
    {
        CPU_68K->STACK_POINTER = CPU_68K->ADDRESS_REGISTER[7];
        CPU_68K->USER_STACK = CPU_68K->ADDRESS_STACK_POINTER;
    }
    
    else
    {
        CPU_68K->STACK_POINTER = CPU_68K->ADDRESS_STACK_POINTER;
        CPU_68K->USER_STACK = CPU_68K->ADDRESS_REGISTER[7];
    }

    CPU_68K->PC += (int)malloc(sizeof(CPU_68K->PC));
    CPU_68K->STATUS_REGISTER += *(U16*)malloc(sizeof(CPU_68K->STATUS_REGISTER));

    memset(CPU_68K, 0x00, sizeof(*CPU_68K));
}

/* IN CONJUNCTION WITH THE BANKSWITCHING METHOD */
/* THIS FUNCTION ENCOMPASSES THE CURRENT FETCH INSTRUCTION TO UPDATE */
/* SAID BANK SWITCHING */

/* THIS IS BY TAKING INTO ACCOUNT HOW MANY MEMORY BANKS ARE AVAILABLE ON EACH CARTRIDGE */
/* AS SOME SUCH AS SONIC 2 HAS TMSS */

/* TMSS CAN INTERFERE WITH BANKSWITCHNG AS THE TRADEMARK SYSTEM */
/* ENCOMPASSES THE STATE BY WHICH WHETHER A ROM WILL LOAD ASSUMING IT DOESN'T FOLLOW PREREQ'S */ 

int MD_UPDATE_BANKING(struct CPU_68K* CPU_68K, int BANKS)
{
    struct MD* MD_CONSOLE = malloc(sizeof(MD));
    int CURRENT_BANK = 0;

    if(MD_CONSOLE->TMSS[CURRENT_BANK])
    {
        CURRENT_BANK = MD_UPDATE_BANKING(CPU_68K->TMSS_BASE[CURRENT_BANK], BANKS);
    }

    free(MD_CONSOLE);
    return CURRENT_BANK;
}

/* SUCH IS THE CASE WITH THE FUNCTION ABOVE */
/* WE WILL NOW DO THIS FOR THE CARTRIDGE */

/* SINVE THE CARTRIDGE NEEDS TO BE ABLE TO COMMUNICATE BETWEEN */
/* THE PC TO DETERMINE THE STATE */

int MD_CART_UPDATE_BANKING(struct CPU_68K* CPU_68K, int BANKS)
{
    struct MD_CART* MD_CARTRIDGE = malloc(sizeof(MD_CART));
    int BANKS_UPDATED = 0;
    int INDEX = 0;

    /* DOES THE CORRESPONDING AMOUNT OF BANKS RELATE */
    /* TO THE CURRENTLY AVAILABLE ON THE STACK */

    if((UNK)BANKS >= sizeof(MD_CARTRIDGE->CARTRIDGE_BANKS))
        BANKS += sizeof(MD_CARTRIDGE->CARTRIDGE_BANKS);

    /* EVALUATE THE HIGH AND LOW ADDRESSING MODES */
    /* THIS IS TO DETERMINE DATA TRANSFER BETWEEN THE BUS */

    for (INDEX = 0; INDEX < BANKS; INDEX++)
    {
        U32 ROM_ADDR_START = (0x80000 * (MD_CARTRIDGE->CARTRIDGE_BANKS[INDEX]));
        MD_CARTRIDGE->CARTRIDGE_BANKS[INDEX] = (U32)sizeof(ROM_ADDR_START);

        if(ROM_ADDR_START < MD_CARTRIDGE->ROM_SIZE)
        {
            CPU_68K->LOW_ADDR = ROM_ADDR_START;
            CPU_68K->HIGH_ADDR = ROM_ADDR_START + 0x7FFFF; /* ADD MASK */
            BANKS_UPDATED++;
        }
    }

    return BANKS_UPDATED;
}

/* INITIALISE THE CARTRIDGE COUROUTINE */
/* BY ESTABLISHING THE VARIABLE ROM SIZE, ALLOCATING MEMORY */
/* FOR THE ROM IMAGE AND DETERMINING VARIOUS INSTANCES OF THE BUFFER */

/* WHILE THE FUNCTION IS AN INT, IT WILL BE VOID AS THERE WILL BE NO LOCAL */
/* RETURN TYPE, JUST FOR BASIC CONDITIONS */

int MD_CART_INIT(void)
{
    struct MD_CART* MD_CARTRIDGE = malloc(sizeof(MD_CART));

    /* ASSUME TO BEGIN WITH THAT THERE IS NO CURRENT ROM */
    /* BEING LOADED ONTO THE STACK */

    if(!MD_CARTRIDGE->ROM_BASE)
    {
        free(MD_CARTRIDGE);
        return -1;
    }

    /* CHECK TO DETERMINE IF THE FILE IS TOO BIG */
    /* IN THE LUCKLIHOOD OF AN INCOMPLETE FILE TYPE */

    if(MD_CARTRIDGE->ROM_SIZE == CART_MAX_SIZE)
    {
        free(MD_CARTRIDGE);
        return -2;
    }

    /* ALLOCATE INITAL SPACE FOR THE CARTRIDGE */
    
    MD_CARTRIDGE->ROM_BASE += (U32)malloc(sizeof(MD_CARTRIDGE->ROM_SIZE));

    /* STORE THE INITIAL 512KB FOR SSE2 */
    /* THIS IS BY ALIGNING 16 BYTES OF ADDRESSABLE VECTOR TYPES TO THE HEADER */
    
    /* 17/02/24 - UPDATED THE DEFERENCING NULLPTR EXCEPTION */
    /* USING ALIGNED MALLOC FIXES THESE ISSUES BY ALIGNING FOR A SPECIFIC PIECE OF MEMORY */

    MD_CARTRIDGE->ROM_DATA = (void*)malloc((MD_CARTRIDGE->ROM_SIZE));

    /* AFTER ALLOCATING THE PROVIDED MEMORY TO THE STACK */
    /* BEGIN BY INITIALISAING THE MEMORY MAP OF THE CARTRIDGE */

    MD_CART_MEMORY_MAP();

    return 0;
}

/* DISCERN THE MEMORY MAP FOR THE CARTRIDGE'S ROM SIZE */
/* THIS IS BY TAKING INTO ACCOUNT SEVERAL FACTORS SUCH AS */
/* SETTING THE ROM MAP, SETTING MAPPER REGISTER BASED ON BANKING TYPE */

/* EACH MAPPER TYPE REPRESENTS AN ACTION TAKEN AT EACH SPECIFIC MEMORY ADDRESS */
/* ON THE HEADER */

/* FROM THERE, COPY THE REGISTER DATA TO THE DESIGNATED MAPPER */

void MD_CART_MEMORY_MAP(void)
{
    UNK INDEX;
    struct MD_CART* MD_CARTRIDGE = malloc(sizeof(MD_CART));
    struct CPU_68K* CPU_68K = malloc(sizeof(CPU_68K));
    int MAP_MODE = 0;

    switch (MAP_MODE)
    {
        case MAPPER_NORMAL:
            for (INDEX = 0; INDEX < sizeof(MD_CARTRIDGE->CARTRIDGE_BANKS); INDEX++)
                MD_CARTRIDGE->CARTRIDGE_BANKS[INDEX] = MD_CART_BANK_DEFAULT;
            break;

        case MAPPER_READONLY:
            for (INDEX = 0; INDEX < 8; INDEX++)
                MD_CARTRIDGE->CARTRIDGE_BANKS[INDEX] = MD_CART_BANK_RO;

            memcpy(&MD_CARTRIDGE->REGISTER_READ, &CPU_68K->REGISTER_BASE, sizeof(MD_CARTRIDGE->REGISTER_READ));

            break;
    }
}

#endif
