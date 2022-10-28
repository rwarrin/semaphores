struct producer_consumer
{
    int32_t *Buffer;
    int32_t Size;
    int32_t Front;
    int32_t Back;

    struct semaphore BufferMutex;
    struct semaphore SlotsSemaphore;
    struct semaphore ItemsSemaphore;
};

struct consumer_data
{
    int32_t ThreadID;
    struct producer_consumer *ProducerConsumer;
};

static void
ProducerConsumerInit(struct producer_consumer *ProducerConsumer, int32_t Size)
{
    ProducerConsumer->Size = Size;
    ProducerConsumer->Front = ProducerConsumer->Back = 0;
    ProducerConsumer->Buffer = (int32_t *)malloc(sizeof(*ProducerConsumer->Buffer) * ProducerConsumer->Size);

    semaphore_init(&ProducerConsumer->BufferMutex, 1);
    semaphore_init(&ProducerConsumer->SlotsSemaphore, ProducerConsumer->Size);
    semaphore_init(&ProducerConsumer->ItemsSemaphore, 0);
}

static void
ProducerConsumerInsert(struct producer_consumer *ProducerConsumer, int32_t Value)
{
    semaphore_wait(&ProducerConsumer->SlotsSemaphore);
    semaphore_wait(&ProducerConsumer->BufferMutex);

    ProducerConsumer->Buffer[ProducerConsumer->Back++ % ProducerConsumer->Size] = Value;

    semaphore_notify(&ProducerConsumer->BufferMutex);
    semaphore_notify(&ProducerConsumer->ItemsSemaphore);
}

static int32_t
ProducerConsumerRemove(struct producer_consumer *ProducerConsumer)
{
    int32_t Result = 0;

    semaphore_wait(&ProducerConsumer->ItemsSemaphore);
    semaphore_wait(&ProducerConsumer->BufferMutex);

    Result = ProducerConsumer->Buffer[ProducerConsumer->Front++ % ProducerConsumer->Size];

    semaphore_notify(&ProducerConsumer->BufferMutex);
    semaphore_notify(&ProducerConsumer->SlotsSemaphore);

    return(Result);
}

static DWORD
ProducerThread(void *Data)
{
    struct producer_consumer *ProducerConsumer = (struct producer_consumer *)Data;
    int32_t Counter = 0;

    for(;;)
    {
        ProducerConsumerInsert(ProducerConsumer, Counter);
        printf("Produced: (%08d)\n", Counter++);
        fflush(stdout);
        Sleep(100);
    }

    return(0);
}

static DWORD
ConsumerThread(void *Data)
{
    struct consumer_data *ConsumerData = (struct consumer_data *)Data;
    struct producer_consumer *ProducerConsumer = ConsumerData->ProducerConsumer;
    int32_t ThreadID = ConsumerData->ThreadID;

    for(;;)
    {
        int32_t Value = ProducerConsumerRemove(ProducerConsumer);
        printf("                        (%02d) Consumed: (%08d)\n", ThreadID, Value);
        fflush(stdout);
        Sleep(1000 + ((rand() % 2) * 1000));
    }

    return(0);
}

static void
ProducerConsumerExample()
{
    printf("Producer/Consumer threading example\n"
           "Producer produces a new value every ~500ms while Consumers consume values\n"
           "every 1 to 3 seconds.\n"
           "Press CTRL+C to quit\n\n");

    struct producer_consumer ProducerConsumer = {0};
    ProducerConsumerInit(&ProducerConsumer, 32);

    CreateThread(0, 0, ProducerThread, (void *)&ProducerConsumer, 0, 0);

    struct consumer_data ConsumerDatas[ThreadCount - 1] = {0};
    for(int32_t i = 0; i < ThreadCount - 1; ++i)
    {
        struct consumer_data *ConsumerData = ConsumerDatas + i;
        ConsumerData->ProducerConsumer = &ProducerConsumer;
        ConsumerData->ThreadID = i;
        CreateThread(0, 0, ConsumerThread, (void *)ConsumerData, 0, 0);
    }

    for(;;)
    {
        Sleep(1000);
    }
}
