#include "asm.h"

#define PROC1_STACK_SIZE 0x50

void calcPinProc1ASM(const signed int timeIn, // [IN] a signed int of the 32-bit timestamp (rbp-0x30)
                     unsigned char* out       // [OUT] a ptr to a C array of length 6 (rbp-0x46)
) {
    // use a plain C array as a fake stack
    unsigned char buf[PROC1_STACK_SIZE] = {0};

    __asm__ __volatile__(
/* Block A */
    // inject timeIn here
    "movl   %[timeIn], -0x30(%[buf]);"

    // everything from here on is stolen from the EXin OsX executable
    "movl   -0x30(%[buf]), %%esi;"  // 0x100011b5c <+177>
    "bswapl %%esi;"                 // 0x100011b5f <+180>
    "xorl   %%ecx, %%ecx;"          // 0x100011b61 <+182>
    "movl   %%esi, -0x34(%[buf]);"  // 0x100011b63 <+184>
    "movl   $0x3, %%edx;"           // 0x100011b66 <+187>
    "movl   $0x1, %%eax;"           // 0x100011b6b <+192>
    "movl   %%esi, -0x40(%[buf]);"  // 0x100011b70 <+197>
    "movl   $0x0, -0x38(%[buf]);"   // 0x100011b73 <+200>

/* Block B
 * Set [rbp-0x38, rbp-0x35], call it `m38`.
 * Temporary values: [rbp-0x40, rbp-0x3d], call it `m40`.
 * */
"B_loc100011b7a:"
    "movslq %%ecx, %%rsi;"      // 0x100011b7a <+207>
    "movb   -0x38(%[buf], %%rsi), %%bl;"     // 0x100011b7d <+210>
    "addb   %%bl, %%bl;"        // 0x100011b81 <+214>
    "movslq %%edx, %%r8;"       // 0x100011b83 <+216>
    "movb   -0x40(%[buf], %%r8), %%cl;"      // 0x100011b86 <+219>
    "movb   %%cl, %%dl;"        // 0x100011b8b <+224>
    "andb   $0x1, %%dl;"        // 0x100011b8d <+226>
    "orb    %%bl, %%dl;"        // 0x100011b90 <+229>
    "movb   %%dl, -0x38(%[buf], %%rsi);"     // 0x100011b92 <+231>
    "movl   %%eax, %%esi;"      // 0x100011b96 <+235>
    "sarl   $0x1f, %%esi;"      // 0x100011b98 <+237>
    "movl   %%esi, %%edi;"      // 0x100011b9b <+240>
    "shrl   $0x1e, %%edi;"      // 0x100011b9d <+242>
    "addl   %%eax, %%edi;"      // 0x100011ba0 <+245>
    "andl   $-0x4, %%edi;"      // 0x100011ba2 <+247>
    "shrb   %%cl;"              // 0x100011ba5 <+250>
    "movb   %%cl, -0x40(%[buf], %%r8);"      // 0x100011ba7 <+252>
    "movl   %%eax, %%ecx;"      // 0x100011bac <+257>
    "subl   %%edi, %%ecx;"      // 0x100011bae <+259>
    "shrl   $0x1d, %%esi;"      // 0x100011bb0 <+261>
    "addl   %%eax, %%esi;"      // 0x100011bb3 <+264>
    "sarl   $0x3, %%esi;"       // 0x100011bb5 <+266>
    "movl   $0x3, %%edx;"       // 0x100011bb8 <+269>
    "subl   %%esi, %%edx;"      // 0x100011bbd <+274>
    "incl   %%eax;"             // 0x100011bbf <+276>
    "cmpl   $0x21, %%eax;"      // 0x100011bc1 <+278>
    "jne    B_loc100011b7a;"      // <+207>   0x100011bc4 <+281>

/* Block C */
    "movl   -0x38(%[buf]), %%ecx;"      // 0x100011bc6 <+283>
    "movb   %%cl, %%al;"        // 0x100011bc9 <+286>
    "shrb   $0x2, %%al;"        // 0x100011bcb <+288>
    "movzwl -0x38(%[buf]), %%edx;"      // 0x100011bce <+291>
    "movb   %%al, -0x46(%[buf]);"       // 0x100011bd2 <+295>

    "movl   %%ecx, %%esi;"      // 0x100011bd5 <+298>
    "andl   $0xf000, %%esi;"    // 0x100011bd7 <+300>
    "shrl   $0xc, %%esi;"       // 0x100011bdd <+306>
    "movb   %%cl, %%bl;"        // 0x100011be0 <+309>
    "shlb   $0x4, %%bl;"        // 0x100011be2 <+311>
    "andb   $0x30, %%bl;"       // 0x100011be5 <+314>
    "orb    %%sil, %%bl;"       // 0x100011be8 <+317>
    "movb   %%bl, -0x45(%[buf]);"    // 0x100011beb <+320>

    "movl   %%ecx, %%esi;"      // 0x100011bee <+323>
    "shrl   $0x16, %%esi;"      // 0x100011bf0 <+325>
    "andb   $0x3, %%sil;"       // 0x100011bf3 <+328>
    "shrl   $0x6, %%edx;"       // 0x100011bf7 <+332>
    "andb   $0x3c, %%dl;"       // 0x100011bfa <+335>
    "orb    %%sil, %%dl;"       // 0x100011bfd <+338>
    "movb   %%dl, -0x44(%[buf]);"    // 0x100011c00 <+341>

    "movl   %%ecx, %%edx;"      // 0x100011c03 <+344>
    "shrl   $0x10, %%edx;"      // 0x100011c05 <+346>
    "andb   $0x3f, %%dl;"       // 0x100011c08 <+349>
    "movb   %%dl, -0x43(%[buf]);"    // 0x100011c0b <+352>

    "movl   %%ecx, %%esi;"      // 0x100011c0e <+355>
    "shrl   $0x14, %%esi;"      // 0x100011c10 <+357>
    "andb   $0x30, %%sil;"      // 0x100011c13 <+360>
    "shrl   $0x1a, %%ecx;"      // 0x100011c17 <+364>
    "movl   $0x1, %%edx;"       // 0x100011c1a <+367>
    "movb   %%cl, -0x42(%[buf]);"   // 0x100011c1f <+372>

    "movb   %%sil, -0x41(%[buf]);"  // 0x100011c22 <+375>
    "jmp    E_loc100011c2f;"          // <+388>   0x100011c26 <+379>

/* Block D */
"D_loc100011c28:"
    "movb   -0x46(%[buf], %%rdx), %%al;"     // 0x100011c28 <+381>
    "incq   %%rdx;"             // 0x100011c2c <+385>

/* Block E */
"E_loc100011c2f:"
    "movb   %%al, %%cl;"        // 0x100011c2f <+388>
    "addb   $0x20, %%cl;"       // 0x100011c31 <+390>
    "cmpb   $0x3f, %%cl;"       // 0x100011c34 <+393>
    "ja     G_loc100011c3d;"      // <+402>   0x100011c37 <+396>

/* Block F */
    "movb   %%cl, %%al;"        // 0x100011c39 <+398>
    "jmp    H_loc100011c3f;"      // <+404>   0x100011c3b <+400>

/* Block G */
"G_loc100011c3d:"
    "addb   $0x21, %%al;"       // 0x100011c3d <+402>

/* Block H */
"H_loc100011c3f:"
    "movb   %%al, -0x47(%[buf], %%rdx);"    // 0x100011c3f <+404>
    "cmpl   $0x6, %%edx;"       // 0x100011c43 <+408>
    "jne    D_loc100011c28;"      // <+381>   0x100011c46 <+411>
    :/* No outputs */
    :[timeIn]"r"(timeIn),[buf]"r"(buf + PROC1_STACK_SIZE)
    :"%rax","%rbx","%rcx","%rdx","%rsi","%rdi","%r8","cc","memory"
    );
    #if DEBUGGING
        printf("[DBG] Proc1 full stack:\n");
        fprintHexByteArray(stdout, (const unsigned char*) buf, PROC1_STACK_SIZE);
    #endif

    memcpy(out, buf + PROC1_STACK_SIZE - 0x46, PROC1_OUT_LENGTH * sizeof(char));
}