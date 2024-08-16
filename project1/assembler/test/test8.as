        lw      0       1       five    load reg1 with 5 (symbolic address)
        lw      1       8       neg1    load reg2 with -1 (numeric address) (error for register8 outside the range[0,7])
start   add     1       8       1       decrement reg1
        beq     0       1       8       goto end of program when reg1==0
        beq     0       0       start   go back to the beginning of the loop
        noop
done    halt                            end of program
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start                   will contain the address of start
