#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int id, nproc;
    MPI_Status status;
    if (argc < 2)
        return 1;
    float number = atol(argv[1]), factorial = 1, constant = 0, allconst, counter = atol(argv[1]), variable = 0;
    float euler = 0;

    // Initialize MPI:
    MPI_Init(&argc, &argv);
    // Get my rank:
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    // Get the total number of processors:
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    while (counter > 1)
    {
        variable = number;

        factorial = 1;
        while (number > 1)
        {
            factorial = factorial * number;
            number--;
        } //number is now 1

        constant = (1 / factorial) + constant;
        counter--;
        variable = variable - 1;
        number = variable;
    }

    //Slave:
    if (id != 0)
    {
        MPI_Send(&constant, 1, MPI_LONG_LONG, 0, 1, MPI_COMM_WORLD);
    }

    //Master:
    else
    {
        allconst = constant;
        for (int i = 1; i < nproc; ++i)
        {
            MPI_Recv(&constant, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            allconst += constant;
        }

        euler = allconst + 1 + (1 / 1.f); //the 1 and 1/1! in the original formula...
    }

    printf("************The approximation of euler number ************")
        printf("e = : %.10f\n", euler);
    // Terminate MPI:
    MPI_Finalize();
    return 0;
}