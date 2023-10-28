# AArch64-Assembler
Main Purpose: Takes in assembly source code and converts it into binary executable code.
What I coded: 
1. assemble.c and .h files - Contains the main body of the algorithm and split across 4 phases
2. structure.c and .h files - Contains all the key structs and related functions for them

All the other modules are for dealing with specific types of instructions

Overview: Our task was to code a c program that takes in AArch 64 instruction set assembly code and convert each line of instruction into 32bit machine code. Each instruction had its unique number along with how its binary code was to be calculated. The assembly code also contained branches, which are like functions. When branches are called in the code, the machine code instruction must replace its name with its memory address. 
The memory was 4 bytes aligned, hence 32 bits. That meant each line of instruction or data took exactly 4 bytes, so that is why we increment the memory address counter by 4 each time we process each line of the assembly file.

Our implementation: We wanted to encapsulate all the key information of the instruction within a structure. We decided to use Nodes and a linked list. The node contains key information such as name of the operation, its memory address and its arguments. Since all the instructions were to be translated in order, we went with a linked list. Since there are branches, we kept track of the each of the names and their memory address. We used Symbol entry for encapsulating each branch's name and memory address and a table for keeping track of all of them. Lastly to avoid lots of if and else statements, we used function pointer entry. Each entry contains the pointer to the function and also the name through which that function is called. This meant we can just pass the name of the operation of an instruction and the corresponding function which deals with that can be easily fetched. 
We broke our algorithm into 4 phases:

Phase One: Initialization and Parsing
In the first phase, the program initializes essential data structures and begins parsing the input assembly file.
Initialization of Function Pointers and Symbol Table:
The program initializes a function pointer table funcPtrTable containing pointers to various processing functions for different types of instructions.
A symbol table symTable is initialized to store labels and their corresponding memory addresses.
Parsing the Assembly File:
The program reads the input assembly file line by line.
Each line is tokenized to extract labels, instruction types, and arguments.
Labels are stored in the symTable, while instructions and arguments are processed further.



Phase Two: Node Creation and Symbol Table Filling
In the second phase, the parsed data is used to create nodes, forming a linked list representation of the assembly code. Additionally, the symbol table is populated with labels and their corresponding addresses.
Node Creation:
Nodes are created to represent each assembly instruction.
The nodes contain information such as the memory address, instruction type, and associated arguments.
Symbol Table Filling:
Labels encountered during parsing are added to the symTable with their respective memory addresses.



Phase Three: Instruction Processing and Binary Conversion
In the third phase, the program processes each node, converts assembly instructions into binary format, and writes the binary output to a file.
Node Processing:
Each node is processed using the appropriate function pointer obtained from the funcPtrTable.
The corresponding function processes the instruction and returns its binary representation.
Binary Conversion and Output:
Processed instructions are converted into 32-bit binary words.
The binary words are written to the output binary file, creating the final binary representation of the input assembly code.



Phase Four: Clean-up and Memory Management
In the final phase, the program performs clean-up tasks, deallocates memory, and ensures proper closure of files.
Memory Deallocation:
All allocated memory for nodes, symbol tables, and strings is deallocated to prevent memory leaks.
File Closure:
The input and output files are closed to ensure that all data is written and resources are released properly.
All allocated memory for nodes, symbol tables, and strings is deallocated to prevent memory leaks.
File Closure:
The input and output files are closed to ensure that all data is written and resources are released properly.
