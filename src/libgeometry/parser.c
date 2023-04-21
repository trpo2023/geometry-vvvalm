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

int check_prefix(char* input)
{
    const char* prefix = "circle(";
    int prefix_len = strlen(prefix);
    if (strncmp(input, prefix, prefix_len)) {
        return -1;
    }
    return 0;
}

int check_x(char* input, double* x)
{
    char* end_ptr;
    *x = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ' ') {
        return -1;
    }
    return 0;
}

int check_y(char* input, double* y)
{
    char* end_ptr;
    *y = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ',') {
        return -1;
    }
    return 0;
}

int check_radius(char* input, double* radius)
{
    char* end_ptr;
    *radius = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ')') {
        return -1;
    }
    return 0;
}

int check_end(char* input)
{
    if (*input != '\0') {
        return -1;
    }
    return 0;
}

int validate_input(char* input, double* x, double* y, double* radius)
{
    char* start_ptr = input;

    if (check_prefix(input)) {
        handle_error("expected '('", start_ptr, 6);
        return -1;
    }

    int prefix_len = strlen("circle(");
    input += prefix_len;

    if (check_x(input, x)) {
        handle_error("expected '<double>'", start_ptr, input - start_ptr);
        return -1;
    }

    char* end_ptr;
    strtod(input, &end_ptr);
    input = end_ptr + 1;

    if (check_y(input, y)) {
        handle_error("expected ','", start_ptr, input - start_ptr);
        return -1;
    }

    strtod(input, &end_ptr);
    input = end_ptr + 1;

    if (check_radius(input, radius)) {
        handle_error("expected ')'", start_ptr, input - start_ptr);
        return -1;
    }

    strtod(input, &end_ptr);
    input = end_ptr + 1;

    end_ptr += 1;

    if (check_end(end_ptr)) {
        handle_error("unexpected token", start_ptr, end_ptr - start_ptr);
        return -1;
    }

    return 0;
}

void calculate_circle(double radius, double* p, double* s)
{
    *p = 2 * M_PI * radius;
    *s = M_PI * radius * radius;
}

void output_results(double x, double y, double radius, double p, double s)
{
    printf("circle(%.1f %.1f, %.1f)\n", x, y, radius);
    printf("perimeter = %f\n", p);
    printf("area = %f\n", s);

    const char* output_f = "OutputGeometry.txt";
    FILE* file = fopen(output_f, "w");
    if (file == NULL) {
        printf("Error: can't create output file:\n");
        printf("%s\n", output_f);
        return;
    }
    fprintf(file, "circle(%.1f %.1f, %.1f)\n", x, y, radius);
    fprintf(file, "perimeter = %f\n", p);
    fprintf(file, "area = %f\n", s);
    fclose(file);
}

int check_circle(char* input)
{
    double x, y, radius;
    if (validate_input(input, &x, &y, &radius) == -1) {
        return -1;
    }

    double p, s;

    calculate_circle(radius, &p, &s);

    output_results(x, y, radius, p, s);

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
