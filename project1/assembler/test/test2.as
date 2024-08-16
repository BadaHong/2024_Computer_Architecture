        lw      0       1       count   load counter address into reg1
        add     0       0       3       clear reg3 for sum
loop    add     3       1       3       add counter value to sum
        add     1       1       1       increment counter
        beq     1       5       end     check if counter equals 5
        beq     0       0       loop    repeat loop if not reached 5
end     sw      0       3       result  store result in memory
        halt                            end of program
count   .fill   1                       counter starting value
result  .fill   0                       placeholder for result of sum of 1~5
