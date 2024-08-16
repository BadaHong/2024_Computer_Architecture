        lw      0       1       neg1    load reg1 with a negative value
        lw      0       2       neg2    load reg2 with another negative value
        lw      0       3       pos     load reg3 with a positive value
        nor     1       0       4       compute the 2's complement of negative1
        add     4       2       5       subtract negative2 from negative1
        add     5       3       6       add the result of subtraction to positive
        sw      0       6       result  store the final result
        noop                            no operation
        halt                            end of program
neg1   .fill   -10                      example negative value
neg2   .fill   -20                      another negative value
pos    .fill   30                       positive value
result .fill   0                        placeholder for the result of (neg1 - neg2 + pos)
