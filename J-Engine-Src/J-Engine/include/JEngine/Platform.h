#pragma once

#define STR(x) #x

#ifdef _MSC_VER
#define ALIGNAS(x) alignas(x)
#else
#error "Align as not implemented for the current compiler!"
#endif

#define JE_BEG_PACK _Pragma("pack(push, 1)")
#define JE_END_PACK _Pragma("pack(pop)")

// Should probably check for compiler 
// specific usages for '__forceinline' too, 
// but for now I won't bother.
#define ACTUALLY_FORCE_INLINE __forceinline

#ifdef JE_DEBUG
#define FORCE_INLINE inline
#elif
#define FORCE_INLINE ACTUALLY_FORCE_INLINE
#endif

#define NO_DISCARD [[nodiscard]]