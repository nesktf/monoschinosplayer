#include <layer/layer_search.hpp>

namespace layer {
	Search::Search(extractor::Extractor* extractor) : Layer("Search") {
		this->extractor = extractor;
		this->selected = nullptr;

		this->animlistFt = std::async(&extractor::Extractor::popReq, extractor);
		this->loading = true;
		this->type = POPULAR;
	}

	void Search::main() {
		ImGui::BeginChild("Search", ImVec2(0, ImGui::GetContentRegionAvail().y));
		if (ImGui::BeginPopupModal("SearchLoading", &loading, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
			ImSpinner::SpinnerIncScaleDots("aca", 35.0f, 5.0f, ImColor(255,255,255), 10.0f, 35);
			ImGui::EndPopup();
		}
		if (ImGui::Button("<<", ImVec2(30.0f, 20.0f))) {
			SDL_Event event;
			event.type = POPLAYER;
			SDL_PushEvent(&event);
		}
		ImGui::SameLine();
		searchBar();
		if (loading) {
			ImGui::OpenPopup("SearchLoading");
			if (animlistFt.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout) {
				loading = false;
				switch(type) {
					case POPULAR:
						popList = animlistFt.get();
						break;
					case QUERY:
						queryList = animlistFt.get();
						break;
				}
			}
		}
		else {
			switch(type) {
				case POPULAR:
					if (widget::AnimeGrid(popList, &selected)) {
						SDL_Event event;
						event.type = PUSHLAYER;
						event.user.data1 = new layer::AnimeInfo(selected, extractor);
						SDL_PushEvent(&event);
					}
					break;
				case QUERY:
					if (widget::AnimeGrid(queryList, &selected)) {
						SDL_Event event;
						event.type = PUSHLAYER;
						event.user.data1 = new layer::AnimeInfo(selected, extractor);
						SDL_PushEvent(&event);
					}
					break;
			}
		}
		ImGui::EndChild();
	}

	void Search::searchBar() {
		ImGui::BeginChild("SearchBar", ImVec2(ImGui::GetContentRegionAvail().x, 30));

		if(type == QUERY) {
			if(ImGui::Button("<-", ImVec2(30.0f, 20.0f))) {
				type = POPULAR;
				delete queryList; queryList = nullptr;
				search_buff = "";
			}
			ImGui::SameLine();
		}

		ImGui::InputText("##", &this->search_buff);

		ImGui::SameLine();
		if (ImGui::Button("=>", ImVec2(30.0f, 20.0f))) {
			std::cout << search_buff << std::endl;
			animlistFt = std::async(&extractor::Extractor::srchReq, extractor, search_buff);
			loading = true;
			type = QUERY;
		}

		ImGui::EndChild();
	};
}
