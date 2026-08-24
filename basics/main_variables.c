#include <stdio.h>
#include <stdbool.h>

// useful info:
// int = whole number (4 bytes)
// float = single-precision decimal numbe (4 bytes)
// double = double-precision decimal number (8 bytes)
// char = single characters (1 byte)
// char[] = array of characters (size varies)
// bool = true or false (1 byte, requires <stdbool.h>!)


int main() {
    float gpa = 2.5; // similar to other floats-probably different memory wise
    float price = 19.99; // will display 6 digits as a default, have to set the max
    double pi = 3.14159265358979; // larger memory footprint for better precision
    char symbol = '!'; // like c#
    // char dog_names[] = ["Echo", "Kenji"]; <-- This is wrong!
    char *dog_names[] = {"Echo", "Kenji"}; // Creates a fixed storage based on the values. Good for Readonly
    char cat_names[2][20] = {"Izzy", "Stella"}; // up to 2 names, no more than 19 chars
    bool is_sunny = false; // night time

    // adding %__ and a letter afterwards helps format the variable
    printf("Your GPA IS: %f\n", gpa); // The %f is inserting the variable, kinda like python
    printf("Price: $%f:\n", price);
    printf("The value of 3 is: %.15lf\n", pi);
    printf("A char: %c\n ", symbol);
    printf("Dogs: %s\n", dog_names); // gives the memory address and gibberish
    printf("Cats: %s\n", cat_names); // just gives the first one

    if (is_sunny) {
        printf("It's sunny!");
    } else {
        printf("It is NOT sunny :(");
    }
    
    return 0;
}