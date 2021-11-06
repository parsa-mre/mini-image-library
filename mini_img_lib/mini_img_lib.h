/**
 * MINI-IMG-LIB
 * 
 * a mini multi-threaded image library 
 *  with the followig functionalities:
 *      1. gray-scale
 *      2. edge-detection
 *      3. blur
 *      4. k-mean clustering
 * 
 * multi-threading project
 *  for Operating Systems course 
 */

#include <iostream>
#include <vector>

// number of threads
extern int NUMBER_OF_THREADS;
// height, width, kernel_size/2
extern int H, W, k;
// standard deviation for gaussian blur
extern float sigma;
// number of clusters for kmean algorithm
extern int NUMBER_OF_CLUSTERS;

// edge detection kernels
const float sobel_1[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const float sobel_2[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

// one dimentional gaussian kernel
extern std::vector<float> blur_kernel;

// three channels
extern std::vector<std::vector<std::vector<float> > > image, blured_image, _cache_3_c;
// two channels
extern std::vector<std::vector<float> > gray_image, edge_image, _cache_1_c, clusters; 
extern std::vector<std::vector<int> > clustered_image, tmp_clustered_image;

// multi-threading part
void parallel_row_operation(void (*row_operation)(const int &, const int &));
void parallel_column_operation(void (*column_operation)(const int &, const int &));

// implemented algorithms
void gray();
void edge_detection();
void blur(int iterations, const int &filter_size, const float &std);
void kmeans(int number_of_clusters, int iterations);

// fixes out of boud indices for symetric padding
inline int _fixed_index(const int &index, const int &N)
{
    return index - N >= 0 ? abs((N << 1) - index - 1) : abs(index);
}
