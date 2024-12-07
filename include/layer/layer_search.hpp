#ifndef LAYER_EXTRSEARCH_HPP
#define LAYER_EXTRSEARCH_HPP

#include <layer/layer_base.hpp>
#include <layer/layer_eplist.hpp>

#include <widgets/widget_animegrid.hpp>
#include <widgets/widget_spinner.hpp>

namespace layer {
	enum SearchType {
		POPULAR,
		QUERY
	};
	class Search : Layer {
	private:
		SearchType type;
		bool loading;
		std::string search_buff;
		extractor::Anime* selected;
		std::future<extractor::AnimeList> animlistFt;
		extractor::Extractor* extractor;
		extractor::AnimeList popList;
		extractor::AnimeList queryList;
	public:
		Search(extractor::Extractor* extractor);
		void main();
		void searchBar();
	};
}

#endif
