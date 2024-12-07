#include <layer/layer_eplist.hpp>

namespace layer {
	/*============================================================*/
	// Episode selection layer
	AnimeInfo::AnimeInfo(extractor::Anime* anime, extractor::Extractor* extractor) : Layer("AnimeInfo") {
		this->anime = anime;
		this->extractor = extractor;

		this->eplistFt = std::async(&extractor::Extractor::epReq, extractor, *anime);

		this->loadingList = true;
		this->loadingStreams = false;
		this->popup = true;
	}

	void AnimeInfo::main() {
		ImGui::BeginChild("AnimeInfo", ImGui::GetContentRegionAvail());

		if (ImGui::BeginPopupModal("EplistLoading", &popup, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
			ImSpinner::SpinnerIncScaleDots("aca", 35.0f, 5.0f, ImColor(255,255,255), 10.0f, 35);
			ImGui::EndPopup();
		}

		if (popup) {
			ImGui::OpenPopup("EplistLoading");
			// popup = false;
		}

		if (loadingStreams && streamlistFt.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout) {
			popup = loadingStreams = false;
			SDL_Event event;
			event.type = INITPLAYER;
			event.user.data1 = streamlistFt.get();
			event.user.data2 = extractor;
			SDL_PushEvent(&event);
			// std::cout << streamlist->at(0)->url << std::endl;
		}

		if (ImGui::Button("<<")) {
			SDL_Event event;
			event.type = POPLAYER;
			SDL_PushEvent(&event);
		}

		ImGui::SameLine();
		ImGui::Text("Datos de tu anime feo");
		ImGui::Separator();
		ImGui::Text("%s", anime->title.c_str());
		//ImGui::Text("Cant. Episodios: %i", (int) list.get()->size());
		//ImGui::Separator();

		if (loadingList) {
			// ImGui::Text("Carganding");
			if (eplistFt.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout) {
				popup = loadingList = false;
				eplist = eplistFt.get();
			}
		}
		else {
			ImGui::Text("Cant. Episodios: %i", (int) eplist->size());
			ImGui::Separator();
			ImGui::BeginChild("EpList", ImGui::GetContentRegionAvail());
			for (extractor::Episode episode : *eplist) {
				if (ImGui::Button(episode.title.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 30.0f))) {
					streamlistFt = std::async(&extractor::Extractor::streamParser, extractor, episode);
					popup = loadingStreams = true;
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
	}
	/*============================================================*/
}
