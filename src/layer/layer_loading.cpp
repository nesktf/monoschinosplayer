#include <layer/layer_loading.hpp>

layer::LoadingScreen::LoadingScreen(layer::LayerStack* parent) : layer::Layer("LoadingScreen", parent){};

void layer::LoadingScreen::main() {
	ImGui::BeginChild("Loading");
	ImSpinner::SpinnerIncScaleDots("aca", 35.0f, 5.0f, ImColor(255,255,255), 10.0f, 35);
	ImGui::EndChild();
}
