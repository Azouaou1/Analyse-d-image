
#include "tpConvolution.h"
#include <cmath>
#include <algorithm>
#include <tuple>
using namespace cv;
using namespace std;
/**
    Compute a mean filter of size 2k+1.

    Pixel values outside of the image domain are supposed to have a zero value.
*/

/********************************************
                YOUR CODE HERE
*********************************************/

// est un cas special de la convolution

cv::Mat meanFilter(cv::Mat image, int k){
    
    Mat res = image.clone();
    
    Mat kernel(2*k+1, 2*k+1, CV_32F, 1.0f/((2*k+1)*(2*k+1)));

    int taille_rows = image.rows;
    int taille_cols = image.cols;

    int ker_rows = kernel.rows;
    int ker_cols = kernel.cols;


    for (int y = 0; y < taille_rows; y++) {
        for (int x = 0; x < taille_cols; x++) {


            float sum = 0.0f;

            // parcours du voisinage pour prendre les valeurs qui sont seulement dans l'imaage
            for (int t = -ker_rows / 2; t < ker_rows / 2 + 1; t++) {
                for (int z = -ker_cols / 2; z < ker_cols / 2 + 1; z++) {

                    int indice_y = y + t;
                    int indice_x = x + z;

                    // nouvelle indice du kernel
                    int t_ker = t + ker_rows/2;
                    int z_ker = z + ker_cols/2;

                    // pixels valides uniquement
                    if (indice_y >= 0 && indice_y < taille_rows && indice_x >= 0 && indice_x < taille_cols) {

                        sum += image.at<float>(indice_y, indice_x) * kernel.at<float>(t_ker, z_ker);
                    }
                }
            }

            res.at<float>(y, x) = sum;
        }
    }
    return res;
    /********************************************
                END OF YOUR CODE
    *********************************************/

}

/**
    Compute the convolution of a float image by kernel.
    Result has the same size as image.
    
    Pixel values outside of the image domain are supposed to have a zero value.
*/
Mat convolution(Mat image, cv::Mat kernel)
{
    
    Mat res = image.clone();
    /********************************************
                YOUR CODE HERE
    *********************************************/

    int taille_rows = image.rows;
    int taille_cols = image.cols;

    int ker_rows = kernel.rows;
    int ker_cols = kernel.cols;


    for (int y = 0; y < taille_rows; y++) {
        for (int x = 0; x < taille_cols; x++) {


            float sum = 0.0f;

            // parcours du voisinage pour prendre les valeurs qui sont seulement dans l'imaage
            for (int t = -ker_rows / 2; t < ker_rows / 2 + 1; t++) {
                for (int z = -ker_cols / 2; z < ker_cols / 2 + 1; z++) {

                    int indice_y = y + t;
                    int indice_x = x + z;

                    // nouvelle indice du kernel
                    int t_ker = t + ker_rows/2;
                    int z_ker = z + ker_cols/2;

                    // pixels valides uniquement
                    if (indice_y >= 0 && indice_y < taille_rows && indice_x >= 0 && indice_x < taille_cols) {

                        sum += image.at<float>(indice_y, indice_x) * kernel.at<float>(t_ker, z_ker);
                    }
                }
            }

            res.at<float>(y, x) = sum / ker_cols*ker_rows;
        }
    }
    return res;
    /********************************************
                END OF YOUR CODE
    *********************************************/


}

/**
    Compute the sum of absolute partial derivative according to Sobel's method
*/
cv::Mat edgeSobel(cv::Mat image)
{
    Mat res = image.clone();
    /********************************************
                YOUR CODE HERE
    *********************************************/
    // ajouter des bordure a l'image
    // creation du kernel 
    Mat kernel_x = (Mat_<float>(3,3) <<
                             1.0, 2.0, 1.0,
                             0.0, 0.0, 0.0,
                            -1.0, -2.0, -1.0
                    );
    
    Mat kernel_y = (Mat_<float>(3,3) <<
                            -1.0, 0.0, 1.0,
                            -2.0, 0.0, 2.0,
                            -1.0, 0.0, 1.0
                    );
    int taille_rows = image.rows;
    int taille_cols = image.cols;

    int ker_rows = kernel_y.rows;
    int ker_cols = kernel_y.cols;


    for (int y = 0; y < taille_rows; y++) {
        for (int x = 0; x < taille_cols; x++) {


            float sum_horiz = 0.0f;
            float sum_vert = 0.0f;

            // parcours du voisinage pour prendre les valeurs qui sont seulement dans l'imaage
            for (int t = -ker_rows / 2; t < ker_rows / 2 + 1; t++) {
                for (int z = -ker_cols / 2; z < ker_cols / 2 + 1; z++) {

                    int indice_y = y + t;
                    int indice_x = x + z;

                    // nouvelle indice du kernel
                    int t_ker = t + ker_rows/2;
                    int z_ker = z + ker_cols/2;

                    // pixels valides uniquement
                    if (indice_y >= 0 && indice_y < taille_rows && indice_x >= 0 && indice_x < taille_cols) {

                        sum_horiz += image.at<float>(indice_y, indice_x) * kernel_x.at<float>(t_ker, z_ker);
                        sum_vert += image.at<float>(indice_y, indice_x) * kernel_y.at<float>(t_ker, z_ker);

                    }
                }
            }
            float valeur_horiz = sum_horiz / float(ker_rows * ker_cols);
            float valeur_vert = sum_vert / float(ker_rows * ker_cols);
            res.at<float>(y, x) = float(sqrt(valeur_horiz*valeur_horiz + valeur_vert*valeur_vert)); 
        }
    }
    return res;

    /********************************************
                END OF YOUR CODE
    *********************************************/
    
}


/**
    Value of a centered gaussian of variance (scale) sigma at point x.
*/
float gaussian(float x, float sigma2)
{
    return 1.0/(2*M_PI*sigma2)*exp(-x*x/(2*sigma2));
}

/**
    Performs a bilateral filter with the given spatial smoothing kernel 
    and a intensity smoothing of scale sigma_r.

*/
cv::Mat bilateralFilter(cv::Mat image, cv::Mat kernel, float sigma_r)
{
    cv::Mat res = image.clone();

    int taille_rows = image.rows;
    int taille_cols = image.cols;

    int ker_rows = kernel.rows;
    int ker_cols = kernel.cols;


    for (int y = 0; y < taille_rows; y++) {
        for (int x = 0; x < taille_cols; x++) {


            float sum = 0.0f;
            float norm = 0.0f;

            // parcours du voisinage
            for (int t = -ker_rows / 2; t <= ker_rows / 2; t++) {
                for (int z = -ker_cols / 2; z <= ker_cols / 2; z++) {

                    int new_y = y + t;
                    int new_x = x + z;

                    // pixels valides uniquement
                    if (new_y >= 0 && new_y < taille_rows && new_x >= 0 && new_x < taille_cols) {

                        float iq = image.at<float>(new_y, new_x);

                        float gs = kernel.at<float>(t + ker_rows / 2,
                                                    z + ker_cols / 2);

                        float intensite_dif = iq - image.at<float>(y, x);
                        float gr = exp(-(intensite_dif * intensite_dif) /
                                            (2.0f * sigma_r * sigma_r));

                        float w = gs * gr;

                        sum  += w * iq;
                        norm += w;
                    }
                }
            }

            res.at<float>(y, x) = sum / norm;
        }
    }

    return res;
}

