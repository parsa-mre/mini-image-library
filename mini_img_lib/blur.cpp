#include "./mini_img_lib.h"
#include <math.h>

// standard deviation for gaussian blur
float sigma;

// returns gaussian(i)
inline float _gaussian_1d(const int &i)
{
    return exp(-(i * i * 1.0) / (2 * (sigma * sigma))) / (sqrt(M_PI * 2) * sigma);
}

// initializes 1D gaussian kerel for blur
void _init_blur_kernel()
{
    blur_kernel = std::vector<float>(2 * k + 1, 0);
    // sum
    float s_ = 0;
    for (int i = -k; i <= k; ++i)
    {
        blur_kernel[i + k] = _gaussian_1d(i);
        s_ += blur_kernel[i + k];
    }
    // devide by the sum to get mean of 1
    for (int i = 0; i < blur_kernel.size(); ++i)
        blur_kernel[i] /= s_;
}

/**
 * Gaussian Blur
 * 
 * uses a 1D kernel; applied to each 
 *  row in [x1, x2) a single iteration
 * 
 * blured_image -> cache
 * 
 * @param x1 startig row (included)
 * @param x2 finishig row (not included)
 */
void _blur_rows(const int &x1, const int &x2)
{
    for (int x = x1; x < x2; ++x)
    {
        for (int y = 0; y < W; ++y)
        {
            _cache_3_c[x][y][0] = _cache_3_c[x][y][1] = _cache_3_c[x][y][2] = 0;
            for (int l = -k; l <= k; ++l)
            {
                float p = blur_kernel[l + k];
                _cache_3_c[x][y][0] += p * blured_image[x][_fixed_index(y + l, W)][0];
                _cache_3_c[x][y][1] += p * blured_image[x][_fixed_index(y + l, W)][1];
                _cache_3_c[x][y][2] += p * blured_image[x][_fixed_index(y + l, W)][2];
            }
        }
    }
}

/**
 * Gaussian Blur
 * 
 * uses a 1D kernel; applied to each 
 *  column in [y1, y2) a single iteration
 * 
 * cache -> blured_image
 * 
 * @param y1 startig column (included)
 * @param y2 finishig column (not included)
 */
void _blur_columns(const int &y1, const int &y2)
{
    for (int y = y1; y < y2; ++y)
    {
        for (int x = 0; x < H; ++x)
        {
            blured_image[x][y][0] = blured_image[x][y][1] = blured_image[x][y][2] = 0;
            for (int l = -k; l <= k; ++l)
            {
                float p = blur_kernel[l + k];
                blured_image[x][y][0] += p * _cache_3_c[_fixed_index(x + l, H)][y][0];
                blured_image[x][y][1] += p * _cache_3_c[_fixed_index(x + l, H)][y][1];
                blured_image[x][y][2] += p * _cache_3_c[_fixed_index(x + l, H)][y][2];
            }
            
        }
    }
}

/**
 * Gaussian Blur
 * 
 * uses a 1D kernel; applied to each 
 *  row and column for a single iteration
 * 
 * saves result in blur_image
 * 
 * @param std  standard deviation
 * @param filter_size gaussian kerel filter size
 * @param iterations number of time gaussian kernel is applied
 */
void blur(int iterations, const int &filter_size, const float &std)
{
    blured_image = _cache_3_c = image;
    k = filter_size / 2;
    sigma = std;

    // initialize gaussian kernel
    _init_blur_kernel();
    while (iterations--)
    {
        // kernel applied horizontally
        parallel_row_operation(_blur_rows);
        // kernel applied vertically
        parallel_column_operation(_blur_columns);
    }
}
