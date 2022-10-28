#ifndef WIN32_TIMINT_H

static LARGE_INTEGER
Win32GetPerformanceCounter()
{
    LARGE_INTEGER Result = {0};
    QueryPerformanceCounter(&Result);
    return(Result);
}

static LARGE_INTEGER
Win32GetPerformanceFrequency()
{
    static LARGE_INTEGER Result = {0};
    QueryPerformanceFrequency(&Result);
    return(Result);
}

struct timing_info
{
    int64_t Cycles;
    float Seconds;
};

static struct timing_info
Win32GetTimingInformation(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    struct timing_info Result = {0};
    LARGE_INTEGER Frequency = Win32GetPerformanceFrequency();
    Result.Cycles = (int64_t)(End.QuadPart - Start.QuadPart);
    Result.Seconds = (float)((float)Result.Cycles / (float)(Frequency.QuadPart));
    return(Result);
}

#define WIN32_TIMING_H
#endif
