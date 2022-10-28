#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "semaphore.h"

#include "win32_timing.h"

#define ThreadCount 12 
#include "mutex_example.cpp"
#include "producer_consumer_example.cpp"

int main(void)
{
    MutexNonThreadedExample();
    MutexThreadedExample();
    ProducerConsumerExample();

    return(0);
}

