#pragma once
#include <vips/vips8>
using namespace vips;

#define FOLDER_PATH "I:\\Datasets\\LargeImages\\"
#define ymin(a,b) (((a) < (b)) ? (a) : (b))
/* How many pings we run at once.
*/
#define NUM_IN_PARALLEL (4)

void hello_world();
void cropping();
void cropping_thread();
void cpu_thread(const char* _in_file);