#ifndef LIDAR_HEADER
#define LIDAR_HEADER


unsigned long int lidar(void);

unsigned long int lidarRun(void);

void Array_sort(long int *array , int n);

long int Find_median(long int array[] , int n);

void Init_TOF(void);

__interrupt void tovisr(void);

#endif