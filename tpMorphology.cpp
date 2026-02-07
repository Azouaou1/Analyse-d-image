#include "tpMorphology.h"
#include <cmath>
#include <algorithm>
#include <tuple>
#include <limits>
#include "common.h"
using namespace cv;
using namespace std;


/**
    Compute a median filter of the input float image.
    The filter window is a square of (2*size+1)*(2*size+1) pixels.

    Values outside image domain are ignored.

    The median of a list l of n>2 elements is defined as:
     - l[n/2] if n is odd 
     - (l[n/2-1]+l[n/2])/2 is n is even 
*/
Mat median(Mat image, int size)
{
    Mat res = image.clone();
    assert(size>0);
    /********************************************
                YOUR CODE HERE
    *********************************************/
    vector<float> list_elements;
    int taille_list = 0;

    for (int y = 0; y < res.rows; y++){
        for (int x = 0 ; x < res.cols; x++){

            list_elements.clear();

            for (int t = y-size/2; t <= y+size/2; t++)
                for (int z = x-size/2; z <= x+size/2; z++){
                    if (t >= 0 && t <= res.rows && z >= 0 && z <= res.cols){
                        list_elements.push_back(image.at<float>(t, z));
                    }
                }
            sort(list_elements.begin(), list_elements.end());
            taille_list = list_elements.size();

            if ((taille_list % 2) == 0)
                res.at<float>(y, x) = float(list_elements[(taille_list/2)-1] + list_elements[(taille_list/2)]) / 2.0f;
            else
                res.at<float>(y, x) = float(list_elements[(taille_list/2)]);
        }
    }

    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}


/**
    Compute the dilation of the input float image by the given structuring element.
     Pixel outside the image are supposed to have value 0
*/


Mat dilate(Mat image, Mat structuringElement)
{
    Mat res = Mat::zeros(image.rows,image.cols,CV_32FC1);
    /********************************************
                YOUR CODE HERE
    *********************************************/
    int structureSur2Y = structuringElement.rows/2;
    int structureSur2X = structuringElement.cols/2;
    float maxVal;

    for (int y = 0; y< image.rows; y++){
        for (int x = 0; x< image.cols; x++){
            
            maxVal = 0.0f;
            for(int t = -structureSur2Y; t <= structureSur2Y; t++){
                for (int z = -structureSur2X; z <= structureSur2X; z++){
                    if (y+t >= 0 && y+t < image.rows && x+z >=0 && x+z < image.cols && structuringElement.at<float>(t+structureSur2Y, z+structureSur2X) >= 0.5f){
                        maxVal = max(maxVal, image.at<float>(y+t, x+z));

                    }
                }
            }
            res.at<float>(y,x) = maxVal;
        }

    }
    /********************************************

                END OF YOUR CODE
    *********************************************/
    
    return res;
    
}





/**
    Compute the erosion of the input float image by the given structuring element.
    Pixel outside the image are supposed to have value 1.
*/
Mat erode(Mat image, Mat structuringElement)
{
    Mat res = image.clone();
    /********************************************
                YOUR CODE HERE
        hint : 1 line of code is enough
    *********************************************/
    int structureSur2Y = structuringElement.rows/2;
    int structureSur2X = structuringElement.cols/2;
    float minVal;

    for (int y = 0; y< image.rows; y++){
        for (int x = 0; x< image.cols; x++){
            
            minVal = 1.0f;
            for(int t = -structureSur2Y; t <= structureSur2Y; t++){
                for (int z = -structureSur2X; z <= structureSur2X; z++){
                    if (y+t >= 0 && y+t < image.rows && x+z >=0 && x+z < image.cols && structuringElement.at<float>(t+structureSur2Y, z+structureSur2X) >= 0.5f){
                        minVal = min(minVal, image.at<float>(y+t, x+z));

                    }
                }
            }
            res.at<float>(y,x) = minVal;
        }

    }
    //res.convertTo(res, CV_8UC1);
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}


/**
    Compute the opening of the input float image by the given structuring element.
*/
Mat open(Mat image, Mat structuringElement)
{

    Mat res = Mat::zeros(image.rows,image.cols,CV_32FC1);
    /********************************************
                YOUR CODE HERE
        hint : 1 line of code is enough
    *********************************************/
    res = erode(image, structuringElement);
    res = dilate(res, structuringElement);
    
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}


/**
    Compute the closing of the input float image by the given structuring element.
*/
Mat close(Mat image, Mat structuringElement)
{

    Mat res = Mat::zeros(image.rows,image.cols,CV_32FC1);
    /********************************************
                YOUR CODE HERE
        hint : 1 line of code is enough
    *********************************************/
    res = dilate(image, structuringElement);
    res = erode(res, structuringElement);
    
    
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}


/**
    Compute the morphological gradient of the input float image by the given structuring element.
*/
Mat morphologicalGradient(Mat image, Mat structuringElement)
{

    Mat res = Mat::zeros(image.rows,image.cols,CV_32FC1);
    /********************************************
                YOUR CODE HERE
        hint : 1 line of code is enough
    *********************************************/

    res = dilate(image, structuringElement) - erode(image, structuringElement);
    
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}

