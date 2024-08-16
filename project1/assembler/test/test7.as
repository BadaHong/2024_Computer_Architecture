        lw      0       1       value   load the value from memory into register 1
        add     1       1       abc     add the value in register 1 to itself and store in register abc (error of non-integer register arg)
        sw      0       abc     res1    store the result from register abc into memory
        beq     0       0       end     branch to the end if register 0 equals 0
        add     1       1       3       add the value in register 1 to itself and store the result in register 3
        nor     4       4       5       perform a NOR operation on the value in register 4 and store the result in register 5
        sw      0       5       res2    store the result from register 5 into memory as result2
end     halt                            halt the program
value   .fill   10                      define a value in memory
res1    .fill   0                       define a memory location for the result
res2    .fill   0                       define another memory location for the second result
