/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAX_LABEL_LENGTH 6
#define INT_MIN -2147483648
#define INT_MAX 2147483647

// Enum to represent instruction types
typedef enum
{
	R_TYPE,
	I_TYPE,
	J_TYPE,
	O_TYPE
} InstructionType;

typedef struct
{
	InstructionType type;
	char opcode[MAXLINELENGTH];
	int regA;
	int regB;
	int destReg;
	int offsetField;
} Instruction;

typedef struct
{
	char name[MAX_LABEL_LENGTH + 1]; // Null-terminated string
	int address;
} Label;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
void phase1(FILE *, Label[], int *);
void phase2(FILE *, FILE *, Label[], int);
int opcodeToInt(char *);
void regCheck(char *);

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3)
	{
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
			   argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL)
	{
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL)
	{
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* TODO: Phase-1 label calculation */
	Label labels[1000];
	int labelCount = 0;
	phase1(inFilePtr, labels, &labelCount);

	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */
	phase2(inFilePtr, outFilePtr, labels, labelCount);

	if (inFilePtr)
	{
		fclose(inFilePtr);
	}
	if (outFilePtr)
	{
		fclose(outFilePtr);
	}
	exit(0); //return (0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
				 char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
	{
		/* reached end of file */
		return (0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL)
	{
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label))
	{
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
				"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
		   opcode, arg0, arg1, arg2);
	return (1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return ((sscanf(string, "%d", &i)) == 1);
}

void phase1(FILE *inFilePtr, Label labels[], int *labelCount)
{
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	int address = 0;

	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		if (strcmp(label, ""))
		{
			//Check if valid label
			if(strlen(label) > 6){
				printf("Error: Invalid label\n");
				exit(1);
			}

			// Check if label is already defined
			for (int i = 0; i < *labelCount; i++)
			{
				if (strcmp(labels[i].name, label) == 0)
				{
					printf("Error: Duplicated label definition for '%s'\n", label);
					exit(1);
				}
			}

			strcpy(labels[*labelCount].name, label);
			labels[*labelCount].address = address;
			(*labelCount)++;
		}

		address++;
	}
}

