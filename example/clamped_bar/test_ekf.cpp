#define VERDANDI_DEBUG_LEVEL_4
#define SELDON_WITH_BLAS
#define SELDON_WITH_LAPACK

#define VERDANDI_WITH_ABORT
#define VERDANDI_DENSE

//#define VERDANDI_WITH_DIRECT_SOLVER
//#define SELDON_WITH_MUMPS
//#define VERDANDI_WITH_MPI
//#define VERDANDI_WITH_OMP

//#define VERDANDI_LOGGING_LEVEL -7

#if defined(VERDANDI_WITH_MPI)
#include <mpi.h>
#endif

#if defined(VERDANDI_WITH_OMP)
#include <omp.h>
#endif

#include "Verdandi.hxx"
#include "seldon/SeldonSolver.hxx"

#include "model/ClampedBar.cxx"
#include "model/CheckingModel.cxx"
#include "observation_manager/LinearObservationManager.cxx"
#include "method/ExtendedKalmanFilter.cxx"


int main(int argc, char** argv)
{

    VERDANDI_TRY;

    if (argc != 2)
    {
        string mesg  = "Usage:\n";
        mesg += string("  ") + argv[0] + " [configuration file]";
        std::cout << mesg << std::endl;
        return 1;
    }

    typedef double real;

    Verdandi::ExtendedKalmanFilter<
        Verdandi::CheckingModel<Verdandi::ClampedBar<real> >,
        Verdandi::LinearObservationManager<real> > driver;

    driver.Initialize(argv[1]);

    while (!driver.HasFinished())
    {
        driver.InitializeStep();
        driver.Forward();
        driver.Analyze();
        driver.FinalizeStep();
    }

    driver.Finalize();

    VERDANDI_END;

    return 0;

}
