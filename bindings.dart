import 'dart:ffi';
import 'dart:io' show Directory;

import 'package:ffi/ffi.dart';
import 'package:path/path.dart' as path;

final class Waypoint {
    double x = 0.0;
    double y = 0.0;
    double track = 0.0;
    double heading = 0.0;
    double curvature = 0.0;

    Waypoint(this.x, this.y, this.track, this.heading);

    @override
    String toString() {
        return 'X: $x, Y: $y, Track: $track, Heading: $heading';
    }
}

final class CWaypoint extends Struct {
    @Double()
    external double x;

    @Double()
    external double y;

    @Double()
    external double track;

    @Double()
    external double heading;

    @Double()
    external double curvature;
}

final class CWaypiointArray extends Struct {
    @Int()
    external int size;
    external Pointer<CWaypoint> wpPtr;
}

typedef CreateWaypointArr = CWaypiointArray Function();
typedef CalcSplines = CWaypiointArray Function(CWaypiointArray waypoints);

final class Bindings {
    late DynamicLibrary lib;
    late CalcSplines calcSplinesFunc;

    Bindings() {
        var libraryPath =
            path.join(Directory.current.path, 'lib', 'build', 'libck_pathcobbler_bindings.so');
        lib = DynamicLibrary.open(libraryPath);
        calcSplinesFunc = lib.lookupFunction<CalcSplines, CalcSplines>('calc_splines');
    }

    List<Waypoint> calc(List<Waypoint> waypoints) {
        Pointer<CWaypiointArray> inputArr = calloc<CWaypiointArray>();
        inputArr.ref.size = waypoints.length;
        inputArr.ref.wpPtr = malloc<CWaypoint>(waypoints.length);

        for (var i = 0; i < waypoints.length; i++) {
            inputArr.ref.wpPtr[i].x = waypoints[i].x;
            inputArr.ref.wpPtr[i].y = waypoints[i].y;
            inputArr.ref.wpPtr[i].track = waypoints[i].track;
            inputArr.ref.wpPtr[i].heading = waypoints[i].heading;
        }

        CWaypiointArray retArr = calcSplinesFunc(inputArr.ref);

        calloc.free(inputArr.ref.wpPtr);
        calloc.free(inputArr);

        List<Waypoint> retWaypoints = [];

        for (var i = 0; i < retArr.size; i++) {
            Waypoint wp = Waypoint(
                retArr.wpPtr[i].x,
                retArr.wpPtr[i].y,
                retArr.wpPtr[i].track,
                retArr.wpPtr[i].heading);

            retWaypoints.add(wp);
        }

        return retWaypoints;
    }
}

void main() {
    Bindings bindings = Bindings();

    var wps = [Waypoint(0, 0, 0, 0), Waypoint(50, 50, 0, 0)];

    var retWps = bindings.calc(wps);

    print(retWps.length);
    retWps.forEach(print);
}
