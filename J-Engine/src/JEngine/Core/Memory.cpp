#include <JEngine/Core/Memory.h>
#include <JEngine/Math/Math.h>
#include <malloc.h>

namespace JEngine {
    uint64_t Memory::_allocCount = 0;
    uint64_t Memory::_alignedAlloc = 0;

    uint64_t Memory::getAllocationCount() { 
        return _allocCount; 
    }

    uint64_t Memory::getAlignedAllocBytes() {
        return _alignedAlloc;
    }

    void Memory::memcpy(void* dst, const void* src, size_t size) {
        JE_CORE_RET_IF_FALSE_MSG(dst && src, "Failed to copy memory! (Destination or Source was null)", void(0));
        std::memcpy(dst, src, size);
    }

    void Memory::memset(void* dst, int32_t value, size_t size) {
        JE_CORE_RET_IF_FALSE_MSG(dst, "Failed to copy memory! (Destination was null)", void(0));
        std::memset(dst, value, size);
    }

    void* Memory::alloc(size_t size) {
        void* allocMem = std::malloc(size);
        JE_CORE_RET_IF_FALSE_MSG(allocMem, "Failed to allocate memory!", nullptr);

        _allocCount++;
        return allocMem;
    }

    void* Memory::realloc(void* pMem, size_t size) {
        if (pMem == nullptr) {
            return alloc(size);
        }

        if (size == 0) {
            free(pMem);
            return nullptr;
        }

        // This macro returns the input if it evaluates to true, and returns a given value if false
        JE_CORE_RET_COND_MSG(std::realloc(pMem, size), "Failed to reallocate memory!", pMem);
    }

    void Memory::free(void* pMem) {
        JE_CORE_RET_IF_FALSE_MSG(pMem, "Cannot free a null pointer!", void());

        _allocCount--;
        std::free(pMem);
    }

    void* Memory::aligned_alloc(size_t alignment, size_t size) {
        JE_CORE_RET_IF_FALSE_MSG(alignment > 0, "Failed to allocate aligned memory! (Invalid alignment)", nullptr);

        void* allocMem = _aligned_malloc(size + alignment, alignment);
        JE_CORE_RET_IF_FALSE_MSG(allocMem, "Failed to allocate aligned memory!", nullptr);
        _allocCount++;

        // Aligned blocks will also store the size before the block which is used in aligned_realloc
        *reinterpret_cast<uint64_t*>(allocMem) = size;
        _alignedAlloc += size;
        return reinterpret_cast<uint8_t*>(allocMem) + alignment;
    }

    void* Memory::aligned_realloc(size_t alignment, void* pMem, size_t size) {
        JE_CORE_RET_IF_FALSE_MSG(alignment > 0, "Failed to reallocate aligned memory! (Invalid alignment)", pMem);

        if (pMem == nullptr) {
            return aligned_alloc(alignment, size);
        }

        if (size == 0) {
            aligned_free(alignment, pMem);
            return nullptr;
        }

        const uint8_t* ui8 = reinterpret_cast<const uint8_t*>(pMem);
        JE_CORE_RET_IF_FALSE_MSG(size_t(ui8) > alignment, "Failed to reallocate aligned memory! (Invalid pointer location)", pMem);

        void* reloc = aligned_alloc(alignment, size);
        JE_CORE_RET_IF_FALSE_MSG(reloc, "Failed to reallocate aligned memory! (Allocation returned null)", pMem);

        memcpy(reloc, pMem, Math::min(size, *(reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(pMem) - alignment))));
        aligned_free(alignment, pMem);
        return reloc;
    }

    void Memory::aligned_free(size_t alignment, void* pMem) {
        JE_CORE_RET_IF_FALSE_MSG(alignment > 0, "Failed to reallocate aligned memory! (Invalid alignment)", void());
        JE_CORE_RET_IF_FALSE_MSG(size_t(reinterpret_cast<uint8_t*>(pMem)) > alignment, "Cannot free an invalid or null pointer!", void());
        uint64_t* ogPtr = reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(pMem) - alignment);
        _allocCount--;
        _alignedAlloc -= *ogPtr;
        _aligned_free(ogPtr);
    }
}

void* operator new(size_t size, const char* id) {
    return JEngine::Memory::alloc(size);
}

void* operator new(size_t p_size, void*(*allocFunc)(size_t size)) {
    return allocFunc(p_size);
}

void operator delete(void* pMem, const char* id) {
    JE_CORE_ASSERT(false, "Call to delete should not be happening!");
}

void operator delete(void* pMem, void* (*allocFunc)(size_t size)) {
    JE_CORE_ASSERT(false, "Call to delete should not be happening!");
}
