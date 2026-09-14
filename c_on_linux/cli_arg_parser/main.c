// Luis Sanchez, 2026. 
// /Built on the assumption of the following assignment Grammar:
// In the grammar below, [sth] denotes sth inside [ ] is optional; “|” denotes Unix pipe;
// “*” denotes 0 or >=1 occurrences. The symbol “>” could be “>>” too.
// command line → cmd [< fn] [| cmd]* [> fn] [&] EOL
// cn -> string // command name
// fn -> string // file name
// ar -> string //argument

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
    char command_buffer[MAX_OUTPUT];
    int input_file_count;
    int output_file_count;
    int command_count;
    bool is_background;
} ParsedCommand;

bool is_operator_symbol(char *user_input_part)
{
    bool is_file_input = strcmp(user_input_part, "<") == 0;
    bool is_file_output = strcmp(user_input_part, ">") == 0 || strcmp(user_input_part, ">>") == 0;
    bool is_background_task = strcmp(user_input_part, "&") == 0;
    bool is_pipe = strcmp(user_input_part, "|") == 0;
    bool is_forward_slash = strcmp(user_input_part, "/") == 0;
    bool is_at_symbol = strcmp(user_input_part, "@") == 0;
    bool is_hashtag = strcmp(user_input_part, "#") == 0;

    return is_file_input || is_file_output || is_background_task || is_pipe || is_forward_slash | is_at_symbol | is_hashtag;
}

bool is_invalid_grammar(char **user_input_parts, int input_count)
{
    if (input_count == 0)
    {
        printf("Please enter valid input! \n");
        return true;
    }

    // first item needs to be a command
    if (is_operator_symbol(user_input_parts[0]))
    {
        printf("Please enter a valid command! \n");
        return true;
    }

    bool has_file_input_operand = false;
    bool has_file_after_input_operand = false;
    bool has_extra_file_input_operand = false;
    bool has_file_output_operand = false;
    bool has_file_after_output_operand = false;
    bool has_extra_file_output_operand = false;
    bool has_operands_in_right_order = false;
    bool has_background_task_operand = false;
    int file_input_idx = 0;
    int file_output_idx = 0;
    int background_task_idx = 0;

    for (int i = 0; i < input_count; i++)
    {
        if (has_file_input_operand && strcmp(user_input_parts[i], "<") == 0 && file_input_idx)
        {
            has_extra_file_input_operand = true;
        }
        if (has_file_output_operand && (strcmp(user_input_parts[i], ">") == 0 || strcmp(user_input_parts[i], ">>") == 0) && file_output_idx)
        {
            has_extra_file_output_operand = true;
        }
        if (strcmp(user_input_parts[i], "<") == 0 && file_input_idx == 0)
        {
            has_file_input_operand = true;
            file_input_idx = i;

            if (i + 1 < input_count && !is_operator_symbol(user_input_parts[i + 1]))
            {
                has_file_after_input_operand = true;
            }
        }
        if ((strcmp(user_input_parts[i], ">") == 0 || strcmp(user_input_parts[i], ">>") == 0) && file_output_idx == 0)
        {
            has_file_output_operand = true;
            file_output_idx = i;

            if (i + 1 < input_count && !is_operator_symbol(user_input_parts[i + 1]))
            {
                has_file_after_output_operand = true;
            }
        }
        if (has_file_input_operand && has_file_output_operand)
        {
            has_operands_in_right_order = file_input_idx < file_output_idx;
        }
        if (strcmp(user_input_parts[i], "&") == 0 && background_task_idx == 0)
        {
            has_background_task_operand = true;
            background_task_idx = i;
        }
    }

    // If < && >/>>, are they in the right order?
    if (!has_operands_in_right_order && has_file_input_operand && has_file_output_operand)
    {
        printf("Please enter file operands in correct order!\n");
        return true;
    }
    // If <, does it only appear once? & does it have a filename afterwards?
    if (has_file_input_operand && (has_extra_file_input_operand || !has_file_after_input_operand))
    {
        printf("Please enter a valid file input command!\n");
        return true;
    }
    // if >/>>, does it only appear once? & does it have a filename afterwards
    if (has_file_output_operand && (has_extra_file_output_operand || !has_file_after_output_operand))
    {
        printf("Please enter a valid file output command!\n");
        return true;
    }
    // if &, is it in the right spot at the end?
    if (has_background_task_operand && background_task_idx != input_count - 1)
    {
        printf("Pleas enter a valid background task command!\n");
        return true;
    }

    return false;
}

