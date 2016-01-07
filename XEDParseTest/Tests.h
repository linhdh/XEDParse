#pragma once

#define ENTRY(LongMode, Ip, Len, Data, Instr) LongMode, Ip, Len, Data, Instr

struct XED_TestEntry
{
    bool LongMode;          // 64-bit
    unsigned __int64 Ip;    // Instruction address
    int DataLen;            // Length of data (-1 indicates invalid instruction)
    const char* Data;       // Raw hex output
    const char* Asm;        // Assembler string
};

static XED_TestEntry XED_AllTests[] =
{
    // 32|64, Base IP, Hexcode length, Expected hexcode, Input
    { ENTRY(false, 0x77513BEE, 2, "\xEB\xFE",                   "JMP SHORT 0x77513BEE") },              // 32 Short jump
    { ENTRY(false, 0x77513BEE, 2, "\xEB\x07",                   "JMP SHORT 0x77513BF7") },              // 32 Short jump forward
    { ENTRY(false, 0x77513BEE, 2, "\xEB\xCF",                   "JMP SHORT 0x77513BBF") },              // 32 Short jump backward

    { ENTRY(false, 0x00405C5B, 5, "\xE9\xCA\x70\x00\x00",       "JMP 0x0040CD2A") },                    // 32 Long jump forward
    { ENTRY(false, 0x00405C5B, 5, "\xE9\x40\xFE\xFF\xFF",       "JMP 0x00405AA0") },                    // 32 Long jump backward
    { ENTRY(false, 0x00405C5B, 6, "\xFF\x25\x00\x01\x00\x00",   "JMP DWORD PTR DS:[100]") },            // 32 Long jump ptr

    { ENTRY(false, 0x00405CCC, 6, "\xFF\x15\x10\x00\x00\x00",   "CALL DWORD PTR DS:[10]") },            // 32 Long call ptr
    { ENTRY(false, 0x00405C5B, 5, "\xE8\xCA\x70\x00\x00",       "CALL 0x0040CD2A") },                   // 32 Long call forward
    { ENTRY(false, 0x00405EF0, 5, "\xE8\x06\xFF\xFF\xFF",       "CALL 0x00405DFB") },                   // 32 Long call backward

    { ENTRY(true, 0x7FFCAA022104, 2, "\xEB\xFE", "JMP SHORT 7FFCAA022104") },                           // 64 Short jump
    { ENTRY(true, 0x7FFCAA022104, 2, "\xEB\x22", "JMP SHORT 7FFCAA022128") },                           // 64 Short jump forward
    { ENTRY(true, 0x7FFCAA022104, 2, "\xEB\xF9", "JMP SHORT 7FFCAA0220FF") },                           // 64 Short jump backward

    { ENTRY(true, 0x7FFCA9FF197C, 5, "\xE9\x35\x64\x93\x53", "JMP 0x7FFCFD927DB6") },                   // 64 Long jump forward
    { ENTRY(true, 0x7FFCAA022104, 5, "\xE9\x7C\xF4\xFC\xFF", "JMP 0x7FFCA9FF1585") },                   // 64 Long jump backward
    { ENTRY(true, 0x000123456789, 6, "\xFF\x25\xFA\xFF\xFF\xFF", "JMP QWORD[0x123456789]") },           // 64 Long jump ptr with RIP-rel
    { ENTRY(true, 0x7FFCA9FF1977, 6, "\xFF\x25\xFA\x00\xFF\xFF", "JMP QWORD PTR DS:[7FFCA9FE1A77]") },  // 64 Long jump ptr backward with RIP-rel

    { ENTRY(false, 0x00405C6A, 6, "\xFF\x35\xF4\x0A\x47\x00",           "PUSH DWORD PTR DS:[470AF4]") },
    { ENTRY(false, 0x00405C92, 3, "\x8B\x45\x08",                       "MOV EAX,DWORD PTR SS:[EBP+8]") },
    { ENTRY(false, 0x00405CB8, 7, "\xC7\x45\xF4\x00\x40\x99\x01",       "MOV DWORD PTR SS:[EBP-0C],01994000") },
    { ENTRY(false, 0x00405D4C, 3, "\x8A\x14\x08",                       "MOV DL,BYTE PTR DS:[EAX+ECX]") },
    { ENTRY(false, 0x00405D4C, 5, "\x0F\xB7\x4C\x45\x98",               "MOVZX ECX,WORD PTR SS:[EAX*2+EBP-68]") },
    { ENTRY(false, 0x00405D4C, 8, "\xC6\x84\x30\x1D\x01\x00\x00\x00",   "MOV BYTE PTR DS:[EAX+ESI+11D],0") },
    { ENTRY(false, 0x0040652A, 5, "\xC0\x64\x18\x50\xFF",               "SAL BYTE PTR DS:[EAX+EBX+50],0FF") },
    { ENTRY(false, 0x00405F1E, 2, "\x6A\x0D",                           "PUSH 0D") },
    { ENTRY(false, 0x00405F1E, 5, "\x68\xFF\x00\x00\x00",               "PUSH FF") },
    { ENTRY(false, 0x00405FF9, 4, "\x83\x60\x70\xFD",                   "AND DWORD PTR DS:[EAX+70],FFFFFFFD") },
    { ENTRY(false, 0x00405FF9, 7, "\x81\x60\x70\x0D\x00\x00\xF0",       "AND DWORD PTR DS:[EAX+70],F000000D") },
    { ENTRY(false, 0x00405C23, 8, "\xf2\x0f\x11\x0d\x00\x00\x00\x00",   "MOVSD QWORD PTR ds:[0x0],xmm1") },
    { ENTRY(false, 0x00405C23, 7, "\x8B\x04\xCD\x00\x00\x00\x00",       "MOV EAX,[ECX*8]") },
    { ENTRY(false, 0x00405C23, 1, "\x60",                               "PUSHAD") },
    { ENTRY(false, 0x00405C23, 1, "\xCC",                               "INT3") },
    { ENTRY(false, 0x00405C23, 2, "\xCD\x03",                           "INT 3") },
    { ENTRY(false, 0x00405C23, 2, "\xE7\xE9",                           "OUT 0xE9, EAX") },
    { ENTRY(false, 0x00405C23, 6, "\x69\xC0\xFF\x01\x00\x00",           "IMUL EAX, EAX, 0x1FF") },
    { ENTRY(false, 0x00405C23, 6, "\x69\xC0\xFF\x00\x00\x00",           "IMUL EAX, EAX, 0xFF") },
    { ENTRY(false, 0x00405C23, 6, "\x69\xC0\xFE\x00\x00\x00",           "IMUL EAX, EAX, 0xFE") },
    { ENTRY(false, 0x00405C23, 5, "\xB8\x78\x56\x34\x12",               "MOV EAX, 0x12345678") },
    { ENTRY(false, 0x00405C23, 5, "\xB8\xFE\xFF\xFF\xFF",               "MOV EAX, 0xFFFFFFFE") },
    { ENTRY(false, 0x00405C23, 3, "\xDF\x3C\x24",                       "FISTP QWORD PTR [ESP]") },

    { ENTRY(false, 0x00405C23, -1, "", "MOV EAX, DWORD PTR ][") },
    { ENTRY(false, 0x00405C23, -1, "", "MOV EAX, DWORD PTR [RAX]") },

    { ENTRY(false, 0x00405C23, 10, "\xC7\x85\xE8\xFD\xFF\xFF\x00\x00\x08\x02",     "MOV DWORD PTR [EBP-0x218],0x2080000") }, //implicit SS segment
    { ENTRY(false, 0x00405C23, 11, "\xC7\x84\x24\xE8\xFD\xFF\xFF\x00\x00\x08\x02", "MOV DWORD PTR [ESP-0x218],0x2080000") }, //implicit SS segment

    { ENTRY(true, 0x7FFCA9FF1977, 10,   "\x48\xb8\x90\x78\x56\x34\x12\x00\x00\x00",     "MOV RAX, 0x1234567890") },
    { ENTRY(true, 0x7FFCA9FF1977, 10,   "\x48\xb8\x90\x78\x56\x34\x12\x00\x00\x00",     "MOVABS RAX, 0x1234567890") },
    { ENTRY(true, 0x7FFCA9FF1977, 10,   "\x48\xB8\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF",     "MOVABS RAX, 0xFFFFFFFFFFFFFFFE") },
    { ENTRY(true, 0x7FFCA9FF1977, 10,   "\x48\xa1\x90\x78\x56\x34\x12\x00\x00\x00",     "MOV RAX, QWORD PTR DS:[0x1234567890]") },
    { ENTRY(true, 0x7FFCA9FF1977, 1,    "\xCC",                                         "INT3") },
    { ENTRY(true, 0x7FFCA9FF1977, 2,    "\xCD\x03",                                     "INT 3") },
    { ENTRY(true, 0x7FFCA9FF1977, 3,    "\x48\x63\xD0",                                 "MOVSXD RDX, EAX") },
    { ENTRY(true, 0x7FFCA9FF1977, 5,    "\x45\x0F\xBE\x24\x2F",                         "MOVSX R12D, BYTE PTR [R15+RBP*1]") },
    { ENTRY(true, 0x7FFCA9FF1977, 7,    "\x4D\x69\xED\x10\x01\x00\x00",                 "IMUL R13, R13, 0x110") },
    { ENTRY(true, 0x7FFCA9FF1977, 11,   "\x48\xC7\x05\xAF\x55\x0F\x00\xFF\x00\x00\x00", "MOV QWORD PTR [RIP+0xF55AF], 0xFF") },
    { ENTRY(true, 0x7FFCA9FF1977, 11,   "\x48\xC7\x05\xAF\x55\x0F\x00\xFE\xFF\xFF\xFF", "MOV QWORD PTR [RIP+0xF55AF], 0xFFFFFFFFFFFFFFFE") },
    { ENTRY(true, 0x7FFCA9FF1977, 4,    "\x48\x0F\xC7\x08",                             "CMPXCHG16B [RAX]") },
    { ENTRY(true, 0x7FFCA9FF1977, 3,    "\x0F\xC7\x08",                                 "CMPXCHG8B [RAX]") },

    // Derived from:
    // https://raw.githubusercontent.com/aquynh/capstone/24341dcd5ab6f75333342911f2616518dc1f07b4/suite/regress.py
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\x6C", "rep insb byte ptr es:[di], dx") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\x6D", "rep insd dword ptr es:[di], dx") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\x6E", "rep outsb dx, byte ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\x6F", "rep outsd dx, dword ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xA4", "rep movsb byte ptr es:[di], byte ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xA5", "rep movsd dword ptr es:[di], dword ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xA6", "repe cmpsb byte ptr [si], byte ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xA7", "repe cmpsd dword ptr [si], dword ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAA", "rep stosb byte ptr es:[di], al") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAB", "rep stosd dword ptr es:[di], eax") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAC", "rep lodsb al, byte ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAD", "rep lodsd eax, dword ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAE", "repe scasb al, byte ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAF", "repe scasd eax, dword ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\x6C", "repne insb byte ptr es:[edi], dx") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\x6D", "repne insw word ptr es:[edi], dx") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\x6E", "repne outsb dx, byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\x6F", "repne outsw dx, word ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\xA4", "repne movsb byte ptr es:[edi], byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\xA5", "repne movsw word ptr es:[edi], word ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\xA6", "repne cmpsb byte ptr [esi], byte ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\xA7", "repne cmpsw word ptr [esi], word ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\xAA", "repne stosb byte ptr es:[edi], al") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\xAB", "repne stosw word ptr es:[edi], ax") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\xAC", "repne lodsb al, byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\xAD", "repne lodsw ax, word ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF2\xAE", "repne scasb al, byte ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF2\x66\xAF", "repne scasw ax, word ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\x6C", "rep insb byte ptr es:[di], dx") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\x6D", "rep insw word ptr es:[di], dx") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\x6E", "rep outsb dx, byte ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\x6F", "rep outsw dx, word ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xA4", "rep movsb byte ptr es:[di], byte ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\xA5", "rep movsw word ptr es:[di], word ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xA6", "repe cmpsb byte ptr [si], byte ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\xA7", "repe cmpsw word ptr [si], word ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAA", "rep stosb byte ptr es:[di], al") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\xAB", "rep stosw word ptr es:[di], ax") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAC", "rep lodsb al, byte ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\xAD", "rep lodsw ax, word ptr [si]") },
    { ENTRY(false, 0x9123ffe1, 3, "\xF3\x67\xAE", "repe scasb al, byte ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 4, "\xF3\x66\x67\xAF", "repe scasw ax, word ptr es:[di]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\x6C", "rep insb byte ptr es:[edi], dx") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\x6D", "rep insd dword ptr es:[edi], dx") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\x6E", "rep outsb dx, byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\x6F", "rep outsd dx, dword ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xA4", "rep movsb byte ptr es:[edi], byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xA5", "rep movsd dword ptr es:[edi], dword ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xA6", "repe cmpsb byte ptr [esi], byte ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xA7", "repe cmpsd dword ptr [esi], dword ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xAA", "rep stosb byte ptr es:[edi], al") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xAB", "rep stosd dword ptr es:[edi], eax") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xAC", "rep lodsb al, byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xAD", "rep lodsd eax, dword ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xAE", "repe scasb al, byte ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 2, "\xF3\xAF", "repe scasd eax, dword ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\x6C", "insb byte ptr es:[edi], dx") },
    { ENTRY(false, 0x9123ffe1, 1, "\x6D", "insd dword ptr es:[edi], dx") },
    { ENTRY(false, 0x9123ffe1, 1, "\x6E", "outsb dx, byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\x6F", "outsd dx, dword ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xA4", "movsb byte ptr es:[edi], byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xA5", "movsd dword ptr es:[edi], dword ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xA6", "cmpsb byte ptr [esi], byte ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xA7", "cmpsd dword ptr [esi], dword ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xAA", "stosb byte ptr es:[edi], al") },
    { ENTRY(false, 0x9123ffe1, 1, "\xAB", "stosd dword ptr es:[edi], eax") },
    { ENTRY(false, 0x9123ffe1, 1, "\xAC", "lodsb al, byte ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xAD", "lodsd eax, dword ptr [esi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xAE", "scasb al, byte ptr es:[edi]") },
    { ENTRY(false, 0x9123ffe1, 1, "\xAF", "scasd eax, dword ptr es:[edi]") },
    /*  { ENTRY(true, 0x649123ffe1, "\xF3\x48\x6C", "rep insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\x6D", "rep insq qword ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\x6E", "rep outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\x6F", "rep outsq dx, qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xA4", "rep movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xA5", "rep movsq qword ptr [rdi], qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xA6", "repe cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xA7", "repe cmpsq qword ptr [rsi], qword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xAA", "rep stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xA", "rep stosq qword ptr [rdi], rax") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xAC", "rep lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xAD", "rep lodsq rax, qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xAE", "repe scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x48\xAF", "repe scasq rax, qword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\x6C", "insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x48\x6D", "insq qword ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x48\x6E", "outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\x6F", "outsq dx, qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xA4", "movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xA5", "movsq qword ptr [rdi], qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xA6", "cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xA7", "cmpsq qword ptr [rsi], qword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xAA", "stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\x48\xA", "stosq qword ptr [rdi], rax") },
        { ENTRY(true, 0x649123ffe1, "\x48\xAC", "lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xAD", "lodsq rax, qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xAE", "scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x48\xAF", "scasq rax, qword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\x6C", "insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\x6D", "insq qword ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\x6E", "outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\x6F", "outsq dx, qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xA4", "movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xA5", "movsq qword ptr [rdi], qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xA6", "cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xA7", "cmpsq qword ptr [rsi], qword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xAA", "stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xA", "stosq qword ptr [rdi], rax") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xAC", "lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xAD", "lodsq rax, qword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xAE", "scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x48\xAF", "scasq rax, qword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\x6C", "insb byte ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\x6D", "insq qword ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\x6E", "outsb dx, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\x6F", "outsq dx, qword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xA4", "movsb byte ptr [edi], byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xA5", "movsq qword ptr [edi], qword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xA6", "cmpsb byte ptr [esi], byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xA7", "cmpsq qword ptr [esi], qword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xAA", "stosb byte ptr [edi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xA", "stosq qword ptr [edi], rax") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xAC", "lodsb al, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xAD", "lodsq rax, qword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xAE", "scasb al, byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x48\xAF", "scasq rax, qword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\x6C", "repne insb byte ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\x6D", "repne insq qword ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\x6E", "repne outsb dx, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\x6F", "repne outsq dx, qword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xA4", "repne movsb byte ptr [edi], byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xA5", "repne movsq qword ptr [edi], qword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xA6", "repne cmpsb byte ptr [esi], byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xA7", "repne cmpsq qword ptr [esi], qword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xAA", "repne stosb byte ptr [edi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xA", "repne stosq qword ptr [edi], rax") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xAC", "repne lodsb al, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xAD", "repne lodsq rax, qword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xAE", "repne scasb al, byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xF2\x48\xAF", "repne scasq rax, qword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\x6C", "rep insb byte ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\x6D", "rep insd dword ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\x6E", "rep outsb dx, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\x6F", "rep outsd dx, dword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xA4", "rep movsb byte ptr [edi], byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xA5", "rep movsd dword ptr [edi], dword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xA6", "repe cmpsb byte ptr [esi], byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xA7", "repe cmpsd dword ptr [esi], dword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xAA", "rep stosb byte ptr [edi], al") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xA", "rep stosd dword ptr [edi], eax") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xAC", "rep lodsb al, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xAD", "rep lodsd eax, dword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xAE", "repe scasb al, byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x67\xAF", "repe scasd eax, dword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\x6C", "repne insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\x6D", "repne insw word ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\x6E", "repne outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\x6F", "repne outsw dx, word ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xA4", "repne movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xA5", "repne movsw word ptr [rdi], word ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xA6", "repne cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xA7", "repne cmpsw word ptr [rsi], word ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xAA", "repne stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xA", "repne stosw word ptr [rdi], ax") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xAC", "repne lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xAD", "repne lodsw ax, word ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xAE", "repne scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF2\xAF", "repne scasw ax, word ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\x6C", "rep insb byte ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\x6D", "rep insw word ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\x6E", "rep outsb dx, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\x6F", "rep outsw dx, word ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xA4", "rep movsb byte ptr [edi], byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xA5", "rep movsw word ptr [edi], word ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xA6", "repe cmpsb byte ptr [esi], byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xA7", "repe cmpsw word ptr [esi], word ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xAA", "rep stosb byte ptr [edi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xA", "rep stosw word ptr [edi], ax") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xAC", "rep lodsb al, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xAD", "rep lodsw ax, word ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xAE", "repe scasb al, byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xF3\x67\xAF", "repe scasw ax, word ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x6C", "rep insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x6D", "rep insd dword ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x6E", "rep outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\x6F", "rep outsd dx, dword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xA4", "rep movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xA5", "rep movsd dword ptr [rdi], dword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xA6", "repe cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xA7", "repe cmpsd dword ptr [rsi], dword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xAA", "rep stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xA", "rep stosd dword ptr [rdi], eax") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xAC", "rep lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xAD", "rep lodsd eax, dword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xAE", "repe scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xF3\xAF", "repe scasd eax, dword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x6C", "insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x6D", "insd dword ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x6E", "outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x6F", "outsd dx, dword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xA4", "movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xA5", "movsd dword ptr [rdi], dword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xA6", "cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xA7", "cmpsd dword ptr [rsi], dword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xAA", "stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\xA", "stosd dword ptr [rdi], eax") },
        { ENTRY(true, 0x649123ffe1, "\xAC", "lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xAD", "lodsd eax, dword ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\xAE", "scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\xAF", "scasd eax, dword ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x6C", "insb byte ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x6D", "insw word ptr [rdi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x6E", "outsb dx, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x6F", "outsw dx, word ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xA4", "movsb byte ptr [rdi], byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xA5", "movsw word ptr [rdi], word ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xA6", "cmpsb byte ptr [rsi], byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xA7", "cmpsw word ptr [rsi], word ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xAA", "stosb byte ptr [rdi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\xA", "stosw word ptr [rdi], ax") },
        { ENTRY(true, 0x649123ffe1, "\x66\xAC", "lodsb al, byte ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xAD", "lodsw ax, word ptr [rsi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xAE", "scasb al, byte ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\xAF", "scasw ax, word ptr [rdi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\x6C", "insb byte ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x67\x6D", "insd dword ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x67\x6E", "outsb dx, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\x6F", "outsd dx, dword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xA4", "movsb byte ptr [edi], byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xA5", "movsd dword ptr [edi], dword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xA6", "cmpsb byte ptr [esi], byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xA7", "cmpsd dword ptr [esi], dword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xAA", "stosb byte ptr [edi], al") },
        { ENTRY(true, 0x649123ffe1, "\x67\xA", "stosd dword ptr [edi], eax") },
        { ENTRY(true, 0x649123ffe1, "\x67\xAC", "lodsb al, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xAD", "lodsd eax, dword ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xAE", "scasb al, byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x67\xAF", "scasd eax, dword ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x6C", "insb byte ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x6D", "insw word ptr [edi], dx") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x6E", "outsb dx, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\x6F", "outsw dx, word ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xA4", "movsb byte ptr [edi], byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xA5", "movsw word ptr [edi], word ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xA6", "cmpsb byte ptr [esi], byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xA7", "cmpsw word ptr [esi], word ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xAA", "stosb byte ptr [edi], al") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xA", "stosw word ptr [edi], ax") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xAC", "lodsb al, byte ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xAD", "lodsw ax, word ptr [esi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xAE", "scasb al, byte ptr [edi]") },
        { ENTRY(true, 0x649123ffe1, "\x66\x67\xAF", "scasw ax, word ptr [edi]") },*/
};