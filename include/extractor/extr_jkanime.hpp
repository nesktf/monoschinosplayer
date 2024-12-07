#ifndef EXTR_JKANIME_HPP
#define EXTR_JKANIME_HPP

#include <util.hpp>
#include <extractor/extr_base.hpp>

namespace extractor {
	/*============================================================*/
	/*
	enum StreamTypeJK {
		JK_NOZOMI,
		JK_DESU,
		JK_OKRU,
		JK_FEMBED,
		JK_DESUKA
	};
	*/
	/*============================================================*/
	class JKAnime : Extractor {
	public:
		JKAnime();
		AnimeList popReq();
		AnimeList srchReq(const std::string &query);
		EpList epReq(const Anime &anime);
		StreamList streamParser(const Episode &episode);
		
		static std::string nozomiStream(const std::string &url);
	};
	/*============================================================*/
}

#endif
