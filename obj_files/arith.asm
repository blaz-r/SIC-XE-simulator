arith   START   0
        LDA     X
        ADD     Y
        STA     SUM

        LDA     X
        SUB     Y
        STA     DIFF

        LDA     X
        MUL     Y
        STA     PROD

        LDA     X
        DIV     Y
        STA     QUOT

        LDA     X   .load X
        DIV     Y   .dvide by Y to get whole part
        MUL     Y   .multiply whole part with Y
        STA     MOD .save that to mod

        LDA     X   .load X to A
        SUB     MOD .substract whole*Y from MOD to get remainder
        STA     MOD .save remaineder to MOD        


HALT    J       HALT

X       WORD    45
Y       WORD    7

SUM     RESW    1
DIFF    RESW    1
PROD    RESW    1
QUOT    RESW    1
MOD     RESW    1
