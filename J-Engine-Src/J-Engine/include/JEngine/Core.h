#pragma once

#include <JEngine/Core/Log.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Utility/Version.h>

#ifdef JENGINE_DEBUG
#ifdef JENGINE_EDITOR
static inline constexpr const char JENGINE_VERSION_STR[] = "DEBUG-EDITOR";
#else
static inline constexpr const char JENGINE_VERSION_STR[] = "DEBUG";
#endif
#else
#ifdef JENGINE_EDITOR
static inline constexpr const char JENGINE_VERSION_STR[] = "RELEASE-EDITOR";
#else
static inline constexpr const char JENGINE_VERSION_STR[] = "RELEASE";
#endif
#endif
static inline constexpr JVersion JENGINE_VERSION(0, 0, 0);
