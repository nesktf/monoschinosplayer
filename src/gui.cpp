#include <gui.hpp>

namespace gui {
	layer::VideoPlayer* playerLayer;
	std::vector<layer::LayerStack> stackVec;
	int stackIndex = 0;

	void renderLoop() {
		stackVec.push_back(layer::LayerStack((layer::Layer*) new layer::Explore())); // Library
		stackVec.push_back(layer::LayerStack((layer::Layer*) new layer::Explore())); // Explore
		stackVec.push_back(layer::LayerStack((layer::Layer*) new layer::Explore())); // Settings

		while(!done) {
			gui::eventHandler();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			mainLayer();

			ImGui::Render();
			mpv::redraw(window);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(window);
		}
	}
	void eventHandler() {
		int w, h;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			switch(event.type) {
				case layer::INITPLAYER:
					playerLayer = new layer::VideoPlayer(
						(extractor::StreamList) event.user.data1,
						(extractor::Extractor*) event.user.data2
					);
					break;
				case layer::QUITPLAYER:
					delete playerLayer; playerLayer = nullptr;
					SDL_SetWindowTitle(window, "MonosChinosPlayer");
					break;
				case layer::PUSHLAYER:
					stackVec[stackIndex].push((layer::Layer*) event.user.data1);
					break;
				case layer::POPLAYER:
					stackVec[stackIndex].pop();
					break;
				case SDL_QUIT:
					done = true;
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_CLOSE)
						done = true;
				default:
					mpv::events(&event);
					if (playerLayer == nullptr)
						stackVec[stackIndex].sendEvent(&event);
					else
						playerLayer->sendEvent(&event);
					break;
			}
		}
		SDL_GetWindowSize(window, &w, &h);
		win_size.x = w;
		win_size.y = h;
	}
	void mainLayer() {
		// ImGui::ShowDemoWindow();
		if (playerLayer == nullptr) {
			ImGui::Begin("MainLayout", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGui::SetWindowSize(win_size, ImGuiCond_Always);
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);

			ImGui::BeginTabBar("MainTabs");
			if (ImGui::BeginTabItem("Library")) {
				// selectedStack = stackMap["Library"];
				// selectedStack->render();
				stackIndex = 0;
				stackVec[0].render();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Explore")) {
				// selectedStack = stackMap["Explore"];
				// selectedStack->render();
				stackIndex = 1;
				stackVec[1].render();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Settings")) {
				// selectedStack = stackMap["Settings"];
				// selectedStack->render();
				stackIndex = 2;
				stackVec[2].render();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

			ImGui::End();
		}
		else {
			playerLayer->render();
		}
	}

}
