#include <cmath>
#include <thread>
#include <future>
#include <iostream>
#include "mandelbrot.h"
#include "drawing.h"

using namespace std;

Mandelbrot::Mandelbrot(double xmin, double xmax,
                       double ymin, double ymax,
                       int height, int width)
{
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;
    this->height = height;
    this->width = width;
}

void Mandelbrot::drawApp()
{
    for (int i=0; i<points.size(); i++)
    {
        drawPoint(points[i].x, points[i].y,
                  points[i].r, points[i].g, points[i].b);
    }
    refreshed = false;
}

void Mandelbrot::calculateApp()
{
    points.clear();
    vector<Point> pointsToCalc;
    double deltaX = ((xmax - xmin) / (double)width);
    double deltaY = ((ymax - ymin) / (double)height);
    double x = xmin;
    while (x <= xmax)
    {
        double y = ymin;
        while (y <= ymax)
        {
            pointsToCalc.push_back(Point(x, y));
            y += deltaY;
        }
        x += deltaX;
    }
    vector<thread> threads;
    vector<future<vector<Point>>> futures;
    for (int block=0; block<WORKER_THREADS; block++)
    {
        int startRange = (int)((pointsToCalc.size() / WORKER_THREADS)) * block;
        int endRange = (int)((pointsToCalc.size() / WORKER_THREADS)) * (block+1);
        vector<Point> subset;
        for (int i=startRange; i<endRange; i++)
        {
            subset.push_back(pointsToCalc[i]);
        }

        promise<vector<Point>> prom;
        futures.push_back(prom.get_future());
        threads.push_back(thread(calcMandelbrotWorker, subset, move(prom)));
    }
    for (int i=0; i<threads.size(); i++)
    {
        threads[i].join();
        vector<Point> results = futures[i].get();
        points.insert(points.end(), results.begin(), results.end());
    }
    refreshed = true;
}

bool Mandelbrot::calcMandelbrotPoint(Point &point)
{
    double prevX = point.x;
    double prevY = point.y;
    int count;
    for (count = 0; count < FRACTAL_ITERATIONS; count++ )
    {
        double x = (prevX * prevX) - (prevY * prevY) + point.x;
        double y = (2 * (prevX * prevY)) + point.y;
        double dist = sqrt(x*x + y*y);
        prevX = x;
        prevY = y;
        if (dist > FRACTAL_ESCAPE)
        {
            break;
        }
    }
    if (count > 0 && count < FRACTAL_ITERATIONS)
    {
        point.r = min(255, count * 10);
        point.g = count;
        point.b = count;
        return true;
    } 
    return false;    
}

void Mandelbrot::calcMandelbrotWorker(vector<Point> subset, promise<vector<Point>> prom)
{
    vector<Point> results;
    for (int i=0; i<subset.size(); i++)
    {
        if (calcMandelbrotPoint(subset[i]))
        {
            results.push_back(subset[i]);
        }
    }
    prom.set_value(results);
}

void Mandelbrot::zoom(int x, int y, double ratio)
{
    double virtualGridXSize = ((xmax - xmin)/2.0) * ratio;
    double virtualGridYSize = ((ymax - ymin)/2.0) * ratio;
    double virtualX = (((x / (double)width)) * (xmax - xmin)) + xmin;
    double virtualY = (((y / (double)height)) * (ymax - ymin)) + ymin;
    xmin = virtualX - virtualGridXSize;
    xmax = virtualX + virtualGridXSize;
    ymin = virtualY - virtualGridYSize;
    ymax = virtualY + virtualGridYSize;
 }