#pragma once

int check_input(const char* input);
int check_circle(char* input);
void handle_error(const char* error_msg, const char* input, int pos);
void calculate_perimeter(double radius, double* p);
void calculate_area(double radius, double* s);