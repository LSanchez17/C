// TODO:
//             &  <-- invalid
//             < input.txt <-- invalid invalid
//             sleep 5 & echo "still going" <-- valid 2 commands

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_OUTPUT 512
#define MAX_INPUT 128

typedef struct
{
    char *commands[MAX_OUTPUT];
    char *input_files[MAX_OUTPUT];
    char *output_files[MAX_OUTPUT];
    int input_file_count;
    int output_file_count;
    int command_count;
    bool is_background;
} ParsedCommand;

// parse a command input segment to check for file operands, commands, etc
void parse_command_segment(char *segment, ParsedCommand *output)
{
    char *split_user_input[MAX_INPUT];
    // Prevents losing where we are splitting strings!
    char *segment_saved_strtok_ptr;
    // Input could be tabbed or newlined if copy/pasted into shell
    char *input_segment = strtok_r(segment, " \t\n", &segment_saved_strtok_ptr);
    int input_count = 0;

    while (input_segment != NULL && input_count < MAX_INPUT)
    {
        split_user_input[input_count] = input_segment;
        input_count++;

        input_segment = strtok_r(NULL, " \t\n", &segment_saved_strtok_ptr);
    }

    // now loop and keep track of what is happening
    for (int i = 0; i < input_count; i++)
    {
        // 1st item is the command
        if (i == 0)
        {
            output->commands[output->command_count] = split_user_input[i];
            output->command_count++;
        }
        // Do we have file input
        if (strcmp(split_user_input[i], "<") == 0 && i + 1 < input_count)
        {
            output->input_files[output->input_file_count] = split_user_input[i + 1];
            output->input_file_count++;
        }
        // Do we have a file output
        else if ((strcmp(split_user_input[i], ">") == 0 || strcmp(split_user_input[i], ">>") == 0) && i + 1 < input_count)
        {
            output->output_files[output->output_file_count] = split_user_input[i + 1];
            output->output_file_count++;
        }
        // Is this a background task
        else if (strcmp(split_user_input[i], "&") == 0)
        {
            output->is_background = true;
        }
    }
}

// output the array as one string using a buffer technique
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

// Cleanly print the result of parsing input
void print_parsed_command(ParsedCommand *parsed_command)
{
    char pretty_input_files[MAX_OUTPUT] = {0};
    char pretty_output_files[MAX_OUTPUT] = {0};
    char pretty_command_count[MAX_OUTPUT] = {0};

    printf("Commands: %s \n", combine_items(parsed_command->commands, parsed_command->command_count, pretty_command_count));
    printf("Input file(s): %s \n", parsed_command->input_file_count > 0 ? combine_items(parsed_command->input_files, parsed_command->input_file_count, pretty_input_files) : "None");
    printf("Output file(s): %s \n", parsed_command->output_file_count > 0 ? combine_items(parsed_command->output_files, parsed_command->output_file_count, pretty_output_files) : "None");
    printf("Background or Not: %s\n\n", parsed_command->is_background ? "Yes" : "No");
}

// Build a clean struct of all piped command structs
void aggregate_pipeline(ParsedCommand *pipeline, int pipeline_count, ParsedCommand *output)
{
    for (int i = 0; i < pipeline_count; i++)
    {
        for (int j = 0; j < pipeline[i].command_count; j++)
        {
            output->commands[output->command_count] = pipeline[i].commands[j];
            output->command_count++;
        }

        for (int j = 0; j < pipeline[i].input_file_count; j++)
        {
            output->input_files[output->input_file_count] = pipeline[i].input_files[j];
            output->input_file_count++;
        }

        for (int j = 0; j < pipeline[i].output_file_count; j++)
        {
            output->output_files[output->output_file_count] = pipeline[i].output_files[j];
            output->output_file_count++;
        }

        output->is_background = output->is_background || pipeline[i].is_background;
    }
}

int main()
{
    while (true)
    {
        char user_input[MAX_OUTPUT];

        printf("Enter your commands: ");

        if (fgets(user_input, sizeof user_input, stdin) != NULL)
        {
            if (strchr(user_input, '|') == NULL)
            {
                ParsedCommand parsed_command = {0};
                parse_command_segment(user_input, &parsed_command);

                if (parsed_command.command_count == 0)
                {
                    printf("Please enter a valid command\n");
                }
                else
                {
                    print_parsed_command(&parsed_command);
                }
            }
            else
            {
                // Prevents losing where we are splitting strings!
                char *saved_strtok_ptr;
                char *segment = strtok_r(user_input, "|", &saved_strtok_ptr);
                int pipeline_count = 0;
                ParsedCommand pipeline[MAX_INPUT] = {0};

                while (segment != NULL)
                {
                    parse_command_segment(segment, &pipeline[pipeline_count]);
                    pipeline_count++;

                    segment = strtok_r(NULL, "|", &saved_strtok_ptr);
                }

                ParsedCommand combined_commands = {0};

                aggregate_pipeline(pipeline, pipeline_count, &combined_commands);
                print_parsed_command(&combined_commands);
            }
        }
        else
        {
            printf("\nBye bye!\n");
            return 0;
        }
    }
}