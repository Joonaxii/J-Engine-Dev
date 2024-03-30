#pragma once

#include <cstdint>
#include <JEngine/Core/Log.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Utility/Version.h>

#ifdef JE_DEBUG
#ifdef JE_EDITOR
static inline constexpr const char JE_VERSION_STR[] = "DEBUG-EDITOR";
#else
static inline constexpr const char JE_VERSION_STR[] = "DEBUG";
#endif
#else
#ifdef JE_EDITOR
static inline constexpr const char JE_VERSION_STR[] = "RELEASE-EDITOR";
#else
static inline constexpr const char JE_VERSION_STR[] = "RELEASE";
#endif
#endif
static inline constexpr JVersion JE_VERSION(0, 0, 0);

