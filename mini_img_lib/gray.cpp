#include "./mini_img_lib.h"

/**
 * Convert rows in [x1, x2) to gray-scale
 * 
 * computes grayscale image by formula:
 *      AVG = R * 0.0722 + G * 0.2126 + B * 0.7152
 * 
 * @param x1 startig row (included)
 * @param x2 finishig row (not included)
 */
void _gray_rows(const int &x1, const int &x2)
{
    for (int x = x1; x < x2; ++x)
        for (int y = 0; y < W; ++y) {
            gray_image[x][y] = image[x][y][0] * 0.2126 +
                               image[x][y][1] * 0.7152 +
                               image[x][y][2] * 0.0722;

            gray_image[x][y] = fmin(gray_image[x][y], 255);
            gray_image[x][y] = fmax(gray_image[x][y], 0);
        }

    
}

/**
 * Convert image to gray-scale
 * 
 * saves results to gray_image
 */
void gray()
{
    // initialize gray_image
    gray_image = std::vector<std::vector<float> >(H, std::vector<float>(W, 0));
    parallel_row_operation(_gray_rows);
}
