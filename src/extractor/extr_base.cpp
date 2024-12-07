#include <extractor/extr_base.hpp>

namespace extractor {
	/*============================================================*/
	// Base extractor class filler definitions
	AnimeList Extractor::popReq() {
		return AnimeList();
	};
	AnimeList Extractor::srchReq(const std::string &query) {
		return AnimeList();
	};
	EpList Extractor::epReq(const Anime &anime) {
		return EpList();
	};
	StreamList Extractor::streamParser(const Episode &episode) {
		return StreamList();
	};
	//std::string Extractor::streamExtr(const Stream* stream) {
	//	return std::string();
	//};
	std::string Extractor::getId() {
		return this->id;
	};
	/*============================================================*/
	Anime::Anime(const std::string &url, const std::string &title) {
		this->url = url;
		this->title = title;
	}
	/*============================================================*/
	Episode::Episode(const std::string &url, const std::string &title) {
		this->url = url;
		this->title = title;
	}
	/*============================================================*/
	Stream::Stream(const std::string &url, const std::string &title, std::string (*func)(const std::string&)) {
		this->url = url;
		this->title = title;
		this->getStream = func;
	}
	/*============================================================*/
}
