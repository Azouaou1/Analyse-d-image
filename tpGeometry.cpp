#include "tpGeometry.h"
#include <cmath>
#include <algorithm>
#include <tuple>
using namespace cv;
using namespace std;

/**
    Transpose the input image,
    ie. performs a planar symmetry according to the
    first diagonal (upper left to lower right corner).
*/
Mat transpose(Mat image)
{
    Mat res = Mat::zeros(image.cols,image.rows,CV_32FC1);
    /********************************************
                YOUR CODE HERE
    hint: consider a non square image
    *********************************************/
    for (int y = 0; y < image.rows; y++){
        for (int x = 0; x < image.cols; x++){
            res.at<float>(x, y) = image.at<float>(y, x);            
        }
    }
   
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}

/**
    Compute the value of a nearest neighbour interpolation
    in image Mat at position (x,y)
*/
float interpolate_nearest(Mat image, float y, float x)
{
    float v=0;
    /********************************************
                YOUR CODE HERE
    *********************************************/

    // calcule des nouvelle position selon x et y en trouvons le plus proche voisin
    v = image.at<float>(int(y + 0.5), int(x + 0.5));

    /********************************************
                END OF YOUR CODE
    *********************************************/
    return v;

}


/**
    Compute the value of a bilinear interpolation in image Mat at position (x,y)
*/
float interpolate_bilinear(Mat image, float y, float x)
{
    float v=0;
    /********************************************
                YOUR CODE HERE
    *********************************************/

    // Indices des 4 pixels voisins
    int i = int(floor(y));
    int j = int(floor(x));

    int i_1 = int(i+1);
    int j_1 = int(j+1);

    // czlcule des proportion des valeurs dans f(x2) et f(x1)
    float alpha = y - i;
    float beta = x - j;

    // extraction des 4 valeurs des voisins dans l'image
    float f_00 = image.at<float>(i, j);
    float f_01 = image.at<float>(i, j_1);
    float f_10 = image.at<float>(i_1, j);
    float f_11 = image.at<float>(i_1, j_1);

    // application de l'interpolation

    v = (1-alpha) * (1- beta) * f_00 + alpha * (1-beta) * f_10 + (1- alpha) * beta * f_01 + alpha * beta * f_11 ;
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return v;
}

/**
    Multiply the image resolution by a given factor using the given interpolation method.
    If the input size is (h,w) the output size shall be ((h-1)*factor, (w-1)*factor)
*/
Mat expand(Mat image, int factor, float(* interpolationFunction)(cv::Mat image, float y, float x))
{
    assert(factor>0);
    Mat res = Mat::zeros((image.rows-1)*factor,(image.cols-1)*factor,CV_32FC1);
    /********************************************
                YOUR CODE HERE
    *********************************************/
    for (int y = 0; y < res.rows; y++){
        for (int x = 0; x < res.cols; x++){
            res.at<float>(y, x) = interpolationFunction(image, y/float(factor), x/float(factor));
        }
    }
    /********************************************
                END OF YOUR CODE
    *********************************************/
    return res;
}

/**
    Performs a rotation of the input image with the given angle (clockwise) and the given interpolation method.
    The center of rotation is the center of the image.

    Ouput size depends of the input image size and the rotation angle.

    Output pixels that map outside the input image are set to 0.
*/


// dans l'implementation de cette methode j'ai pas reussis a avoir un bon resultat 
Mat rotate(Mat image, float angle, float(* interpolationFunction)(cv::Mat image, float y, float x))
{
    int new_image_bounderies = int(sqrt(image.rows * image.rows + image.cols * image.cols)+1);
    Mat res = Mat::zeros(new_image_bounderies,
                         new_image_bounderies,
                         CV_32FC1);

    // calcul du centre de l'image initiale
    float centre_y = image.rows / 2.0f;
    float centre_x = image.cols / 2.0f;

    // calcul du centre de la nouvelle image
    float centre_res_y = new_image_bounderies / 2.0f;
    float centre_res_x = new_image_bounderies / 2.0f;

    // boucle sur la nouvelle image
    for (int y = 0; y < new_image_bounderies; y++){
        for (int x = 0; x < new_image_bounderies; x++){
            
            // decaler les valeur de x et y sur l'image 
            float x_decaler = x - centre_res_x;
            float y_decaler = y - centre_res_y;

            // les nouvelle valeur apres rotation inverse
            float x_new =  cos(angle) * x_decaler + sin(angle) * y_decaler + centre_x;
            float y_new = -sin(angle) * x_decaler + cos(angle) * y_decaler + centre_y;

            // vérifier les limites de l'image et mettre des zeros ou il le faut
            if(x_new >= 0 && x_new < image.cols && y_new >= 0 && y_new < image.rows)
                res.at<float>(y, x) = interpolationFunction(image, y_new, x_new);
            else
                res.at<float>(y, x) = 0; 
        }
    }

    return res;
}

