#include "parser.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100

void print_input(const char* input)
{
    fprintf(stderr, "%s\n", input);
}

void print_error_mark(int pos)
{
    for (int i = 0; i < pos; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^\n");
}

void print_error_message(const char* error_msg, int pos)
{
    fprintf(stderr, "Error at column %d: %s\n", pos, error_msg);
}

void handle_error(const char* error_msg, const char* input, int pos)
{
    print_input(input);
    print_error_mark(pos);
    print_error_message(error_msg, pos);
}

int check_circle(char* input)
{
    const char* prefix = "circle(";
    char* start_ptr = input;
    int prefix_len = strlen(prefix);
    double pi = M_PI, p, s;
    if (strncmp(input, prefix, prefix_len)) {
        handle_error("expected '('", start_ptr, 6);
        return -1;
    }
    input += prefix_len;
    char* end_ptr;
    double x = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ' ') {
        handle_error("expected '<double>'", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    input = end_ptr + 1;
    double y = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ',') {
        handle_error("expected ','", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    input = end_ptr + 1;
    double radius = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ')') {
        handle_error("expected ')'", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    end_ptr += 1;
    if (*end_ptr != '\0') {
        handle_error("unexpected token", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    p = 2 * pi * radius;
    s = pi * radius * radius;
    printf("circle(%.1f %.1f, %.1f)\n", x, y, radius);
    printf("perimeter = %f\n", p);
    printf("area = %f\n", s);

    const char* output_f = "OutputGeometry.txt";
    FILE* file = fopen(output_f, "w");
    if (file == NULL) {
        printf("Error: can't create output file:\n");
        printf("%s\n", output_f);
        return -1;
    }
    fprintf(file, "circle(%.1f %.1f, %.1f)\n", x, y, radius);
    fprintf(file, "perimeter = %f\n", p);
    fprintf(file, "area = %f\n", s);
    fclose(file);
    return 0;
}

int check_input(const char* input)
{
    if (strncmp(input, "circle", 6) == 0) {
        return check_circle((char*)input);
    }
    handle_error("expected 'circle'", input, 0);
    return -1;
}
