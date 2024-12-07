#ifndef IMSPINNER_HPP
#define IMSPINNER_HPP

#include "imgui.h"
#include "imgui_internal.h"

namespace ImSpinner {
	void SpinnerIncScaleDots(const char *label, float radius, float thickness, const ImColor &color, float speed , size_t dots);
}

#endif
