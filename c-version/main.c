
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#define I 0
#define J 1
#define M 24
#define ITERS 10000

double (*values)[2] = NULL;

size_t m = 0;
double theta0 = 0;
double theta1 = 0;
double learn_rate = 0.1;

//------------------------------------------------------------------------------------------------------------------------------------

double estimated_price(double x)
{
    return theta0 + theta1 * x;
}

void calc_new_thetas()
{
    double sum_estimated_prices_theta0 = 0;
    for (size_t i = 0; i < m; i++) {
        sum_estimated_prices_theta0 += estimated_price(values[i][I]) - values[i][J];
    }

    double sum_estimated_prices_theta1 = 0;
    for (size_t i = 0; i < m; i++) {
        sum_estimated_prices_theta1 += (estimated_price(values[i][I]) - values[i][J]) * values[i][I];
    }

    //------------------------------------------------------------------------------------------------------------------------------------

    double theta0_var = learn_rate * (sum_estimated_prices_theta0 / m);
    double theta1_var = learn_rate * (sum_estimated_prices_theta1 / m);
    theta0 -= theta0_var;
    theta1 -= theta1_var;
}

//------------------------------------------------------------------------------------------------------------------------------------

size_t strlen_without_spaces(const char *str)
{
    size_t i = 0;
    while (*str)
    {
        if (*str != ' ' && *str != '\n' && *str != '\r')
            i++;
        str++;
    }
    return i;
}

char *epur_line_from_space(const char *str)
{
    if (!str) return NULL;
    char *res = malloc(strlen_without_spaces(str) + 1);
    if (!res)
    {
        printf("Malloc failed in function %s\n", __func__);
        return NULL;
    }
    size_t i = 0;
    while (*str)
    {
        if (*str != ' ' && *str != '\n' && *str != '\r')
            res[i++] = *str;
        str++;
    }
    res[i] = 0;
    return res;
}

bool is_float(const char *str)
{
    if (!str) return false;
    char *end_ptr;
    strtod(str, &end_ptr);
    end_ptr = epur_line_from_space(end_ptr);
    bool is_float = *end_ptr == 0;
    free(end_ptr);
    return (is_float);
}

int main(int argc, char **argv)
{
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------

    {
        bool failed_parsing = false;
        if (argc != 2) {
            printf("Error: need only one argument <filename>.csv\n");
            exit(1);
        }

        char *strrchr_res = strrchr(argv[1], '.');
        if (!strrchr_res || strcmp(strrchr_res, ".csv")) {
            printf("Error: wrong file format \"%s\"\n", argv[1]);
            exit(1);
        }

        FILE *fd = fopen(argv[1], "r");
        if (fd == NULL) {
            printf("Error: failed opening \"%s\"\n", argv[1]);
            exit(1);
        }

        char *line = NULL;
        size_t len = 0;
        size_t read_bytes = 0;
        
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------

        if ((read_bytes = getline(&line, &len, fd)) != -1) {
            char *comma_finder = NULL;

            // are there more than two commas ??
            comma_finder = strchr(line, ',');
            if (comma_finder == NULL) {
                printf("Error: header error\n");
                failed_parsing = true;
                goto failed_parsing_goto;
            }

            comma_finder = strchr(comma_finder + 1, ',');
            if (comma_finder != NULL) {
                printf("Error: header error\n");
                failed_parsing = true;
                goto failed_parsing_goto;
            }

        } else {
            printf("Error: empty dataset file\n");
            goto failed_parsing_goto;
        }

        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------

        while ((read_bytes = getline(&line, &len, fd)) != -1) {
            if (line[0] == '\n' || (line[0] == '\r' && line[1] == '\n')) continue;

            char *str_first_value = NULL;
            char *str_second_value = NULL;
            double num_first_value = 0;
            double num_second_value = 0;

            {//-----------------------------------------------------------------------------------------------------------
                // is there a comma to separate with the second number ??
                str_second_value = strchr(line, ',') + 1;
                if (str_second_value == NULL + 1) {
                    printf("Error: no comma found in line\n");
                    failed_parsing = true;
                    goto failed_parsing_goto;
                }
                
                // is the number a float ???
                if (!is_float(str_second_value)){
                    printf("Error: str_second_value is not a float\n");
                    failed_parsing = true;
                    goto failed_parsing_goto;
                }

                num_second_value = strtod(str_second_value, NULL);
            }//-----------------------------------------------------------------------------------------------------------

            {//-----------------------------------------------------------------------------------------------------------
                size_t i = 0;
                while (line[i] && line[i] != ',')
                    i++;
                if (!(str_first_value = malloc(i + 1))) {
                    printf("Malloc failed in function %s\n", __func__);
                    goto failed_parsing_goto;
                }
                i = -1;
                while (line[++i] && line[i] != ',')
                    str_first_value[i] = line[i];
                str_first_value[i] = 0;

                // is the number a float ???
                if (!is_float(str_first_value)){
                    printf("Error: str_first_value is not a float\n");
                    failed_parsing = true;
                    goto failed_parsing_goto;
                }

                num_first_value = strtod(str_first_value, NULL);
                free(str_first_value);
            }//-----------------------------------------------------------------------------------------------------------

            m++;
            double (*new_values)[2] = malloc(m * sizeof(*values));
            if (!new_values) {
                free(values);
                failed_parsing = true;
                goto failed_parsing_goto;
            }

            size_t i = 0;
            while (i < m - 1) 
            {
                new_values[i][0] = values[i][0];
                new_values[i][1] = values[i][1];
                i++;
            }
            new_values[i][0] = num_first_value;
            new_values[i][1] = num_second_value;
        
            void *temp_to_free = values;
            values = new_values;
            free(temp_to_free);
        }

        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------------

        failed_parsing_goto:
            free(line);
            fclose(fd);

            if (failed_parsing) {
                printf("Error: failed parsing\n");
                return 1;
            }

    }

    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------

    {
        double values_min_x = values[0][I];
        for (size_t i = 0; i < m; i++) {
            if (values[i][I] < values_min_x)
                values_min_x = values[i][I];
        }

        double values_max_x = values[0][I];
        for (size_t i = 0; i < m; i++) {
            if (values[i][I] > values_max_x)
                values_max_x = values[i][I];
        }

        for (size_t i = 0; i < m; i++) {
            values[i][I] = (values[i][I] - values_min_x) / (values_max_x - values_min_x);
        }

        for (int i1 = 0; i1 < ITERS; i1++) {
            calc_new_thetas();
            printf("theta0 = %f\ntheta1 = %f\n-------\n", theta0, theta1);
        }

        printf("%f\n", estimated_price((176000 - values_min_x) / (values_max_x - values_min_x)));

        free(values);
    }
}
