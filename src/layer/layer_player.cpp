#include <layer/layer_player.hpp>

namespace layer {
	/*============================================================*/
	// MPV layer
	VideoPlayer::VideoPlayer(const extractor::StreamList streams, extractor::Extractor* extractor) : Layer("VideoPlayer"){
		this->streams = streams;
		this->extractor = extractor;
		//std::cout << streams->at(0).url << std::endl;
		//this->currStream = &streams->at(0);

		//this->streamFt = std::async(&extractor::Extractor::streamExtr, extractor, &streams->at(0));
		//this->loading = true;

		this->showGui = true;

		if ((int) streams->size() > 0) {

			std::cout << "Extractors found: " << std::endl;
			for (extractor::Stream estream : *streams)
				std::cout << estream.title << std::endl;

			this->currStream = &streams->back();
			if (currStream->getStream != nullptr)
				mpv::load_file(currStream->getStream(currStream->url));
			else
				mpv::load_file(currStream->url);
		}
		else {
			std::cout << "No extractors found" << std::endl;
			SDL_Event event;
			event.type = QUITPLAYER;
			SDL_PushEvent(&event);
		}
	}
	void VideoPlayer::render() {
		/*
		if (loading) {
			if(streamFt.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout) {
				loading = false;
				stream = streamFt.get();
				if (stream != "NULL") {
					mpv::load_file(stream);
				}
				else {
					std::cout << "No Extractors found" << std::endl;
					SDL_Event event;
					event.type = QUITPLAYER;
					SDL_PushEvent(&event);
				}
			}
		}
		*/

		if (showGui) {
			ImGui::Begin(this->id.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGui::SetWindowSize(ImVec2(win_size.x, 100.0f));
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
			if (ImGui::Button("<<")){
				if (fullscreen) {
					SDL_SetWindowFullscreen(window, 0);
					fullscreen = !fullscreen;
				}
				mpv::clear_playlist();
				SDL_Event event;
				event.type = QUITPLAYER;
				SDL_PushEvent(&event);
			}
			ImGui::SameLine();
			if (ImGui::Button("pause"))
				mpv::toggle_pause();
			ImGui::SameLine();
			if (ImGui::Button("debug"))
				mpv::debug_info();

			focus = ImGui::IsWindowHovered();
			ImGui::End();
		}
	}
	void VideoPlayer::sendEvent(SDL_Event* event) {
		switch (event->type) {
			case SDL_MOUSEBUTTONDOWN:
				if(event->button.button == SDL_BUTTON_LEFT) {
					if(!showGui)
						showGui = true;
					else if (!focus) {
						showGui = false;
					}
				}
				if (!focus && event->button.button == SDL_BUTTON_RIGHT)
					mpv::toggle_pause();
				break;
			case SDL_KEYDOWN:
				switch (event->key.keysym.sym) {
					case SDLK_SPACE:
					case SDLK_k:
						mpv::toggle_pause();
						break;
					case SDLK_i:
						mpv::debug_info();
						break;
					case SDLK_o:
						mpv::show_progress();
						break;
					case SDLK_LEFT:
						mpv::seek("-5");
						break;
					case SDLK_RIGHT:
						mpv::seek("5");
						break;
					case SDLK_UP:
						mpv::seek("60");
						break;
					case SDLK_DOWN:
						mpv::seek("-60");
						break;
					case SDLK_l:
						mpv::seek("10");
						break;
					case SDLK_j:
						mpv::seek("-10");
						break;
					case SDLK_f:
						if (fullscreen)
							SDL_SetWindowFullscreen(window, 0);
						else
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						fullscreen = !fullscreen;
						break;
				}
				break;
		}
	}
	/*============================================================*/
	// void VideoPlayer::guiTimer() {
	// 	this->showGui = true;
	// 	std::thread t([=]() {
	// 		if(!showGui.load()) return;
	// 		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	// 		if(!showGui.load()) return;
	// 		showGui = false;
	// 	});
	// 	t.detach();
	// }
}
