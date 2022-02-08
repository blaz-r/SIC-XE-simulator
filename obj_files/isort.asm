. insertion sort algorithm
. reads list of numbers from INPUT (BB.dev) file and writes sorted list to OUTPUT (CC.dev) file
. it wont work without INPUT file present! (BB.dev, included with example)

isort       START   0

            JSUB    stackinit

            JSUB    readarray

            JSUB    sort

            JSUB    writearray

HALT        J       HALT


INPUT       BYTE    X'BB'
OUTPUT      BYTE    X'CC'


. ____________SORT SEGMENT__________________

. insertion sort algorithm
sort        STL     @stackptr
            JSUB    stackpush
            STA     @stackptr
            JSUB    stackpush
            STX     @stackptr
            JSUB    stackpush
            STS     @stackptr
            JSUB    stackpush
            STT     @stackptr
            JSUB    stackpush
            STB     @stackptr
            JSUB    stackpush
        
            . S = i, T = j

            LDS     #3

OUTLOOP     RMO     S, A        . outer loop i < arrlen
            COMP    ARRLEN
            JEQ     SORTEND
            RMO     S, T        . j = i

INLOOP      RMO     T, A        . inner loop
            COMP    #0          . while j > 0
            JEQ     ENDIN

            . while arr[j - i] > arr[j]
            RMO     A, X
            LDB     BUFFER, X   . B = Arr[j]
            SUB     #3
            RMO     A, X        
            LDA     BUFFER, X   . A = Arr[j - 1]

            COMPR   A, B        . end if arr[j - i] > arr[j] no longer holds
            JLT     ENDIN
            JEQ     ENDIN
                                . swap arr[j - 1] and  arr[j]
            STB     BUFFER, X   . Arr[j - 1] = B
            LDB     #3
            ADDR    B, X
            STA     BUFFER, X   . Arr[j] = A

            SUBR    B, T        . j = j - 1

            J       INLOOP

ENDIN       LDA     #3          . i = i + 1
            ADDR    A, S

            J       OUTLOOP


SORTEND     JSUB    stackpop
            LDB     @stackptr
            JSUB    stackpop
            LDT     @stackptr
            JSUB    stackpop
            LDS     @stackptr
            JSUB    stackpop
            LDX     @stackptr
            JSUB    stackpop
            LDA     @stackptr
            JSUB    stackpop
            LDL     @stackptr

            RSUB

.____________END OF SORT SEGMENT____________


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

.____________END OF STACK SEGMENT__________________


.____________OUTPUT SEGMENT______________________

char        WD      OUTPUT
            RSUB


nl          STL     @stackptr
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


string      STL     @stackptr
            JSUB    stackpush

            STA     SADDR

LOOP        CLEAR   A
            LDCH    @SADDR
            COMP    #0
            JEQ     STREND

            JSUB    char
            LDA     #1
            ADD     SADDR
            STA     SADDR

            J       LOOP
  
STREND      JSUB    stackpop
            LDL     @stackptr

            RSUB

SADDR       RESW    1


num         STL     @stackptr
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
NLOOP       RMO     S, A    . S and A contain num
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

NEND        LDA     #ONUM
            ADDR    X, A    . X contains offset
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


. write array to file
writearray  STL     @stackptr
            JSUB    stackpush
            STA     @stackptr
            JSUB    stackpush
            STX     @stackptr
            JSUB    stackpush

            LDX     #0

WRITEAL     RMO     X, A        
            COMP    ARRLEN      . if we reached the end of array stop
            JEQ     WRETEE

            LDA     BUFFER, X
            JSUB    num         . write number to file then new line
            JSUB    nl

            LDA     #3
            ADDR    A, X

            J       WRITEAL

WRETEE      JSUB    stackpop
            LDX     @stackptr
            JSUB    stackpop
            LDA     @stackptr
            JSUB    stackpop
            LDL     @stackptr

            RSUB


ONUM        RESB    8
            BYTE    0       . null terminator

NEWL        BYTE    10

.____________END OF OUTPUT SEGMENT______________________


.____________INPUT SEGMENT______________________________

readnum     STL     @stackptr
            JSUB    stackpush
            STB     @stackptr
            JSUB    stackpush
            STX     @stackptr
            JSUB    stackpush

            CLEAR   B
            LDX     #10

RNL	        TD	    INPUT
            JEQ	    RNL

            CLEAR   A
            RD      INPUT
            COMP    #10     . newline
            JEQ     RNE

            COMP    #0
            JEQ     RNE

            SUB     ZERO
            MULR    X, B
            ADDR    A, B

            J       RNL

RNE         RMO     B, A
            JSUB    stackpop
            LDX     @stackptr
            JSUB    stackpop
            LDB     @stackptr
            JSUB    stackpop
            LDL     @stackptr

            RSUB


. read array of numbers from a file
readarray   STL     @stackptr
            JSUB    stackpush
            STA     @stackptr
            JSUB    stackpush
            STX     @stackptr
            JSUB    stackpush

            LDX     #0

READAL      JSUB    readnum     . read number from file
            COMP    #0
            JEQ     READE       . end when we reach 0
            STA     BUFFER, X
            LDA     #3
            ADDR    A, X        . increase index of buffer

            J       READAL

READE       STX     ARRLEN     
            JSUB    stackpop
            LDX     @stackptr
            JSUB    stackpop
            LDA     @stackptr
            JSUB    stackpop
            LDL     @stackptr

            RSUB


ZERO        BYTE    0
            BYTE    0
            BYTE    C'0'     

BUFFER      RESW    256         . buffer for numbers read from file
ARRLEN      RESW    1

.____________END OF INPUT SEGMENT_______________________
