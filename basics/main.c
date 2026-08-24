// Preprocessors-Include the standard input output. Using a header function
# include <stdio.h>

// entry point is the `main` function for programs-Required
int main() {
    printf("Hello, World");
    // no hot reloading or rebuild. Must recompile to see newly added items
    printf("This is the log");

    return 0;
}

// gcc main.c -o (outputs into terminal) WUMBO (name of the executable)