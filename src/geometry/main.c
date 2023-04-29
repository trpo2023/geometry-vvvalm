#include <libgeometry/parser.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char input[100];

    const char* input_f = "InputGeometry.txt";
    FILE* file = fopen(input_f, "r");
    if (file == NULL) {
        printf("Error: can't open input file:\n");
        printf("%s\n", input_f);
        return 1;
    }
    while (!feof(file)) {
        fgets(input, 100, file);
        input[strcspn(input, "\n")] = '\0';
        if (check_input(input)) {
            return 1;
        }
    }
    fclose(file);
    return 0;
}
