print   START   0
        
        LDX     #0

LOOP    LDCH    TEXT,X
        WD      OUTPUT
        TIX     #LEN
        JLT     LOOP
        


HALT    J       HALT


OUTPUT  BYTE    X'11'

TEXT    BYTE    C'SIC/XE'
        BYTE    10
LAST    EQU     *
LEN     EQU     LAST - TEXT