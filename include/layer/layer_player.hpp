#ifndef LAYER_PLAYER
#define LAYER_PLAYER

#include <mpv.hpp>
#include <layer/layer_base.hpp>
#include <extractor/extr_base.hpp>

namespace layer {
	/*============================================================*/
  class VideoPlayer : Layer {
  private:
		std::future<std::string> streamFt;
		std::string stream;
		bool focus;
		bool showGui;
		bool loading;
		// extractor::Episode* episode;
		extractor::StreamList streams;
		extractor::Extractor* extractor;
		// std::string selstream;
		// void guiTimer();
		extractor::Stream* currStream;
	public:
		VideoPlayer(const extractor::StreamList streams, extractor::Extractor* extractor);
		void render();
		void sendEvent(SDL_Event* event);
	};
	/*============================================================*/
}

#endif

