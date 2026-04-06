
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#define I 0
#define J 1
#define M 24
#define ITERS 1000

double **values = NULL;

size_t m = 0;
double theta0 = 0;
double theta1 = 0;
double learn_rate = 0.01;

//------------------------------------------------------------------------------------------------------------------------------------

double estimated_price(double x)
{
    return theta0 + theta1 * x;
}

void calc_new_thetas()
{
    double sum_estimated_prices_theta0 = 0;
    for (size_t i = 0; i < M; i++) {
        sum_estimated_prices_theta0 += estimated_price(values[i][I]) - values[i][J];
    }

    double sum_estimated_prices_theta1 = 0;
    for (size_t i = 0; i < M; i++) {
        sum_estimated_prices_theta1 += (estimated_price(values[i][I]) - values[i][J]) * values[i][I];
    }

    //------------------------------------------------------------------------------------------------------------------------------------

    double theta0_var = learn_rate * (sum_estimated_prices_theta0 / M);
    double theta1_var = learn_rate * (sum_estimated_prices_theta1 / M);
    theta0 -= theta0_var;
    theta1 -= theta1_var;
}

//------------------------------------------------------------------------------------------------------------------------------------

size_t strlen_without_spaces(const char *str)
{
    size_t i = 0;
    while (*str)
    {
        if (*str != ' ' && *str != '\n')
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
        if (*str != ' ' && *str != '\n')
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
    //------------------------------------------------------------------------------------------------------------------------------------
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
            if (line[0] == '\n') continue;

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
                printf("%f - ", num_second_value);
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
                printf("%f\n", num_first_value);
                free(str_first_value);
            }//-----------------------------------------------------------------------------------------------------------

            m++;
            double **new_values = malloc(m * sizeof(double *));
            if (!new_values) {
                free(values);
                failed_parsing = true;
                goto failed_parsing_goto;
            }

            size_t i = 0;
            printf("-------------\n");
            while (i < m - 1) 
            {
                printf("{ %f, %f }\n", values[i][0], values[i][1]);   
                new_values[i] = values[i];
                i++;
            }
            new_values[i] = (double[2]){ num_first_value, num_second_value };
            printf("DAME{ %f, %f }\n", values[i][0], values[i][1]);
        
            void *temp_to_free = values;
            values = new_values;
            free(temp_to_free);
        }

        size_t i = -1;
        while (++i < m)
        {
            printf("{ %f, %f }\n", values[i][0], values[i][1]);
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

    // for (size_t i = 0; i < M; i++)
    // {
    //     values[i][I] = (values[i][I] - 48235) / (240000 - 48235);
    // }

    // for (int i1 = 0; i1 < ITERS; i1++)
    // {
    //     calc_new_thetas();
    //     printf("theta0 = %f\ntheta1 = %f\n-------\n", theta0, theta1);
    // }
}