bool is_pipeline_grammar_invalid(ParsedCommand *pipeline, int pipeline_count)
{
    int last_idx = pipeline_count - 1;

    for (int i = 0; i < pipeline_count; i++)
    {
        if (pipeline[i].command_count == 0)
        {
            printf("Please enter a command between every pipe!\n");
            return true;
        }
        // ensure we only have one entry (the 1st) as a file input
        if (i != 0 && pipeline[i].input_file_count > 0)
        {
            printf("Please enter a valid file input command!\n");
            return true;
        }
        // ensure we only have the file output in the right pipeline slot
        if (i != last_idx && pipeline[i].output_file_count > 0)
        {
            printf("Please enter a valid file output command!\n");
            return true;
        }
        // ensure we only have the background at the last pipeline slot
        if (i != last_idx && pipeline[i].is_background)
        {
            printf("Pleas enter a valid background task command!\n");
            return true;
        }
    }

    return false;
}

void parse_command_segment(char *segment, ParsedCommand *output)
{
    char *user_input_parts[MAX_INPUT];
    // Prevents losing where we are splitting strings!
    char *segment_saved_strtok_ptr;
    // Input could be tabbed or newlined if copy/pasted into shell
    char *input_segment = strtok_r(segment, " \t\n", &segment_saved_strtok_ptr);
    int input_count = 0;

    while (input_segment != NULL && input_count < MAX_INPUT)
    {
        user_input_parts[input_count] = input_segment;
        input_count++;

        input_segment = strtok_r(NULL, " \t\n", &segment_saved_strtok_ptr);
    }

    if (is_invalid_grammar(user_input_parts, input_count))
    {
        return;
    }

    for (int i = 0; i < input_count; i++)
    {
        if (strcmp(user_input_parts[i], "<") == 0 && i + 1 < input_count)
        {
            output->input_files[output->input_file_count] = user_input_parts[i + 1];
            output->input_file_count++;
            // At this point, we have a valid grammar so skip over the next item to properly capture command args
            i++;
        }
        else if ((strcmp(user_input_parts[i], ">") == 0 || strcmp(user_input_parts[i], ">>") == 0) && i + 1 < input_count)
        {
            output->output_files[output->output_file_count] = user_input_parts[i + 1];
            output->output_file_count++;
            // At this point, we have a valid grammar so skip over the next item to properly capture command args
            i++;
        }
        else if (strcmp(user_input_parts[i], "&") == 0 && i == input_count - 1)
        {
            output->is_background = true;
        }
        else
        {
            if (output->command_buffer[0] != '\0')
            {
                strcat(output->command_buffer, " ");
            }
            strcat(output->command_buffer, user_input_parts[i]);
        }
    }
    if (output->command_buffer[0] != '\0')
    {
        output->commands[output->command_count] = output->command_buffer;
        output->command_count++;
    }
}

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

void print_parsed_command(ParsedCommand *parsed_command)
{
    char pretty_input_files[MAX_OUTPUT] = {0};
    char pretty_output_files[MAX_OUTPUT] = {0};
    char pretty_command_count[MAX_OUTPUT] = {0};

    printf("Commands: %s \n", combine_items(parsed_command->commands, parsed_command->command_count, pretty_command_count));
    printf("Input file: %s \n", parsed_command->input_file_count > 0 ? combine_items(parsed_command->input_files, parsed_command->input_file_count, pretty_input_files) : "None");
    printf("Output file: %s \n", parsed_command->output_file_count > 0 ? combine_items(parsed_command->output_files, parsed_command->output_file_count, pretty_output_files) : "None");
    printf("Background or Not: %s\n\n", parsed_command->is_background ? "Yes" : "No");
}

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
            user_input[strcspn(user_input, "\r\n")] = '\0';
            if (strcmp(user_input, "exit") == 0)
            {
                printf("Bye bye!\n");
                return 0;
            }

            if (strchr(user_input, '|') == NULL)
            {
                ParsedCommand parsed_command = {0};
                parse_command_segment(user_input, &parsed_command);

                if (parsed_command.command_count == 0)
                {
                    continue;
                }
                else
                {
                    print_parsed_command(&parsed_command);
                }
            }
            else
            {
                char *cursor = user_input;
                int pipeline_count = 0;
                ParsedCommand pipeline[MAX_INPUT] = {0};

                while (pipeline_count < MAX_INPUT)
                {
                    char *pipeline_location = strchr(cursor, '|');

                    if (pipeline_location != NULL)
                    {
                        *pipeline_location = '\0';
                    }

                    parse_command_segment(cursor, &pipeline[pipeline_count]);
                    pipeline_count++;

                    if (pipeline_location == NULL)
                    {
                        break;
                    }

                    cursor = pipeline_location + 1;
                }

                if (is_pipeline_grammar_invalid(pipeline, pipeline_count))
                {
                    continue;
                }
                else
                {
                    ParsedCommand combined_commands = {0};

                    aggregate_pipeline(pipeline, pipeline_count, &combined_commands);
                    print_parsed_command(&combined_commands);
                }
            }
        }
        else
        {
            printf("\nBye bye!\n");
            return 0;
        }
    }
}
