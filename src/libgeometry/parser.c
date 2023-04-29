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

int validate_input(
        char* input,
        double* x1,
        double* y1,
        double* radius1,
        double* x2,
        double* y2,
        double* radius2)
{
    char* start_ptr = input;

    // Проверяем первую окружность
    if (check_prefix(input)) {
        handle_error("expected '('", start_ptr, 6);
        return -1;
    }

    int prefix_len = strlen("circle(");
    input += prefix_len;

    if (check_x(input, x1)) {
        handle_error("expected '<double>'", start_ptr, input - start_ptr);
        return -1;
    }

    char* end_ptr;
    strtod(input, &end_ptr);
    input = end_ptr + 1;

    if (check_y(input, y1)) {
        handle_error("expected ','", start_ptr, input - start_ptr);
        return -1;
    }

    strtod(input, &end_ptr);
    input = end_ptr + 1;

    if (check_radius(input, radius1)) {
        handle_error("expected ')'", start_ptr, input - start_ptr);
        return -1;
    }

    strtod(input, &end_ptr);
    input = end_ptr + 2; // Пропускаем символы ')' и '\n'

    // Проверяем вторую окружность
    if (check_prefix(input)) {
        handle_error("expected '('", start_ptr, input - start_ptr);
        return -1;
    }

    input += prefix_len;

    if (check_x(input, x2)) {
        handle_error("expected '<double>'", start_ptr, input - start_ptr);
        return -1;
    }

    strtod(input, &end_ptr);
    input = end_ptr + 1;

    if (check_y(input, y2)) {
        handle_error("expected ','", start_ptr, input - start_ptr);
        return -1;
    }

    strtod(input, &end_ptr);
    input = end_ptr + 1;

    if (check_radius(input, radius2)) {
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

void calculate_perimeter(double radius, double* p)
{
    *p = 2 * M_PI * radius;
}

void calculate_area(double radius, double* s)
{
    *s = M_PI * radius * radius;
}

int intersect_circles(
        double x1,
        double y1,
        double r1,
        double x2,
        double y2,
        double r2,
        double* x3,
        double* y3,
        double* x4,
        double* y4)
{
    double d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    if (d > r1 + r2 || d < fabs(r1 - r2)) {
        // circles do not intersect
        return 0;
    }
    double a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    double h = sqrt(r1 * r1 - a * a);
    double x0 = x1 + a * (x2 - x1) / d;
    double y0 = y1 + a * (y2 - y1) / d;
    *x3 = x0 + h * (y2 - y1) / d;
    *y3 = y0 - h * (x2 - x1) / d;
    *x4 = x0 - h * (y2 - y1) / d;
    *y4 = y0 + h * (x2 - x1) / d;
    return 1;
}

void output_results(
        double x1,
        double y1,
        double radius1,
        double p1,
        double s1,
        double x2,
        double y2,
        double radius2,
        double p2,
        double s2,
        int intersect,
        double x3,
        double y3,
        double x4,
        double y4)
{
    printf("First circle: circle(%.1f %.1f, %.1f)\n", x1, y1, radius1);
    printf("perimeter = %f\n", p1);
    printf("area = %f\n", s1);
    printf("Second circle: circle(%.1f %.1f, %.1f)\n", x2, y2, radius2);
    printf("perimeter = %f\n", p2);
    printf("area = %f\n", s2);
    if (intersect) {
        printf("Intersection points: (%.1f %.1f), (%.1f %.1f)\n",
               x3,
               y3,
               x4,
               y4);
    } else {
        printf("Circles do not intersect\n");
    }
    const char* output_f = "OutputGeometry.txt";
    FILE* file = fopen(output_f, "w");
    if (file == NULL) {
        printf("Error: can't create output file:\n");
        printf("%s\n", output_f);
        return;
    }
    fprintf(file, "First circle: circle(%.1f %.1f, %.1f)\n", x1, y1, radius1);
    fprintf(file, "perimeter = %f\n", p1);
    fprintf(file, "area = %f\n", s1);
    fprintf(file, "Second circle: circle(%.1f %.1f, %.1f)\n", x2, y2, radius2);
    fprintf(file, "perimeter = %f\n", p2);
    fprintf(file, "area = %f\n", s2);
    if (intersect) {
        fprintf(file,
                "Intersection points: (%.1f %.1f), (%.1f %.1f)\n",
                x3,
                y3,
                x4,
                y4);
    } else {
        fprintf(file, "Circles do not intersect\n");
    }
    fclose(file);
}

int check_circle(char* input)
{
    double x1, y1, radius1;
    double x2, y2, radius2;
    if (validate_input(input, &x1, &y1, &radius1, &x2, &y2, &radius2) == -1) {
        return -1;
    }
    double p1, s1;
    calculate_perimeter(radius1, &p1);
    calculate_area(radius1, &s1);

    double p2, s2;
    calculate_perimeter(radius2, &p2);
    calculate_area(radius2, &s2);
    double x3, y3, x4, y4;
    int intersect = intersect_circles(
            x1, y1, radius1, x2, y2, radius2, &x3, &y3, &x4, &y4);
    output_results(
            x1,
            y1,
            radius1,
            p1,
            s1,
            x2,
            y2,
            radius2,
            p2,
            s2,
            intersect,
            x3,
            y3,
            x4,
            y4);
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