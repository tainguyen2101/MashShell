# UW Tacoma Operating System Spring 2020

## Mash Shell

### Requirement
This program must be run on a Linux machine
You must create a txt test file to test these command

### Objective 
The purpose of this assignment is to use the fork, wait, and exec commands to write a simple Linux shell. This shell, called “mash” will **MASH** three Linux command requests together and run them against the same input file in parallel (i.e. at the same time). The user will provide three distinct Linux commands with arguments, and a single filename to operate on. The mash shell will **mash** the requests together executing each command separately against the file.
For this program, implement your mash shell using the fork, exec, and wait commands. The following limitations will apply:
1. User commands plus arguments will not exceed 255 characters
2. The filename will not exceed 255 characters. The file will either be in the local directory, or the user will provide a fully qualified path name which is 255 characters or less. The mash shell is not responsible for finding the input file.
3. All commands provided to “mash” will run using the user’s original path. Type “echo $PATH” to see the current path variable setting.
The mash shell is not responsible for finding the command file to run.
4. For each command, the maximum number of arguments including the command itself will not exceed 5. This equates to 4 command arguments, plus the command, for a total of 5 inputs.
** Extra Credit-1: SUPPORT UNLIMITED ARGUMENTS **
5. On the event that a user makes a mistake typing a command or its arguments, mash will simply fail to run the command. A simple error should be shown, but only if the exec fails
6. Mash does not accept command line arguments. Running mash requests 3 commands (plus arguments) and a file name interactively from the user.
7. To execute the “mash” of commands as fast as possible, mash should execute commands in parallel using separate processes. Consequently, the commands may complete out of order.
** Extra credit-2: DISPLAY OUTPUT SORTED BY PROCESS COMPLETION: capture the output from each command to a temporary file, and display the output for each command in the same order as requested by the user. Use file redirection as shown in the example in class. Capture the output of each command’s STDOUT file stream to a separate temporary file on disk. Read the temporary files to then regenerate the command output in the correct order. Once results are display to the screen temporary files should be deleted. **

### How to run
* Open a terminal and navigate to the source code
* $make to compile our program
* $./mash to call our program to run
* input commands and filename when prompt. 

### Possible commands
* “wc”                   Reports the line count, word count, and character count
* “md5sum”               Generates a unique 128-bit md5 (checksum) hash message digest
* “grep –c the”          Counts the number of occurrences of a given word, here “the”
* “grep –ci the”         Counts the number of occurrences of a given word ignoring case, here “the”
* “tail –n 10”           outputs 10 lines from the end of a file
* “head –n 10 ”          outputs 10 lines from the start of a file
* “ls –l”                provides a long directory listing

### Example run
$ ./mash
mash-1>grep -c the
mash-2>md5sum
mash-3>wc -l
file>googlebig.txt
First process finished...
Second process finished...
Third process finished...
-----CMD 1: grep -c the--------------------------------------------------------- 
15697
Result took:314ms
-----CMD 2: md5sum-------------------------------------------------------------- 
0362a7bf9035eba363462ea484bb43a6 googlebig.txt
Result took:2664ms
-----CMD 3: wc -l--------------------------------------------------------------- 
76860248 googlebig.txt
Result took:803ms 

Children process IDs: 12287 12289 12288.
Total elapsed time:2664ms