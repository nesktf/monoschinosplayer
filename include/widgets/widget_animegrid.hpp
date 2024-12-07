#ifndef WIDGET_ANIMEGRID_HPP
#define WIDGET_ANIMEGRID_HPP

#include <layer/layer_base.hpp>

namespace widget {
	bool AnimeGrid(const extractor::AnimeList &list, extractor::Anime** ret_anime);
	/*
	class AnimGrid {
	private:
		extractor::AnimeList* list;
	public:
		AnimGrid(extractor::AnimeList* list);
		void main();
	};
	*/
}

#endif
