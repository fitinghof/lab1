/***************************************************************************
 *
 * Sequential version of Gaussian elimination
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 4096

typedef double matrix[MAX_SIZE][MAX_SIZE];

int matrix_size;           /* matrix size		*/
int max_num_elements;      /* max number of element*/
char *Init;                /* matrix init type	*/
int PRINT;                 /* print switch		*/
matrix A;                  /* matrix A		*/
double vector_b[MAX_SIZE]; /* vector b             */
double vector_y[MAX_SIZE]; /* vector y             */

/* forward declarations */
void work(void);
void Init_Matrix(void);
void Print_Matrix(void);
void Init_Default(void);
int Read_Options(int, char **);

int main(int argc, char **argv)
{
    int i, timestart, timeend, iter;

    Init_Default();           /* Init default values	*/
    Read_Options(argc, argv); /* Read arguments	*/
    Init_Matrix();            /* Init the matrix	*/
    work();
    if (PRINT == 1)
        Print_Matrix();
}

void work(void)
{
    int i, j, row;

    /* Gaussian elimination algorithm, Algo 8.4 from Grama */
    for (row = 0; row < matrix_size; row++)
    { /* Outer loop */
        for (j = row + 1; j < matrix_size; j++)
        {
            A[row][j] = A[row][j] / A[row][row]; /* Division step */
        }
        vector_y[row] = vector_b[row] / A[row][row];
        A[row][row] = 1.0;

        for (i = row + 1; i < matrix_size; i++)
        {
            for (j = row + 1; j < matrix_size; j++)
            {
                A[i][j] = A[i][j] - A[i][row] * A[row][j]; /* Elimination step */
            }
            vector_b[i] = vector_b[i] - A[i][row] * vector_y[row];
            A[i][row] = 0.0;
        }
    }
}

void Init_Matrix()
{
    int i, j;

    printf("\nsize      = %dx%d ", matrix_size, matrix_size);
    printf("\nmaxnum    = %d \n", max_num_elements);
    printf("Init	  = %s \n", Init);
    printf("Initializing matrix...");

    if (strcmp(Init, "rand") == 0)
    {
        for (i = 0; i < matrix_size; i++)
        {
            for (j = 0; j < matrix_size; j++)
            {
                if (i == j) /* diagonal dominance */
                    A[i][j] = (double)(rand() % max_num_elements) + 5.0;
                else
                    A[i][j] = (double)(rand() % max_num_elements) + 1.0;
            }
        }
    }
    if (strcmp(Init, "fast") == 0)
    {
        for (i = 0; i < matrix_size; i++)
        {
            for (j = 0; j < matrix_size; j++)
            {
                if (i == j) /* diagonal dominance */
                    A[i][j] = 5.0;
                else
                    A[i][j] = 2.0;
            }
        }
    }

    /* Initialize vectors b and y */
    for (i = 0; i < matrix_size; i++)
    {
        vector_b[i] = 2.0;
        vector_y[i] = 1.0;
    }

    printf("done \n\n");
    if (PRINT == 1)
        Print_Matrix();
}

void Print_Matrix()
{
    int i, j;

    printf("Matrix A:\n");
    for (i = 0; i < matrix_size; i++)
    {
        printf("[");
        for (j = 0; j < matrix_size; j++)
            printf(" %5.2f,", A[i][j]);
        printf("]\n");
    }
    printf("Vector b:\n[");
    for (j = 0; j < matrix_size; j++)
        printf(" %5.2f,", vector_b[j]);
    printf("]\n");
    printf("Vector y:\n[");
    for (j = 0; j < matrix_size; j++)
        printf(" %5.2f,", vector_y[j]);
    printf("]\n");
    printf("\n\n");
}

void Init_Default()
{
    matrix_size = 2048;
    Init = "rand";
    max_num_elements = 15.0;
    PRINT = 0;
}

int Read_Options(int argc, char **argv)
{
    char *prog;

    prog = *argv;
    while (++argv, --argc > 0)
        if (**argv == '-')
            switch (*++*argv)
            {
            case 'n':
                --argc;
                matrix_size = atoi(*++argv);
                break;
            case 'h':
                printf("\nHELP: try sor -u \n\n");
                exit(0);
                break;
            case 'u':
                printf("\nUsage: gaussian [-n problemsize]\n");
                printf("           [-D] show default values \n");
                printf("           [-h] help \n");
                printf("           [-I init_type] fast/rand \n");
                printf("           [-m maxnum] max random no \n");
                printf("           [-P print_switch] 0/1 \n");
                exit(0);
                break;
            case 'D':
                printf("\nDefault:  n         = %d ", matrix_size);
                printf("\n          Init      = rand");
                printf("\n          maxnum    = 5 ");
                printf("\n          P         = 0 \n\n");
                exit(0);
                break;
            case 'I':
                --argc;
                Init = *++argv;
                break;
            case 'm':
                --argc;
                max_num_elements = atoi(*++argv);
                break;
            case 'P':
                --argc;
                PRINT = atoi(*++argv);
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog);
                break;
            }
}
