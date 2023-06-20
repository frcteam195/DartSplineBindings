#include "waypoint.hpp"
#include "ck_utilities/team254_geometry/Pose2d.hpp"
#include "ck_utilities/team254_geometry/Pose2dWithCurvature.hpp"
#include "ck_utilities/team254_geometry/Rotation2d.hpp"
#include "ck_utilities/team254_geometry/Translation2d.hpp"
#include "ck_utilities/team254_geometry/QuinticHermiteSpline.hpp"
#include "ck_utilities/team254_geometry/SplineGenerator.hpp"
#include <vector>
#include <stdio.h>

using namespace ck::team254_geometry;
using ck::trajectory::TrajectoryPoint;

waypoint_t *make_waypoints(int size)
{
    waypoint_t *wps = new waypoint_t[size];

    // printf("Allocating address: %p\n", wps);

    for (int i = 0; i < size; i++)
    {
        wps[i].x = i * 1.0;
        wps[i].y = i * 2.0;
        wps[i].track = i * 3.0;
        wps[i].heading = i * 4.0;
    }

    return wps;
}

waypoint_array_t get_waypoints()
{
    waypoint_array_t wp_arr;
    wp_arr.size = 15;
    wp_arr.wp_ptr = make_waypoints(wp_arr.size);
    return wp_arr;
}

void freeme(void *ptr)
{
    // printf("Freeing address: %p\n", ptr);
    free(ptr);
}

waypoint_array_t calc_splines(waypoint_array_t waypoints)
{
    std::vector<Pose2d> points;
    std::vector<Rotation2d> headings;

    for (int i = 0; i < waypoints.size; i++)
    {
        double x = waypoints.wp_ptr[i].x;
        double y = waypoints.wp_ptr[i].y;
        double track = waypoints.wp_ptr[i].track;
        double heading = waypoints.wp_ptr[i].heading;

        // std::cout << "X: " << x
        //           << ", Y: " << y
        //           << ", Heading: " << heading
        //           << std::endl;

        points.push_back(Pose2d(Translation2d(x, y), Rotation2d::fromDegrees(track)));
        headings.push_back(Rotation2d::fromDegrees(heading));
    }

    std::vector<QuinticHermiteSpline> mQunticHermiteSplines;
    std::vector<TrajectoryPoint<Pose2dWithCurvature, Rotation2d>> positions;

    if (points.size() < 2)
    {
        waypoint_array_t wp_arr;
        wp_arr.size = 0;
        wp_arr.wp_ptr = nullptr;
        return wp_arr;
    }
    else
    {
        for (int i = 0; i < points.size() - 1; i++)
        {
            mQunticHermiteSplines.push_back(QuinticHermiteSpline(points[i], points[i+1]));
        }

        QuinticHermiteSpline::optimizeSpline(mQunticHermiteSplines);

        positions = SplineGenerator::parameterizeSplines(mQunticHermiteSplines, headings);
    }

    waypoint_array_t wp_arr;
    wp_arr.size = positions.size();
    wp_arr.wp_ptr = new waypoint_t[wp_arr.size];

    for (int i = 0; i < wp_arr.size; i++)
    {
        wp_arr.wp_ptr[i].x = positions[i].state_.getTranslation().x();
        wp_arr.wp_ptr[i].y = positions[i].state_.getTranslation().y();
        wp_arr.wp_ptr[i].track = positions[i].state_.getRotation().getDegrees();
        wp_arr.wp_ptr[i].heading = positions[i].heading_.getRotation().getDegrees();
        wp_arr.wp_ptr[i].curvature = positions[i].state_.getCurvature();
    }

    return wp_arr;
}
