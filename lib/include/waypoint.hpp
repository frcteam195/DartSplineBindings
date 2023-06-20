#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    double x;
    double y;
    double track;
    double heading;
    double curvature;
} waypoint_t;

typedef struct
{
    int size;
    waypoint_t *wp_ptr;
} waypoint_array_t;

waypoint_t *make_waypoints(int size);
waypoint_array_t get_waypoints();
void freeme(void *ptr);

waypoint_array_t calc_splines(waypoint_array_t waypoints);

#ifdef __cplusplus
}
#endif
