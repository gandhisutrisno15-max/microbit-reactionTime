.syntax unified
.cpu cortex-m4
.thumb

.global measure_reaction_time

.equ DEMCR,      0xE000EDFC
.equ DWT_CTRL,   0xE0001000
.equ DWT_CYCCNT, 0xE0001004

measure_reaction_time:

    PUSH {r4, lr}

    /* Enable DWT */
    LDR r0, =DEMCR
    LDR r1, [r0]
    ORR r1, r1, #0x01000000
    STR r1, [r0]

    /* Reset cycle counter */
    LDR r0, =DWT_CYCCNT
    MOV r1, #0
    STR r1, [r0]

    /* Enable cycle counter */
    LDR r0, =DWT_CTRL
    LDR r1, [r0]
    ORR r1, r1, #1
    STR r1, [r0]

    /* Read start cycles */
    LDR r0, =DWT_CYCCNT
    LDR r4, [r0]

wait_for_button:

    /* Check Button A */
    LDR r0, =0x50000510
    LDR r1, [r0]

    /* Test bit 14 */
    TST r1, #(1 << 14)

    BNE wait_for_button

    /* Read end cycles */
    LDR r0, =DWT_CYCCNT
    LDR r1, [r0]

    /* elapsed = end - start */
    SUB r0, r1, r4

    POP {r4, pc}
