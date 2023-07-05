#pragma once

#include <JEngine/Core/Log.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Utility/Version.h>

#ifdef JENGINE_DEBUG
static inline constexpr const char JENGINE_VERSION_STR[] = "DEBUG-EARLY-ALPHA";
#else
static inline constexpr const char JENGINE_VERSION_STR[] = "EARLY-ALPHA";
#endif
static inline constexpr JVersion JENGINE_VERSION(0, 0, 0);
