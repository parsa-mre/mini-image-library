#include "./mini_img_lib.h"
#include <math.h>

/**
 * Detects edges for rows in [x1, x2)
 * 
 * uses sobel's filters
 * @see https://en.wikipedia.org/wiki/Sobel_operator
 * 
 * @param x1 startig row (included)
 * @param x2 finishig row (not included)
 */
void _detect_edge_rows(const int &x1, const int &x2)
{
    for (int x = x1; x < x2; ++x)
        for (int y = 0; y < W; ++y)
        {
            for (int i = -k; i <= k; ++i)
                for (int j = -k; j <= k; ++j)
                {
                    // apply 1st filter and save to edge_image
                    edge_image[x][y] += sobel_1[i + k][j + k] *
                                        gray_image[_fixed_index(i + x, H)][_fixed_index(j + y, W)];
                    // apply 2nd filter and save to cache
                    _cache_1_c[x][y] += sobel_2[i + k][j + k] *
                                        gray_image[_fixed_index(i + x, H)][_fixed_index(j + y, W)];
                }

            // edge_image = sqrt(cache^2 + edge_image^2)
            edge_image[x][y] = sqrt(_cache_1_c[x][y] * _cache_1_c[x][y] +
                                    edge_image[x][y] * edge_image[x][y]);
            // if value is less than 50, set it to zero
            edge_image[x][y] = edge_image[x][y] < 70 ? 0 : edge_image[x][y];
            edge_image[x][y] = edge_image[x][y] > 255 ? 255 : edge_image[x][y];
        }
}

/**
 * Detects edges
 * 
 * uses sobel's filters
 * @see https://en.wikipedia.org/wiki/Sobel_operator
 *
 * saves result into edge_image 
 */

void edge_detection()
{
    // k = filter_size/2  ->  3/2 = 1
    k = 1;
    // performs gray scaling on image and saves the result in gray_image
    gray();
    // initializing edge_image and cache
    edge_image = _cache_1_c = std::vector<std::vector<float> >(H, std::vector<float>(W, 0));
    // automatically allocates rows to threads for edge detection
    parallel_row_operation(_detect_edge_rows);
}
