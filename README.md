# Semaphores

Custom implementation of semaphores using atomics. Uses \_mm\_pause() intrinsic
for spinning.

Currently only works with Windows.


## Building
From the root directory run `code\build.bat`.

## Examples

There are 3 example functions called from main.


* MutexNonThreadedExample
Runs the Threaded code in a single thread and prints timing information.
* MutexThreadedExample
Runs the Threaded code in multiple threads and prints timing information. Uses a
mutex derived from the semaphore library to guarantee thread safe access to the
global counter variable.
* ProducerConsumerExample
Uses the semaphore library to create a single producer thread and multiple
consumer threads. One mutex and 2 semaphores guarantee correct access to the
circular buffer.
