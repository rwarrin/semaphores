static volatile int32_t GlobalCounter = 0;
static struct semaphore GlobalCounterMutex = {0};

static int32_t CounterTarget = 10000000;

static DWORD
IncrementFunction(void *Data)
{
    int32_t Limit = (int32_t)Data;

    int32_t LocalCounter = 0;
    for(int i = 0; i < Limit; ++i)
    {
        ++LocalCounter;
    }

    semaphore_wait(&GlobalCounterMutex);
    GlobalCounter += LocalCounter;
    semaphore_notify(&GlobalCounterMutex);

    return(0);
}

static void
MutexThreadedExample()
{
    semaphore_init(&GlobalCounterMutex, 1);
    HANDLE ThreadHandles[ThreadCount] = {0};
    LARGE_INTEGER Start = Win32GetPerformanceCounter();
    for(int i = 0; i < ThreadCount; ++i)
    {
        ThreadHandles[i] = CreateThread(0, 0, IncrementFunction, (void *)CounterTarget, 0, 0);
    }
    WaitForMultipleObjects(ThreadCount, ThreadHandles, true, INFINITE);
    LARGE_INTEGER End = Win32GetPerformanceCounter();

    struct timing_info TimingInfo = Win32GetTimingInformation(Start, End);

    printf("Count: %d, Expected: %d\n", GlobalCounter, CounterTarget*ThreadCount);
    printf("Seconds: %0.2f (%0.5f ms), Cycles: %lld\n",
           TimingInfo.Seconds, TimingInfo.Seconds * 1000, TimingInfo.Cycles);
}

static void
MutexNonThreadedExample()
{
    int32_t Target = CounterTarget * ThreadCount;
    int32_t Counter = 0;
    LARGE_INTEGER Start = Win32GetPerformanceCounter();
    for(int i = 0; i < Target; ++i)
    {
        ++Counter;
    }
    LARGE_INTEGER End = Win32GetPerformanceCounter();

    struct timing_info TimingInfo = Win32GetTimingInformation(Start, End);

    printf("Count: %d, Expected: %d\n", Counter, Target);
    printf("Seconds: %0.2f (%0.5f ms), Cycles: %lld\n",
           TimingInfo.Seconds, TimingInfo.Seconds * 1000, TimingInfo.Cycles);
}
