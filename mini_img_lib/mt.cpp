#include "./mini_img_lib.h"
#include <thread>

using namespace std;

void parallel_row_operation(void (*row_operation)(const int &, const int &)) {
    // height of each partition
    const int D = H / NUMBER_OF_THREADS; 
    vector<thread> threads;  
    
    // do row_operations on [x, x+D) for x = 0, D, 2D, ...
    for (int x = 0; x+D <= H; x += D)
        threads.push_back(thread(row_operation, x, x+D)); 
    
    // do row_operation on the remaining rows
    if (D * NUMBER_OF_THREADS != H) 
        threads.push_back(thread(row_operation, D * NUMBER_OF_THREADS, H));

    // wait until each thread is terminated
    for (auto &t: threads) 
       t.join();
}

void parallel_column_operation(void (*column_operation)(const int &, const int &)) {
    // width of each partition
    const int D = W / NUMBER_OF_THREADS; 
    vector<thread> threads;
    
    // do column_operations on [y, y+D) for y = 0, D, 2D, ...
    for (int y = 0; y+D <= W; y += D)
        threads.push_back(thread(column_operation, y, y+D)); 
    
    // do column_operation on the remaining columns
    if (D * NUMBER_OF_THREADS != W) 
        threads.push_back(thread(column_operation, D * NUMBER_OF_THREADS, W));
    
    // wait until each thread is terminated
    for (auto &t: threads) 
        t.join();
}