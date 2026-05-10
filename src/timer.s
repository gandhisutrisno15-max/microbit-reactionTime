; ARM v7 Assembly Timer Function for micro:bit
; Measures reaction time using DWT_CYCCNT (cycle counter)
; 
; Function: uint32_t measure_reaction_time(void)
; Returns: Number of CPU cycles elapsed

    AREA    |.text|, CODE, READONLY
    THUMB
    EXPORT  measure_reaction_time

; DWT (Data Watchpoint and Trace) register addresses
DWT_CTRL        EQU 0xE0001000
DWT_CYCCNT      EQU 0xE0001004
DEMCR           EQU 0xE000EDFC

; Button register addresses (micro:bit GPIO)
GPIOTE_BASE     EQU 0x40006000
GPIO_BASE       EQU 0x50000000

measure_reaction_time
    PUSH    {R4, R5, LR}
    
    ; Initialize cycle counter (DWT_CYCCNT)
    LDR     R0, =DEMCR
    LDR     R1, [R0]
    ORR     R1, R1, #0x01000000        ; Set TRCENA bit
    STR     R1, [R0]
    
    ; Reset cycle counter
    LDR     R0, =DWT_CYCCNT
    MOV     R1, #0
    STR     R1, [R0]
    
    ; Start counting
    MOV     R4, #0                     ; R4 = start_cycles
    LDR     R5, =DWT_CYCCNT
    LDR     R4, [R5]                   ; Read initial cycle count
    
    ; Wait for button press (polling method)
    ; Check GPIO for button A press on P0.04
wait_for_button
    LDR     R0, =GPIO_BASE
    LDR     R1, [R0, #0x510]           ; Read PIN register
    TST     R1, #0x10                  ; Test bit 4 (P0.04)
    BNE     wait_for_button            ; Loop if button not pressed
    
    ; Button pressed - stop counting
    LDR     R5, =DWT_CYCCNT
    LDR     R0, [R5]                   ; Read final cycle count
    
    ; Calculate elapsed cycles: R0 = end_cycles - start_cycles
    SUB     R0, R0, R4
    
    POP     {R4, R5, PC}
    
    END