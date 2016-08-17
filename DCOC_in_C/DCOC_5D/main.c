
#include "value_iteration.h"
#include "globals.h"
#include "test_pendulum.h"



int main(int argc, const char * argv[]) {
    
    int status;
    
    init_value_iteration();

    status = value_iteration();
    test_pendulum();
    return 0;
}