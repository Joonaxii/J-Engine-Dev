#pragma once
#include <JEngine/Core/Assert.h>
#include <JEngine/Platform.h>
#include <new>
#include <memory>
#include <type_traits>

// Simple memory "management" system, doesn't track but could be expanded upon later on
// Loosely based on Godot's 4.2 memory.h - https://github.com/godotengine/godot/blob/4.2/core/os/memory.h 

namespace JEngine {
    class Memory {
    public:
        static void memcpy(void* dst, const void* src, size_t size);
        static void memset(void* dst, int32_t value, size_t size);

        static void* alloc(size_t size);
        static void* realloc(void* pMem, size_t size);
        static void free(void* pMem);

        static void* aligned_alloc(size_t alignment, size_t size);
        static void* aligned_realloc(size_t alignment, void* pMem, size_t size);
        static void aligned_free(size_t alignment, void* pMem);

        static uint64_t getAllocationCount();
        static uint64_t getAlignedAllocBytes();

        template<typename T>
        static constexpr void memcpy_expr(T* dst, const T* src, size_t size) {
            for (size_t i = 0; i < size; i++)
            {
                dst[i] = src[i];
            }
        }

        template<typename T>
        static constexpr void memset_expr(T* dst, T value, size_t size) {
            for (size_t i = 0; i < size; i++)
            {
                dst[i] = value;
            }
        }

    private:
        static uint64_t _allocCount;
        static uint64_t _alignedAlloc;
    };

    enum CommonAligns : size_t {
        ALIGN_1 = 1,
        ALIGN_2 = 2,
        ALIGN_4 = 4,
        ALIGN_8 = 8,
        ALIGN_16 = 16,
        ALIGN_32 = 32,
        ALIGN_64 = 64,
    };

    template<size_t alignment = ALIGN_1>
    class DefaultAllocator {
    public:
        FORCE_INLINE static void* alloc(size_t size) {
            return alignment > 1 ? Memory::aligned_alloc(alignment, size) : Memory::alloc(size);
        }

        FORCE_INLINE static void* realloc(void* prevMem, size_t size) {
            return  alignment > 1 ? Memory::aligned_realloc(alignment, prevMem, size) : Memory::realloc(prevMem, size);
        }

        FORCE_INLINE static void free(void* mem) {
            if (alignment > 1) {
                Memory::aligned_free(alignment, mem);
            }
            else {
                Memory::free(mem);
            }
        }
    };

    template<typename T>
    void memDestruct(T* item) {
        JE_CORE_RET_IF_FALSE_MSG(item, "Attempting to destruct a nullptr!", void());
        if (!std::is_trivially_destructible<T>::value) {
            item->~T();
        }
    }

    template<typename T>
    void memDelete(T* item) {
        JE_CORE_RET_IF_FALSE_MSG(item, "Attempting to delete a nullptr!", void());
        if (!std::is_trivially_destructible<T>::value) { item->~T(); }
        Memory::free(item);
    }
}

void* operator new(size_t size, const char* id);
void* operator new(size_t size, void*(*allocFunc)(size_t size));

void operator delete(void* pMem, const char* id);
void operator delete(void* pMem, void* (*allocFunc)(size_t size));

#define JE_ALLOC(SIZE) ::JEngine::Memory::alloc(SIZE)
#define JE_REALLOC(P_MEM, SIZE) ::JEngine::Memory::realloc(P_MEM, SIZE)
#define JE_FREE(MEM) ::JEngine::Memory::free(MEM)
#define JE_ALLOC_T(T, SIZE) reinterpret_cast<T*>(::JEngine::Memory::alloc(SIZE))
#define JE_REALLOC_T(T, P_MEM, SIZE) reinterpret_cast<T*>(::JEngine::Memory::realloc(P_MEM, SIZE))

#define JE_ALLOC_ALIGNED(ALIGNMENT, SIZE) ::JEngine::Memory::aligned_alloc(ALIGNMENT, SIZE)
#define JE_REALLOC_ALIGNED(ALIGNMENT, P_MEM, SIZE) ::JEngine::Memory::aligned_realloc(ALIGNMENT, P_MEM, SIZE)
#define JE_FREE_ALIGNED(ALIGNMENT, MEM) ::JEngine::Memory::aligned_free(ALIGNMENT, MEM)
#define JE_ALLOC_ALIGNED_T(ALIGNMENT, SIZE) reinterpret_cast<T*>(::JEngine::Memory::aligned_alloc(ALIGNMENT, SIZE))
#define JE_REALLOC_ALIGNED_T(ALIGNMENT, P_MEM, SIZE) reinterpret_cast<T*>(::JEngine::Memory::aligned_realloc(ALIGNMENT, P_MEM, SIZE))

#define JE_NEW(TYPE) (new("") TYPE)
#define JE_NEW_ALLOCATOR(TYPE, ALLOCATOR) (new(ALLOCATOR) TYPE)
#define JE_NEW_PLACEMENT(PLACEMENT, TYPE) (new(PLACEMENT) TYPE)

#define JE_COPY(DST, SRC, SIZE) ::JEngine::Memory::memcpy(DST, SRC, SIZE)
#define JE_MEMSET(DST, VAL, SIZE) ::JEngine::Memory::memset(DST, VAL, SIZE)
#define JE_ZERO_MEM(DST, SIZE) ::JEngine::Memory::memset(DST, 0, SIZE)

#define JE_COPY_EXPR(DST, SRC, SIZE) ::JEngine::Memory::memcpy_expr<TYPE>(DST, SRC, SIZE)
#define JE_ZERO_EXPR(DST, SIZE) ::JEngine::Memory::memset_expr<TYPE>(DST, 0, SIZE)

