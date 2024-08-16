        lw      0       1       value1  load value1 into reg1
        lw      0       2       value2  load value2 into reg2
        lw      0       4       addr    load address of func into reg4
        jalr    4       7               jump and link to func
        halt                            end of program
func    add     1       1       1       multiply: 2*value
        nor     2       2       3       2's complement of value2 and store in reg4
        add     1       3       1       reg1 = reg1 + reg3 (= 2*value1 - value2)
        add     1       5       1       add 5 to value
        sw      0       1       result  store result value to memory
        jalr    7       4               jump back to return address
        noop                            no operation
addr    .fill   func                    address of func
value1  .fill   10                      value1
value2  .fill   3                       value2
result  .fill   0                       result of calculation in func
