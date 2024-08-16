        lw      0       1       value   load the value from memory into register 1
        add     1       1       2       add the value in register 1 to itself and store the result in register 2
        sw      0       2       result  store the result from register 2 into memory
        beq     0       0       end     branch to the end if register 0 equals 0
        sub     1       1       3       subtract the value in register 1 to itself and store in register 3 (error of unrecognized opcode "sub")
end     halt                            halt the program
value   .fill   10                      define a value in memory
result  .fill   0                       define a memory location for the result
