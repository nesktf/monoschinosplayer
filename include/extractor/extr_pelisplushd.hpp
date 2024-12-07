#ifndef EXTR_PELISPLUSHD_HPP
#define EXTR_PELISPLUSHD_HPP

#include <util.hpp>
#include <extractor/extr_base.hpp>

namespace extractor {
	class PelisPlusHD : Extractor {
	private:
		StreamList fembedParser(const std::string &url, bool directApi = false, std::string baselabel = "");
		StreamList moeParser(const std::string &url);
	public:
		PelisPlusHD();
		AnimeList popReq();
		AnimeList srchReq(const std::string &query);
		EpList epReq(const Anime &anime);
		StreamList streamParser(const Episode &episode);
		//std::string streamExtr(const Stream* stream);

		//static std::string fembedStream(const std::string &url);
	};
}

#endif
