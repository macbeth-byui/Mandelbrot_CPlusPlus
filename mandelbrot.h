#ifndef mandelbrot_h
#define mandelbrot_h

#include <vector>
#include <future>
#include "app.h"

#define FRACTAL_ITERATIONS 255
#define FRACTAL_ESCAPE 2
#define WORKER_THREADS 10

using namespace std;

struct Point
{
    double x;
    double y;
    int g;
    int r;
    int b;

    Point(double x, double y) : x(x), y(y), g(0), r(0), b(0) {}
};

class Mandelbrot : public App
{
    private:
        double xmin;
        double xmax;
        double ymin;
        double ymax;
        int height;
        int width;
        vector<Point> points;

    public:
        Mandelbrot(double xmin, double xmax,
                   double ymin, double ymax,
                   int height, int width);
        
        void drawApp();
        void calculateApp();
        void zoom(int x, int y, double ratio);
        double getXmin() { return xmin; }
        double getXmax() { return xmax; }
        double getYmin() { return ymin; }
        double getYmax() { return ymax; }

    private:
        static bool calcMandelbrotPoint(Point &point);
        static void calcMandelbrotWorker(vector<Point> subset, 
                                         promise<vector<Point>> prom);
        
        

};

#endif