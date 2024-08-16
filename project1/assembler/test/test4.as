        lw      0       1       addr1   load address of value1 into reg1
        lw      0       2       addr2   load address of value2 into reg2
        lw      0       3       temp    load address of temp variable into reg3
        lw      0       4       0       load 0 into reg4 (used as a temporary register)
        lw      0       5       0       load 0 into reg5 (used as a temporary register)
        lw      0       6       0       load 0 into reg6 (used as a temporary register)
        lw      1       6       0       load value1 from memory into temporary reg6
        lw      2       4       0       load value2 into reg4
        sw      1       4       0       store value2 into address of value1 (swap the values)
        sw      2       6       0       store value1 into address of value2 (swap the values)
        halt                            end of program
addr1   .fill   value1                  address of value1 in memory
addr2   .fill   value2                  address of value2 in memory
value1  .fill   10                      initial value of value1
value2  .fill   20                      initial value of value2
temp    .fill   0                       placeholder for temporary variable to swap
