// TODO:
//    Create function to run basic core parsing logic
//    check validity against PATH & Arg valid
//  Multiple:
//    do strok until no pipe
//    at each iteration, call the function above or a slightly different one
//    once loop ends, output

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_OUTPUT 512
#define MAX_INPUT 128

// edge cases: &, < input.txt - both should crash, but doing the validity of command would catch it?

// output the array as one string
char *combine_items(char **items, int total_items, char *buffer)
{
    for (int i = 0; i < total_items; i++)
    {
        buffer = strcat(buffer, items[i]);

        if (i != total_items - 1)
        {
            buffer = strcat(buffer, ", ");
        }
    }

    return buffer;
}

int main()
{
    while (true)
    {
        bool is_background = false;
        int command_count = 0;
        int input_count = 0;
        int input_file_count = 0;
        int output_file_count = 0;
        char user_input[MAX_OUTPUT];
        char *split_user_input[MAX_INPUT];
        char *commands[MAX_INPUT];
        char *input_files[MAX_OUTPUT];
        char *output_files[MAX_OUTPUT];

        printf("Enter your commands: ");

        if (fgets(user_input, sizeof user_input, stdin) != NULL)
        {
            // If we do not have a pipe in the input
            if (strchr(user_input, '|') == NULL)
            {
                // Input could be tabbed or newlined if copy/pasted into shell
                char *input_segment = strtok(user_input, " \t\n");

                while (input_segment != NULL && input_count < MAX_OUTPUT)
                {
                    split_user_input[input_count] = input_segment;
                    input_count++;

                    input_segment = strtok(NULL, " \t\n");
                }

                // now loop and keep track of what is happening
                for (int i = 0; i < input_count; i++)
                {
                    // 1st item is the command
                    if (i == 0)
                    {
                        commands[command_count] = split_user_input[i];
                        command_count++;
                    }
                    // Do we have file input
                    if (strcmp(split_user_input[i], "<") == 0 && i + 1 < input_count)
                    {
                        input_files[input_file_count] = split_user_input[i + 1];
                        input_file_count++;
                    }
                    // Do we have a file output
                    else if ((strcmp(split_user_input[i], ">") == 0 || strcmp(split_user_input[i], ">>") == 0) && i + 1 < input_count)
                    {
                        output_files[output_file_count] = split_user_input[i + 1];
                        output_file_count++;
                    }
                    // Is this a background task
                    else if (strcmp(split_user_input[i], "&") == 0)
                    {
                        is_background = true;
                    }
                }

                char pretty_commands[MAX_OUTPUT] = {0};
                char pretty_input_files[MAX_OUTPUT] = {0};
                char pretty_output_files[MAX_OUTPUT] = {0};

                printf("Commands: %s \n", combine_items(commands, command_count, pretty_commands));
                if (input_file_count > 0)
                {
                    printf("Input file(s): %s \n", combine_items(input_files, input_file_count, pretty_input_files));
                }
                if (output_file_count > 0)
                {
                    printf("Output file(s): %s \n", combine_items(output_files, output_file_count, pretty_output_files));
                }
                printf("Background or Not: %s\n\n", is_background ? "Yes" : "No");
            }
            else
            {
                // we have multiple commands!
            }
        }
        else
        {
            printf("\nBye bye!\n");
        }
    }

    return 0;
}