void phase2(FILE *inFilePtr, FILE *outFilePtr, Label labels[], int labelCount)
{
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	Instruction instructions[MAXLINELENGTH];
	int instructionCount = 0;
	int currentAddress = 0;

	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
	{
		if (strcmp(opcode, ".fill") == 0)
		{
			// Handle .fill directive
			int value;
			if (isNumber(arg0))
			{
				// If arg0 is a number, use it as the value directly
				value = atoi(arg0);
			}
			else
			{
				// If arg0 is a label, find its address
				int found = 0;
				for (int i = 0; i < labelCount; i++)
				{
					if (strcmp(labels[i].name, arg0) == 0)
					{
						value = labels[i].address;
						found = 1;
						break;
					}
				}
				if (!found) //if not found the address, exception handling for undefined label
				{
					printf("Error: Undefined label '%s'\n", arg0);
					exit(1);
				}
			}

			// Output the value as machine code
			fprintf(outFilePtr, "%d\n", value);
			printf("(address %d): %d (hex 0x%X)\n", currentAddress, value, value);
		}
		else
		{
			InstructionType type;
			// Determine the type
			if (strcmp(opcode, "add") == 0 || strcmp(opcode, "nor") == 0)
			{
				type = R_TYPE;
			}
			else if (strcmp(opcode, "lw") == 0 || strcmp(opcode, "sw") == 0 ||
					 strcmp(opcode, "beq") == 0)
			{
				type = I_TYPE;
			}
			else if (strcmp(opcode, "jalr") == 0)
			{
				type = J_TYPE;
			}
			else if (strcmp(opcode, "noop") == 0 || strcmp(opcode, "halt") == 0)
			{
				type = O_TYPE;
			}
			else //otherwise, exception handling
			{
				printf("Error: Unrecognized opcode '%s'\n", opcode);
				exit(1);
			}

			// Process fields based on instruction type
			Instruction instr;
			instr.type = type;
			strcpy(instr.opcode, opcode);

			if (type == R_TYPE)
			{
				// Check register arguments
				if (!isNumber(arg0) || !isNumber(arg1) || !isNumber(arg2))
				{
					printf("Error: Non-integer register argument for R-type instruction\n");
					exit(1);
				}
				regCheck(arg0);
				regCheck(arg1);
				regCheck(arg2);

				instr.regA = atoi(arg0);
				instr.regB = atoi(arg1);
				instr.destReg = atoi(arg2);
			}
			else if (type == I_TYPE)
			{
				// Check register arguments
				if (!isNumber(arg0) || !isNumber(arg1))
				{
					printf("Error: Non-integer register argument for R-type instruction\n");
					exit(1);
				}
				regCheck(arg0);
				regCheck(arg1);

				instr.regA = atoi(arg0);
				instr.regB = atoi(arg1);

				if (isNumber(arg2))
				{
					// If arg2 is a number, use it as the offsetField directly
					instr.offsetField = atoi(arg2);
				}
				else
				{
					// If arg2 is a label, compute the offsetField
					int found = 0;
					for (int i = 0; i < labelCount; i++)
					{
						if (strcmp(labels[i].name, arg2) == 0)
						{
							int offset = 0;
							if(strcmp(opcode, "beq") == 0){ // Handle "beq"
								offset = labels[i].address - currentAddress - 1;
							}
							else{
								offset = labels[i].address;
							}
							if (offset < 0){
            					offset = offset % (1<<16);
							}
							if (offset > 32767 || offset < -32768) //exception handling for range of offsetField
							{
								printf("Error: OffsetField '%s' does not fit in 16 bits\n", arg2);
								exit(1);
							}
							instr.offsetField = offset;
							found = 1;
							break;
						}
					}
					if (!found) //if not found the address, exception handling for undefined label
					{
						printf("Error: Undefined label '%s'\n", arg2);
						exit(1);
					}
				}
			}
			else if (type == J_TYPE)
			{
				// Check register arguments
				if (!isNumber(arg0) || !isNumber(arg1))
				{
					printf("Error: Non-integer register argument for J-type instruction\n");
					exit(1);
				}
				regCheck(arg0);
				regCheck(arg1);

				instr.regA = atoi(arg0);
				instr.regB = atoi(arg1);
			}

			instructions[instructionCount++] = instr;
			int machineCode = 0;

			// Generate machine code based on instruction type
			if (instr.type == R_TYPE)
			{
				machineCode = 0;
				machineCode |= (opcodeToInt(instr.opcode) << 22);
				machineCode |= (instr.regA << 19);
				machineCode |= (instr.regB << 16);
				machineCode |= (instr.destReg << 0);
			}
			else if (instr.type == I_TYPE)
			{
				machineCode = 0;
				machineCode |= (opcodeToInt(instr.opcode) << 22);
				machineCode |= (instr.regA << 19);
				machineCode |= (instr.regB << 16);
				machineCode |= (instr.offsetField & 0xFFFF); // Masking to keep only lower 16 bits
			}
			else if (instr.type == J_TYPE)
			{
				machineCode = 0;
				machineCode |= (opcodeToInt(instr.opcode) << 22);
				machineCode |= (instr.regA << 19);
				machineCode |= (instr.regB << 16);
			}
			else if (instr.type == O_TYPE)
			{
				machineCode = 0;
				machineCode |= (opcodeToInt(instr.opcode) << 22);
			}

			// Print the machine code in decimal and hexadecimal
		    fprintf(outFilePtr, "%d\n", machineCode);
			printf("(address %d): %d (hex 0x%06X)\n", currentAddress, machineCode, machineCode);
		}
		
		currentAddress++; // Increment current address for the next instruction
	}
}

// Function to convert opcode string to corresponding integer value
int opcodeToInt(char *opcode)
{
	if (strcmp(opcode, "add") == 0)
		return 0;
	if (strcmp(opcode, "nor") == 0)
		return 1;
	if (strcmp(opcode, "lw") == 0)
		return 2;
	if (strcmp(opcode, "sw") == 0)
		return 3;
	if (strcmp(opcode, "beq") == 0)
		return 4;
	if (strcmp(opcode, "jalr") == 0)
		return 5;
	if (strcmp(opcode, "halt") == 0)
		return 6;
	if (strcmp(opcode, "noop") == 0)
		return 7;
	printf("Error: Unrecognized opcodes\n"); // Invalid opcode
	exit(1); //return -1;
}

void regCheck(char *reg){ // Exception handling for register out of range
	if(atoi(reg) < 0 || atoi(reg) > 7){
		printf("Error: Registers outside the range [0, 7]\n");
		exit(1);
	}
}