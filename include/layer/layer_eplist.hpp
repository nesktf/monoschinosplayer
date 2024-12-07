#ifndef LAYER_EPLIST
#define LAYER_EPLIST

#include <extractor/extr_base.hpp>

#include <layer/layer_base.hpp>
#include <layer/layer_player.hpp>

#include <widgets/widget_spinner.hpp>

namespace layer {
	/*============================================================*/
	class AnimeInfo : Layer {
	private:
		bool loadingList;
		bool loadingStreams;
		bool popup;
		extractor::Episode* selected;
		std::future<extractor::EpList> eplistFt;
		std::future<extractor::StreamList> streamlistFt;
		extractor::EpList eplist;
		extractor::StreamList streamlist;
		extractor::Extractor* extractor;
		extractor::Anime* anime;
		std::string desc;
	public:
		AnimeInfo(extractor::Anime* anime, extractor::Extractor* extractor);
		void main();
	};
	/*============================================================*/
}

#endif
