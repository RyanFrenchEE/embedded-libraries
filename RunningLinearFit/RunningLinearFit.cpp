#include "RunningLinearFit.h"

RunningLinearFit::RunningLinearFit()
{
    for(int i = 0; i < containerSize; i++)
    {
        x[i] = -99;
        y[i] = -99;
    }
}


int RunningLinearFit::DetermineContainerLength()
{
    int length = 0;
    for(int i = 0; i < containerSize; i++)
    {
        if(x[i] != -99)
        {
            length++;
        }
    }

    return length;
}


void RunningLinearFit::LinearRegression()
{
    double sumX = 0.0;
    double sumX2 = 0.0;
    double sumXY = 0.0;
    double sumY = 0.0;
    double sumY2 = 0.0;
    int length = DetermineContainerLength();
    if(length == 0 || length == 1)
    {
        return;
    }

    for(int i = 0; i < length; i++)
    {
        sumX += x[i];
        sumX2 += pow(x[i], 2);
        sumXY += x[i] * y[i];
        sumY += y[i];
        sumY2 += pow(y[i], 2);
    }

    double denominator = (length * sumX2 - pow(sumX, 2));

    if(denominator == 0)
    {
        m = 0;
        b = 0;

    }

    m = (length * sumXY - sumX * sumY) / denominator;
    b = (sumY * sumX2 - sumX * sumXY) / denominator;

    double r = ((length * sumXY - sumX * sumY)/sqrt((length * sumX2 - pow(sumX,2)) * (length * sumY2 - pow(sumY,2))));

    R2 = pow(r, 2);
}


double RunningLinearFit::DetermineY(double x_input)
{
    double inputX = x_input;
    double y = (m * inputX) + b;

    return y;
}

void RunningLinearFit::AddData(double x_input, double y_input)
{
    double inputX = x_input;
    double inputY = y_input;
    int length = DetermineContainerLength();

    if(length < containerSize)
    {
        for(int i = length; i > 0; i--)
        {
            x[i] = x[i-1];
            y[i] = y[i-1];
        }
        x[0] = inputX;
        y[0] = inputY;
    }
    else if(length == containerSize)
    {
        for(int i = length - 1; i > 0; i--)
        {
            x[i] = x[i-1];
            y[i] = y[i-1];
        }
        x[0] = inputX;
        y[0] = inputY;
    }
    else
    {
        x[0] = inputX;
        y[0] = inputY;
    }
}


double RunningLinearFit::PercentDifference(double x_input, double y_input)
{
    double inputX = x_input;
    double inputY = y_input;
    double yTheoretical = DetermineY(inputX);
    if(yTheoretical == 0.0)
    {
        yTheoretical = 0.00000001;
    }
    double percentDifference = (fabs(yTheoretical - inputY)) / yTheoretical;

    return percentDifference;
}

void RunningLinearFit::Clear()
{
    for(int i = 0; i < containerSize; i++)
    {
        x[i] = -99;
        y[i] = -99;
    }
}

double RunningLinearFit::ReturnBestY(double x_input, double y_input)
{
    double inputX = x_input;
    double inputY = y_input;
    double percentDifference = PercentDifference(inputX, inputY);

    if(fabs(percentDifference) <= maxDifference)
    {
        return inputY;
    }
    else
    {
        double outputY = DetermineY(inputX);
        return outputY;
    }
}

double RunningLinearFit::GetM(){ return m; }
double RunningLinearFit::GetB(){ return b; }
double RunningLinearFit::GetR2(){ return R2; }
double RunningLinearFit::GetMaxDifference(){ return maxDifference; }
