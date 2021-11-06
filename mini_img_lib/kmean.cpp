#include "./mini_img_lib.h"
#include <mutex>

std::vector<std::vector<int> > tmp_clustered_image;

/**
 * finds nearest cluster center for pixels in rows [x1, x2)
 * 
 * computes distances using euclidean distance
 * 
 * @param x1 startig row (included)
 * @param x2 finishig row (not included)
 */
void _find_clusters_row(const int &x1, const int &x2)
{
    for (int x = x1; x < x2; ++x)
    {
        for (int y = 0; y < W; ++y)
        {
            // pick nearest cluster center
            int c_min = -1;
            double d_min = 1e18;

            for (int c = 0; c < NUMBER_OF_CLUSTERS; ++c)
            {
                // compute distance between cluster center and pixel
                float d = (image[x][y][0] - clusters[c][0]) * (image[x][y][0] - clusters[c][0]) +
                          (image[x][y][1] - clusters[c][1]) * (image[x][y][1] - clusters[c][1]) +
                          (image[x][y][2] - clusters[c][2]) * (image[x][y][2] - clusters[c][2]);

                if (d < d_min)
                {
                    d_min = d;
                    c_min = c;
                }
            }

            tmp_clustered_image[x][y] = c_min;
        }
    }
}

void kmeans_one_iteration()
{
    parallel_row_operation(_find_clusters_row);

    // sums up points for each cluster
    for (int x = 0; x < H; ++x)
        for (int y = 0; y < W; ++y)
        {
            int c = tmp_clustered_image[x][y];
            _cache_1_c[c][0] += image[x][y][0];
            _cache_1_c[c][1] += image[x][y][1];
            _cache_1_c[c][2] += image[x][y][2];
            ++_cache_1_c[c][3];
        }

    // update cluster centers by taking average
    for (int c = 0; c < NUMBER_OF_CLUSTERS; ++c)
    {
        clusters[c][0] = _cache_1_c[c][0] / _cache_1_c[c][3];
        clusters[c][1] = _cache_1_c[c][1] / _cache_1_c[c][3];
        clusters[c][2] = _cache_1_c[c][2] / _cache_1_c[c][3];
    }
}

// evaluates cluster
double _total_dist()
{
    double res = 0;
    for (int x = 0; x < H; ++x)
        for (int y = 0; y < W; ++y)
        {
            int c = tmp_clustered_image[x][y];
            res += (image[x][y][0] - clusters[c][0]) * (image[x][y][0] - clusters[c][0]) +
                   (image[x][y][1] - clusters[c][1]) * (image[x][y][1] - clusters[c][1]) +
                   (image[x][y][2] - clusters[c][2]) * (image[x][y][2] - clusters[c][2]);
        }
    return res;
}

/**
 * k-mean algorithm
 * 
 * takes the best clustering out 
 *  of 6 random initial points
 * 
 * 
 * @param number_of_clusters 
 * @param iterations number of times to run
 */
void kmeans(int number_of_clusters, int iterations)
{
    NUMBER_OF_CLUSTERS = number_of_clusters;
    clusters = _cache_1_c = std::vector<std::vector<float> >(NUMBER_OF_CLUSTERS, std::vector<float>(4, 0));
    tmp_clustered_image = clustered_image = std::vector<std::vector<int> >(H, std::vector<int>(W, 0));

    srand(time(NULL));

    double min_d = 1e18;

    for (int t = 0; t < 6; ++t)
    {
        // pick random points
        for (int c = 0; c < NUMBER_OF_CLUSTERS; ++c)
        {
            int r_x = rand() % H, r_y = rand() % W;
            clusters[c][0] = image[r_x][r_y][0];
            clusters[c][1] = image[r_x][r_y][1];
            clusters[c][2] = image[r_x][r_y][2];
        }

        // run k-means u times
        int u = iterations;
        while (u--)
        {
            _cache_1_c = std::vector<std::vector<float> >(NUMBER_OF_CLUSTERS, std::vector<float>(4, 0));
            kmeans_one_iteration();
        }
        double d = _total_dist();
        if (d < min_d)
        {
            d = min_d;
            clustered_image = tmp_clustered_image;
        }
    }
}