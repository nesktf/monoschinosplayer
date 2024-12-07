#include <widgets/widget_animegrid.hpp>

namespace widget {
	bool AnimeGrid(const extractor::AnimeList &list, extractor::Anime** ret_anime) {
		bool retVal = false;
		ImGui::BeginChild("AnimeGrid", ImVec2(0, ImGui::GetContentRegionAvail().y));

		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(100, 100, 100, 100));
		ImGui::BeginChild("animgrid", ImVec2(0, ImGui::GetContentRegionAvail().y));
		ImGui::Columns(4, NULL, false);
		ImGui::Separator();

		for (int i = 0; i < (int) list->size(); ++i) {
			char num[16];
			sprintf(num, "%d", i);
			if (ImGui::Button(num, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x * 1.2f))) {
				*ret_anime = &list->at(i);
				// std::cout << ret_anime << std::endl;
				retVal = true;
			}

			ImGui::TextWrapped("%s", list->at(i).title.c_str());
			ImGui::NextColumn();
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::EndChild();
		return retVal;
	}

	/*
	AnimGrid::AnimGrid(extractor::AnimeList* list) {
		this->list = list;
	}
	void AnimGrid::main() {
		ImGui::BeginChild("eplist", ImVec2(0, ImGui::GetContentRegionAvail().y));

		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(100, 100, 100, 100));
		ImGui::BeginChild("animgrid", ImVec2(0, ImGui::GetContentRegionAvail().y));
		// ImGui::Text("Animes feos de temporada");
		ImGui::Columns(4, NULL, false);
		ImGui::Separator();

		for (int i = 0; i < (int) list->size(); ++i) {
			char num[16];
			sprintf(num, "%d", i);
			if (ImGui::Button(num, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x * 1.2f))) {
				mainparent->push((layer::Layer*) new EpList(list->at(i), mainparent));
			}

			ImGui::TextWrapped("%s", list->at(i)->title.c_str());
			ImGui::NextColumn();
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::EndChild();
	}
	*/
}
