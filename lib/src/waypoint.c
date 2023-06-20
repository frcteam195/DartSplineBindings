#include "waypoint.h"
#include <stdio.h>

void waypoint_test(waypoint_t wp)
{
    printf("X: %lf, Y: %lf, Heading: %lf\n", wp.x, wp.y, wp.heading);
}

void print_num(int x)
{
    printf("%d\n", x);
}
