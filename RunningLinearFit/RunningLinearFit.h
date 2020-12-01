#ifndef RUNNINGLINEARFIT_H_INCLUDED
#define RUNNINGLINEARFIT_H_INCLUDED

#include <math.h>

class RunningLinearFit
{
private:
    const static int containerSize = 10;
    double x[containerSize];
    double y[containerSize];
    double m;
    double b;
    double R2;
    const double maxDifference = 0.05;


public:
    RunningLinearFit::RunningLinearFit();
    int DetermineContainerLength();
    void LinearRegression();
    double DetermineY(double x_input);
    void AddData(double x_input, double y_input);
    double PercentDifference(double x_input, double y_input);
    double ReturnBestY(double x_input, double y_input);
    double GetM();
    double GetB();
    double GetR2();
    double GetMaxDifference();
    void Clear();

};

#endif // RUNNINGLINEARFIT_H_INCLUDED
