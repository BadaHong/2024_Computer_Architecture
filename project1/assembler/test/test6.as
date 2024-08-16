        lw      0       1       label1  load value from memory into reg1 (error for duplicated definition of label)
        add     1       2       3       add reg1 and reg2 and store in reg3
label1  .fill   10                     first definition of label
label1  .fill   20                     second definition of label with a different address
        halt                            end of program
