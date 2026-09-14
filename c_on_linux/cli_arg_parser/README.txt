Assignment 1 - A Simple Shell
Luis Sanchez

1. How to build

    make

This compiles main.c into an executable named "main" using gcc.

To remove the compiled binary:

    make clean

2. How to launch and quit

How to Launch:

    ./main

The shell will print "Enter your commands: " and wait for input.

How to Quit:

Type "exit" and press Enter, or send EOF with Ctrl+C. 
Either way the shell prints "Bye bye!" and exits.

3. Example inputs and expected outputs

Valid grammar:

    Enter your commands: ls -l
    Commands: ls -l
    Input file: None
    Output file: None
    Background or Not: No

Valid, full pipeline with input/output redirection and background:

    Enter your commands: cat < aaa | more | more | grep 2 | sort | head | wc > bbb &
    Commands: cat, more, more, grep 2, sort, head, wc
    Input file: aaa
    Output file: bbb
    Background or Not: Yes

Invalid, command line cannot start with an operator:

    Enter your commands: & ls
    Please enter a valid command!

Invalid, "<" must be followed by a filename, not another command:

    Enter your commands: < in.txt cat
    Please enter a valid command!

Invalid, only one file input operand is allowed:

    Enter your commands: cat < aaa > bbb < ccc
    Please enter a valid file input command!

Invalid, empty command between/after pipes:

    Enter your commands: ls |
    Please enter valid input!
    Please enter a command between every pipe!

Quitting:

    Enter your commands: exit
    Bye bye!
