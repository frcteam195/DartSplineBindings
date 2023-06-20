#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    double x;
    double y;
    double heading;
} waypoint_t;

void waypoint_test(waypoint_t wp);
void print_num(int x);

#ifdef __cplusplus
}
#endif
