#include "ImGui/imgui.h"
#ifndef IMGUI_DISABLE
#include "ImGui/imgui_internal.h"

// System includes
#include <stdint.h>     // intptr_t
#endif

#include <stdio.h>      // FILE*, sscanf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi, atof
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <limits.h>     // INT_MIN, INT_MAX

namespace ImGuiCustom
{
    void Theme_OrangeFont();

    // Other Widgets
    void HelpMarker(const char* desc);
}
