#include "stats.h"

stats::stats(PNG & im){

    // resize all the vectors to match the width and height of the image
    sumRed.resize(im.width(), vector<long>(im.height()));
    sumGreen.resize(im.width(), vector<long>(im.height()));
    sumBlue.resize(im.width(), vector<long>(im.height()));

    sumsqRed.resize(im.width(), vector<long>(im.height()));
    sumsqGreen.resize(im.width(), vector<long>(im.height()));
    sumsqBlue.resize(im.width(), vector<long>(im.height()));

    // for every pixel in img
    for (unsigned int x = 0; x < im.width(); x++) {
        // summed up values of all the channels
        long redValues = 0;
        long greenValues = 0;
        long blueValues = 0;

        long sqredValues = 0;
        long sqgreenValues = 0;
        long sqblueValues = 0;

        for (unsigned int y = 0; y < im.height(); y++) {
            RGBAPixel *pixel = im.getPixel(x, y);

            if (x == 0) {
                redValues += pixel->r;
                sumRed[x][y] = redValues;
                greenValues += pixel->g;
                sumGreen[x][y] = greenValues;
                blueValues += pixel->b;
                sumBlue[x][y] = blueValues;

                sqredValues += pow(pixel->r, 2);
                sumsqRed[x][y] = sqredValues;
                sqgreenValues += pow(pixel->g, 2);
                sumsqGreen[x][y] = sqgreenValues;
                sqblueValues += pow(pixel->b, 2);
                sumsqBlue[x][y] = sqblueValues;
            }
            else {
                redValues += pixel->r;
                sumRed[x][y] = sumRed[x - 1][y] + redValues;
                
                greenValues += pixel->g;
                sumGreen[x][y] = sumGreen[x - 1][y] + greenValues;
                
                blueValues += pixel->b;
                sumBlue[x][y] = sumBlue[x - 1][y] + blueValues;

                // square value
                sqredValues += pow(pixel->r, 2);
                sumsqRed[x][y] = sumsqRed[x - 1][y] + sqredValues;
                
                sqgreenValues += pow(pixel->g, 2);
                sumsqGreen[x][y] = sumsqGreen[x - 1][y] + sqgreenValues;
                
                sqblueValues += pow(pixel->b, 2);
                sumsqBlue[x][y] = sumsqBlue[x - 1][y] + sqblueValues;
            }
        }
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
    // check what channel to get  
    if (channel == 'r')
    {
        if (ul.first == 0 && ul.second == 0) {
            return sumRed[w-1][h-1];
        }
        else if (ul.first == 0) {
            return sumRed[w-1][(h-1) + ul.second] - sumRed[w-1][ul.second-1];
        }
        else if (ul.second == 0) {
            return sumRed[(w-1)+ul.first][h-1] - sumRed[ul.first-1][h-1];
        }
        else {
            return sumRed[ul.first+(w-1)][ul.second+(h-1)] - sumRed[ul.first+(w-1)][ul.second-1] - sumRed[ul.first-1][ul.second+(h-1)] + sumRed[ul.first-1][ul.second-1];
        }
    }
    if (channel == 'g') 
    {
        if (ul.first == 0 && ul.second == 0) {
            return sumGreen[w-1][h-1];
        }
        else if (ul.first == 0) {
            return sumGreen[w-1][(h-1) + ul.second] - sumGreen[w-1][ul.second-1];
        }
        else if (ul.second == 0) {
            return sumGreen[(w-1)+ul.first][h-1] - sumGreen[ul.first-1][h-1];
        }
        else {
            return sumGreen[ul.first+(w-1)][ul.second+(h-1)] - sumGreen[ul.first+(w-1)][ul.second-1] - sumGreen[ul.first-1][ul.second+(h-1)] + sumGreen[ul.first-1][ul.second-1];
        }
    } else {
        if (ul.first == 0 && ul.second == 0) {
            return sumBlue[w-1][h-1];
        }
        else if (ul.first == 0) {
            return sumBlue[w-1][(h-1) + ul.second] - sumBlue[w-1][ul.second-1];
        }
        else if (ul.second == 0) {
            return sumBlue[(w-1)+ul.first][h-1] - sumBlue[ul.first-1][h-1];
        }
        else {
            return sumBlue[ul.first+(w-1)][ul.second+(h-1)] - sumBlue[ul.first+(w-1)][ul.second-1] - sumBlue[ul.first-1][ul.second+(h-1)] + sumBlue[ul.first-1][ul.second-1];
        }
    }
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    if (channel == 'r')
    {
        if (ul.first == 0 && ul.second == 0) {
            return sumsqRed[w-1][h-1];
        }
        else if (ul.first == 0) {
            return sumsqRed[w-1][(h-1) + ul.second] - sumsqRed[w-1][ul.second-1];
        }
        else if (ul.second == 0) {
            return sumsqRed[(w-1)+ul.first][h-1] - sumsqRed[ul.first-1][h-1];
        }
        else {
            return sumsqRed[ul.first+(w-1)][ul.second+(h-1)] - sumsqRed[ul.first+(w-1)][ul.second-1] - sumsqRed[ul.first-1][ul.second+(h-1)] + sumsqRed[ul.first-1][ul.second-1];
        }
    }
    if (channel == 'g')
    {
        if (ul.first == 0 && ul.second == 0) {
            return sumsqGreen[w-1][h-1];
        }
        else if (ul.first == 0) {
            return sumsqGreen[w-1][(h-1) + ul.second] - sumsqGreen[w-1][ul.second-1];
        }
        else if (ul.second == 0) {
            return sumsqGreen[(w-1)+ul.first][h-1] - sumsqGreen[ul.first-1][h-1];
        }
        else {
            return sumsqGreen[ul.first+(w-1)][ul.second+(h-1)] - sumsqGreen[ul.first+(w-1)][ul.second-1] - sumsqGreen[ul.first-1][ul.second+(h-1)] + sumsqGreen[ul.first-1][ul.second-1];
        }
    } else {
        if (ul.first == 0 && ul.second == 0) {
            return sumsqBlue[w-1][h-1];
        }
        else if (ul.first == 0) {
            return sumsqBlue[w-1][(h-1) + ul.second] - sumsqBlue[w-1][ul.second-1];
        }
        else if (ul.second == 0) {
            return sumsqBlue[(w-1)+ul.first][h-1] - sumsqBlue[ul.first-1][h-1];
        }
        else {
            return sumsqBlue[ul.first+(w-1)][ul.second+(h-1)] - sumsqBlue[ul.first+(w-1)][ul.second-1] - sumsqBlue[ul.first-1][ul.second+(h-1)] + sumsqBlue[ul.first-1][ul.second-1];
        }
    }
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    double redAvg = getSumSq('r', ul, w, h) - (pow(getSum('r', ul, w, h), 2)/(w*h));
    double greenAvg = getSumSq('g', ul, w, h) - (pow(getSum('g', ul, w, h), 2)/(w*h));
    double blueAvg = getSumSq('b', ul, w, h) - (pow(getSum('b', ul, w, h), 2)/(w*h));

    return redAvg + blueAvg + greenAvg;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){

    long avgRed = getSum('r', ul, w, h) / (w * h);
    long avgGreen = getSum('g', ul, w, h) / (w * h);
    long avgBlue = getSum('b', ul, w, h) / (w * h);

    RGBAPixel avgPixel = RGBAPixel(avgRed, avgGreen, avgBlue);
    
    return avgPixel;
}
