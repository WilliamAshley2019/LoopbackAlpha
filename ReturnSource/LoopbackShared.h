#pragma once
#include <JuceHeader.h>

#if JUCE_WINDOWS
#include <windows.h>
#endif

// Ring buffer structure that lives in shared memory
struct SharedRingBuffer
{
    static constexpr int bufferSizeSamples = 262144;

    std::atomic<int> writePos{ 0 };
    std::atomic<int> readPos{ 0 };
    float leftChannel[bufferSizeSamples];
    float rightChannel[bufferSizeSamples];
    std::atomic<int64_t> totalWritten{ 0 };
    std::atomic<int64_t> totalRead{ 0 };
};

class LoopbackShared
{
public:
    static LoopbackShared& getInstance()
    {
        static LoopbackShared instance;
        return instance;
    }

    bool isInitialized() const { return sharedBuffer != nullptr; }

    void write(const float* left, const float* right, int numSamples) noexcept
    {
        if (!sharedBuffer) return;

        const int mask = SharedRingBuffer::bufferSizeSamples - 1;
        int writeIndex = sharedBuffer->writePos.load(std::memory_order_acquire);

        for (int i = 0; i < numSamples; ++i)
        {
            int idx = (writeIndex + i) & mask;
            sharedBuffer->leftChannel[idx] = left[i];
            sharedBuffer->rightChannel[idx] = right ? right[i] : left[i];
        }

        sharedBuffer->writePos.store((writeIndex + numSamples) & mask, std::memory_order_release);
        sharedBuffer->totalWritten.fetch_add(numSamples, std::memory_order_relaxed);
    }

    void read(float* left, float* right, int numSamples) noexcept
    {
        if (!sharedBuffer)
        {
            juce::FloatVectorOperations::clear(left, numSamples);
            if (right) juce::FloatVectorOperations::clear(right, numSamples);
            return;
        }

        const int mask = SharedRingBuffer::bufferSizeSamples - 1;
        int readIndex = sharedBuffer->readPos.load(std::memory_order_acquire);
        int writeIndex = sharedBuffer->writePos.load(std::memory_order_acquire);

        // Calculate available samples
        int available = (writeIndex - readIndex) & mask;
        int toRead = juce::jmin(available, numSamples);

        // Read available samples
        for (int i = 0; i < toRead; ++i)
        {
            int idx = (readIndex + i) & mask;
            left[i] = sharedBuffer->leftChannel[idx];
            if (right) right[i] = sharedBuffer->rightChannel[idx];
        }

        // Fill remainder with silence if underrun
        if (toRead < numSamples)
        {
            juce::FloatVectorOperations::clear(left + toRead, numSamples - toRead);
            if (right) juce::FloatVectorOperations::clear(right + toRead, numSamples - toRead);
        }

        sharedBuffer->readPos.store((readIndex + toRead) & mask, std::memory_order_release);
        sharedBuffer->totalRead.fetch_add(toRead, std::memory_order_relaxed);
    }

    int getNumAvailable() const noexcept
    {
        if (!sharedBuffer) return 0;

        int writeIndex = sharedBuffer->writePos.load(std::memory_order_acquire);
        int readIndex = sharedBuffer->readPos.load(std::memory_order_acquire);
        return (writeIndex - readIndex) & (SharedRingBuffer::bufferSizeSamples - 1);
    }

    int64_t getTotalWritten() const noexcept
    {
        return sharedBuffer ? sharedBuffer->totalWritten.load(std::memory_order_relaxed) : 0;
    }

    int64_t getTotalRead() const noexcept
    {
        return sharedBuffer ? sharedBuffer->totalRead.load(std::memory_order_relaxed) : 0;
    }

private:
    LoopbackShared()
    {
#if JUCE_WINDOWS
        // Create or open named shared memory
        const char* mapName = "Global\\LoopbackAudioBuffer_V1";

        hMapFile = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(SharedRingBuffer),
            mapName
        );

        if (hMapFile == NULL)
        {
            DBG("Failed to create file mapping");
            return;
        }

        bool existed = (GetLastError() == ERROR_ALREADY_EXISTS);

        sharedBuffer = (SharedRingBuffer*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(SharedRingBuffer)
        );

        if (sharedBuffer == NULL)
        {
            DBG("Failed to map view of file");
            CloseHandle(hMapFile);
            hMapFile = NULL;
            return;
        }

        // If we created it (first time), zero it out
        if (!existed)
        {
            memset(sharedBuffer, 0, sizeof(SharedRingBuffer));
            DBG("Created new shared memory region");
        }
        else
        {
            DBG("Opened existing shared memory region");
        }
#else
        // macOS/Linux would use POSIX shared memory here
        DBG("Shared memory only implemented for Windows");
#endif
    }

    ~LoopbackShared()
    {
#if JUCE_WINDOWS
        if (sharedBuffer)
        {
            UnmapViewOfFile(sharedBuffer);
            sharedBuffer = nullptr;
        }
        if (hMapFile)
        {
            CloseHandle(hMapFile);
            hMapFile = NULL;
        }
#endif
    }

    LoopbackShared(const LoopbackShared&) = delete;
    LoopbackShared& operator=(const LoopbackShared&) = delete;

#if JUCE_WINDOWS
    HANDLE hMapFile = NULL;
#endif
    SharedRingBuffer* sharedBuffer = nullptr;
};