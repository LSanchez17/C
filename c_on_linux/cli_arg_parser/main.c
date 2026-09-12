/*
Parse arguments from the terminal and output the input as follows:
- Commands: ....
- input file: ... (None if "<" not used)
- Output file: ... (None if ">" and ">>" not used)
- Background: Yes/No (No if "&" not used)
*/  

// TODO:
// If no pipes in input
//  Singular:
//    split command by spaces
//    check validity against PATH? for command first (first item in spaces)
//    Once valid, check for file parsing (likely second command)
//    Lastly check for background (& symbol)
//    store data (commands, file input, file output, background)
//    output
//    Repeat
//  Multiple:
//    repeat the singular steps in a loop
//    once loop ends, output
//    repeat
// Loop entire program
// unit test?

# include <stdio.h>
# include <string.h>

#define MAX_INPUT 1024
#define MAX_OUTPUT 512
#define MAX_COMMANDS_AND_ARGS 128

int main() {
    // This is likely long enough to handle 99% of user cases
    char user_input[MAX_OUTPUT];
    char *file_inputs[MAX_OUTPUT];
    char *file_outputs[MAX_OUTPUT];
    char *commands[MAX_OUTPUT];
    char *command_and_args[MAX_COMMANDS_AND_ARGS]
    bool is_background_job;

    printf("Enter your commands: \n");

    if (fgets(user_input, sizeof user_input, stdin) != NULL) {
        // only "one" command came through, no pipes
        if (strchr(user_input, '|') == NULL) {
            // split command by spaces to get true list of arguments
            
            return 0;
        }


        int total_commands = 0;
        char *current_command = strtok(user_input, "|");

        while (current_command != NULL && total_commands < 512) {
            commands[total_commands] = current_command;
            current_command = strtok(NULL, "|");
            total_commands++;
        }

        for (int i = 0; i < total_commands; i++) {
            // Repeat singular command logic
        }

        
        return 0;
    }

    printf("No valid commands entered");

    return 0;
}