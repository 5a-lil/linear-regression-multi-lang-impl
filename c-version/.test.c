
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

size_t m = 0;

void print_int_table(int **values)
{
    if (!values) return;
    size_t i = -1;
    while (++i < m)
    {
        printf("{ %d, %d }\n", values[i][0], values[i][1]);
    }
    printf("-------------------\n");
}

void add_value(int to_add[2], int ***values)
{
    m++;
    int **new_values = malloc(m * sizeof(int *));

    size_t i = -1;
    while (++i < m - 1)
    {
        new_values[i] = (*values)[i];
    }
    new_values[i] = to_add;

    int *temp_to_free = (void *)*values;
    *values = new_values;
    free(temp_to_free);
}

int main()
{
    int **values = NULL;
    int new[2] = {2, 3};
    add_value(new, &values);
    print_int_table(values);
    add_value((int[2]){ 5, 5 }, &values);
    print_int_table(values);
    add_value(new, &values);
    print_int_table(values);
    add_value(new, &values);
    print_int_table(values);
    free(values);
}