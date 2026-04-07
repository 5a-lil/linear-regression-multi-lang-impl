
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

bool is_number(const char *str)
{
    if (!str) return false;
    char *end_ptr;
    strtod(str, &end_ptr);
    bool is_number = *end_ptr == 0;
    return (is_number);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Error: wrong number of arguments\n");
        return 1;
    }

    if (!is_number(argv[1])) {
        printf("Error: argument is not a number\n");
        return 1;
    }

    FILE *fd = fopen(".thetas", "r");
    if (fd == NULL) {
        printf("Error: failed opening \"%s\"\n", argv[1]);
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    size_t read_bytes = 0;

    read_bytes = getline(&line, &len, fd);
    double theta0 = strtod(line, NULL);
    read_bytes = getline(&line, &len, fd);
    double theta1 = strtod(line, NULL);

    double input_km = strtod(argv[1], NULL);
    double output_price = theta0 + theta1 * strtod(argv[1], NULL);

    printf("%f km -> %f $\n", input_km, output_price);

    free(line);
    fclose(fd);
}