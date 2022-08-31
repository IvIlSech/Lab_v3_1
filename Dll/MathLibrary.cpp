#include "pch.h" 
#include "MathLibrary.h"
//#include <mkl.h>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <dos.h>

void GlF_vms(int n, float max, float min, int func_ind, int *time, double *func_res_arr)
{
    unsigned int start_time, end_time;
    float* args = new float[n];
    float* value_HA = new float[n];
    float* value_EP = new float[n];
    float* value_func = new float[n];
    for (int i = 0; i < n; i++)
    {
        args[i] = (max - min) / n * i + min;
    }
    switch (func_ind)
    {
    case 0:
        start_time = clock();
        //vmsLn(n, args, value_EP, VML_EP);
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_EP[i] = n * n;    //
        }                           //
        end_time = clock();
        time[0] = (start_time - end_time) / CLK_TCK;

        start_time = clock();
        //vmsLn(n, args, value_HA, VML_HA);
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_HA[i] = n;        //
        }                           //
        end_time = clock();
        time[1] = (start_time - end_time) / CLK_TCK;
        break;
    case 1:
        start_time = clock();
        //vmsLGamma(n, args, value_EP, VML_EP);   не работающий mkl
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_EP[i] = n * n;    //
        }                           //
        end_time = clock();
        time[0] = (start_time - end_time) / CLK_TCK;
        start_time = clock();
        //vmsLGamma(n, args, value_HA, VML_HA);   не работающий mkl
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_HA[i] = n;        //
        }                           //
        end_time = clock();
        time[1] = (start_time - end_time) / CLK_TCK;
        break;
    }
    double max_sub = -1;
    for (int i = 0; i < n; i++) {
        if (abs(value_HA[i] - value_EP[i]) > max_sub) {
            func_res_arr[0] = abs(value_HA[i] - value_EP[i]);
            func_res_arr[1] = args[i];
            func_res_arr[2] = value_HA[i];
            func_res_arr[3] = value_EP[i];
        }
    }
    delete[] args;
    delete[] value_func;
    delete[] value_EP;
    delete[] value_HA;
}

void GlF_vmd(int n, float max, float min, int func_ind, int *time, double *func_res_arr)
{
    unsigned int start_time, end_time;
    double* args = new double[n];
    for (int i = 0; i < n; i++)
    {
        args[i] = (max - min) / n * i + min;
    }
    double* value_HA = new double[n];
    double* value_EP = new double[n];
    double* value_func = new double[n];
    switch (func_ind)
    {
    case 2:
        start_time = clock();
        //vmdLn(n, args, value_EP, VML_EP);  не работающий mkl
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_EP[i] = n * n;    //
        }                           //
        end_time = clock();
        time[0] = (start_time - end_time) / CLK_TCK;

        start_time = clock();
        //vmdLn(n, args, value_HA, VML_HA);  не работающий mkl
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_HA[i] = n;        //
        }                           //
        end_time = clock();
        time[1] = (start_time - end_time) / CLK_TCK;
        break;
    case 3:
        start_time = clock();
        //vmdLGamma(n, args, value_EP, VML_EP);  не работающий mkl
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_EP[i] = args[i];  //
        }                           //
        end_time = clock();
        time[0] = (start_time - end_time) / CLK_TCK;

        start_time = clock();
        //vmdLGamma(n, args, value_HA, VML_HA);   не работающий mkl
        for (int i = 0; i < n; i++) //
        {                           // замена не работающего mkl
            value_HA[i] = args[i] * args[i];        //
        }
        end_time = clock();
        time[1] = (start_time - end_time)/ CLK_TCK;
        break;
    }
    double max_sub = -1;
    for (int i = 0; i < n; i++) {
        if (abs(value_HA[i] - value_EP[i]) > max_sub) {
            func_res_arr[0] = abs(value_HA[i] - value_EP[i]);
            func_res_arr[1] = args[i];
            func_res_arr[2] = value_HA[i];
            func_res_arr[3] = value_EP[i];
        }
    }
    delete[] args;
    delete[] value_func;
    delete[] value_EP;
    delete[] value_HA;
}


extern "C" _declspec(dllexport)
void GlF(int n, float max, float min, int func_ind, int time[], double func_res_arr[])
{
    if (func_ind < 2)
    {
        GlF_vms(n, max, min, func_ind, time, func_res_arr);
    }
    else
    {
        GlF_vmd(n, max, min, func_ind, time, func_res_arr);
    }
}