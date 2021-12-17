echo    START   0

        JSUB    stackinit

        LDCH    TESTCHR
        JSUB    char

        JSUB    nl

        LDA     #STRING
        JSUB    string

        JSUB    nl

        LDA     COOLN
        JSUB    num

        JSUB    nl

HALT    J       HALT


.____________OUTPUT SEGMENT______________________

char    WD      OUTPUT
        RSUB



nl      STL     @stackptr
        JSUB    stackpush
        STA     @stackptr
        JSUB    stackpush

        LDCH    NEWL
        WD      OUTPUT

        JSUB    stackpop
        LDA     @stackptr
        JSUB    stackpop
        LDL     @stackptr
        RSUB


string  STL     @stackptr
        JSUB    stackpush

        STA     SADDR

LOOP    CLEAR   A
        LDCH    @SADDR
        COMP    #0
        JEQ     STREND

        JSUB    char
        LDA     #1
        ADD     SADDR
        STA     SADDR

        J       LOOP
  
STREND  JSUB    stackpop
        LDL     @stackptr

        RSUB

SADDR   RESW    1

num     STL     @stackptr
        JSUB    stackpush

        STS     @stackptr
        JSUB    stackpush
        STT     @stackptr
        JSUB    stackpush
        STB     @stackptr
        JSUB    stackpush
        STX     @stackptr
        JSUB    stackpush
        STA     @stackptr
        JSUB    stackpush

        LDX     #7      . start saving at the end
        LDT     #10     . mod 10
        RMO     A, S    . start by making A = S = num

        .modulo 
NLOOP   RMO     S, A    . S and A contain num
        DIVR    T, A    . get whole part in A
        RMO     A, B    . save whole part in B for next loop
        MULR    T, A    . mul 10 with A
        COMP    #0      . if whole part is 0 end on next jeq

        SUBR    A, S    . remove from whole num in S to get remainder
        LDA     #48     . ascii for 0
        ADDR    S, A    . add to S to get ascii for that num
        STCH    ONUM,X  . store remainder as ascii

        JEQ     NEND

        RMO     B, S    . move whole part to S for next loop
        LDA     #1
        SUBR    A, X    . move X back 1, for next digit  

        J       NLOOP

NEND    LDA     #ONUM
        ADDR    X, A
        JSUB    string

        JSUB    stackpop
        LDA     @stackptr
        JSUB    stackpop
        LDX     @stackptr
        JSUB    stackpop
        LDB     @stackptr
        JSUB    stackpop
        LDT     @stackptr
        JSUB    stackpop
        LDS     @stackptr
        JSUB    stackpop
        LDL     @stackptr

        RSUB

ONUM    RESB    8
        BYTE    0       . null terminator

NEWL    BYTE    10

.____________END OF OUTPUT SEGMENT______________________

OUTPUT  BYTE    1

TESTCHR BYTE    C'x'

STRING  BYTE    C'jodl lojz, ta 0 bi mogla bit ignorirana'
        BYTE    0

COOLN   WORD    42069


.____________STACK SEGMENT__________________

stackinit   STA     SAVEA_S
            LDA     #SBASE
            SUB     #3
            STA     stackptr
            LDA     SAVEA_S
            RSUB

stackpush   STA     SAVEA_S
            LDA     stackptr
            SUB     #3
            STA     stackptr  
            LDA     SAVEA_S   
            RSUB

stackpop    STA     SAVEA_S
            LDA     stackptr
            ADD     #3
            STA     stackptr
            LDA     SAVEA_S
            RSUB

SAVEA_S     RESW    1

stackptr    RESW    1

ALLOC       RESW    256
SBASE       EQU     *

.____________STACK SEGMENT__________________