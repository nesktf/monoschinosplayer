#include <layer/layer_explore.hpp>

namespace layer {
	Explore::Explore() : Layer("Explore") {}

	void Explore::main() {
		ImGui::BeginChild("ServerSel", ImVec2(0, ImGui::GetContentRegionAvail().y));
		ImGui::Text("Elige la forma mas conveniente de infringir copyright");
		ImGui::Separator();

		ImVec2 butt_size(ImGui::GetContentRegionAvail().x, 40.0f);

		if (ImGui::Button("JKAnime", butt_size)) {
			SDL_Event event;
			event.type = PUSHLAYER;
			event.user.data1 = new layer::Search((extractor::Extractor*) new extractor::JKAnime());
			SDL_PushEvent(&event);
		}

		if (ImGui::Button("PelisPlusHD", butt_size)) {
			SDL_Event event;
			event.type = PUSHLAYER;
			event.user.data1 = new layer::Search((extractor::Extractor*) new extractor::PelisPlusHD());
			SDL_PushEvent(&event);
		}

		ImGui::EndChild();
	}
}
