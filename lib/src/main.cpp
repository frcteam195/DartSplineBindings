#include <iostream>
#include "waypoint.hpp"

int main(int argc, char** argv)
{
    waypoint_array_t waypoints;
    waypoints.size = 2;
    waypoints.wp_ptr = make_waypoints(2);

    waypoints.wp_ptr[0] = waypoint_t{0.0, 0.0, 0.0, 0.0, 0.0};
    waypoints.wp_ptr[1] = waypoint_t{50.0, 0.0, 180.0, 0.0, 0.0};

    // waypoint_array_t wps = get_waypoints();

    waypoint_array_t splines_out = calc_splines(waypoints);

    return 0;
}
