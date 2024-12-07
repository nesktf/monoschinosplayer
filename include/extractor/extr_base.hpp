#ifndef EXTR_BASE_HPP
#define EXTR_BASE_HPP

#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <json.hpp>

namespace extractor {
	/*============================================================*/
	using Json = nlohmann::json;
	struct Anime;
	struct Episode;
	struct Stream;
	typedef std::vector<Anime>* AnimeList;
	typedef std::vector<Episode>* EpList;
	typedef std::vector<Stream>* StreamList;
	/*============================================================*/
	class Extractor {
	protected:
		std::string id;
		std::string base_url;
		std::string pop_url;
		std::string search_url;
	public:
		virtual AnimeList popReq();
		virtual AnimeList srchReq(const std::string &query);
		virtual EpList epReq(const Anime &anime);
		virtual StreamList streamParser(const Episode &episode);
		//virtual std::string streamExtr(const Stream* stream);
		std::string getId();
	};
	/*============================================================*/
	struct Anime {
		std::string url;
		std::string title;
		Anime(const std::string &url, const std::string &title);
	};
	/*============================================================*/
	struct Episode {
		std::string url;
		std::string title;
		Episode(const std::string &url, const std::string &title);
	};
	/*============================================================*/
	struct Stream {
		std::string url;
		std::string title;
		int id;
		std::string (*getStream)(const std::string&);
		Stream(const std::string &url, const std::string &title, std::string (*func)(const std::string&));
	};
	/*============================================================*/
}
#endif
