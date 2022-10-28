#ifndef SEMAPHORE_H

#include <windows.h>
#include <intrin.h>


struct semaphore
{
    volatile int32_t Count;
    volatile int32_t Limit;
};

inline void
semaphore_init(struct semaphore *Semaphore, int32_t Count)
{
    Semaphore->Count = Count;
}

inline void
semaphore_wait(struct semaphore *Semaphore)
{
    for(;;)
    {
        int32_t PreviousCount = Semaphore->Count;
        int32_t NewCount = PreviousCount - 1;
        _WriteBarrier();
        if(NewCount >= 0)
        {
            if(InterlockedCompareExchange((volatile LONG *)&Semaphore->Count, NewCount, PreviousCount) == PreviousCount)
            {
                break;
            }
        }

        _mm_pause();
    }
}

inline void
semaphore_notify(struct semaphore *Semaphore)
{
    for(;;)
    {
        int32_t PreviousCount = Semaphore->Count;
        int32_t NewCount = PreviousCount + 1;
        _WriteBarrier();
        if(NewCount)
        {
            if(InterlockedCompareExchange((volatile LONG *)&Semaphore->Count, NewCount, PreviousCount) == PreviousCount)
            {
                break;
            }
        }

        _mm_pause();
    }
}

#define SEMAPHORE_H
#endif
