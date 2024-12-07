#include "extractor/extr_base.hpp"
#include <algorithm>
#include <extractor/extr_pelisplushd.hpp>

namespace extractor {
	PelisPlusHD::PelisPlusHD() {
		this->id = "PELISPLUSHD";
		this->base_url = "https://pelisplushd.is";
		this->pop_url = base_url + "/series";
		this->search_url = base_url + "/search";
	}

	AnimeList PelisPlusHD::popReq() {
		AnimeList list = new std::vector<Anime>();

		network::req popreq{pop_url};

		std::vector<std::string> raw_pop = util::split(
			util::findBetween(popreq.body, "\"Posters\"", "example"),
			"<a"
		);
		raw_pop.erase(raw_pop.begin());

		for (std::string raw : raw_pop) {
			Anime wea{
				util::findBetween(raw, "href=\"", "\" class"),	// url
				util::findBetween(raw, "<p>", "</p>")			// title
			};
			list->push_back(wea);
		}
		
		return list;
	}

	AnimeList PelisPlusHD::srchReq(const std::string &query) {
		AnimeList list = new std::vector<Anime>();

		network::req srchreq{search_url+"?s="+util::findReplace(query, " ", "+")};

		std::vector<std::string> raw_srch = util::split(
			util::findBetween(srchreq.body, "\"Posters\"", "content-footer"),
			"<a"
		);
		raw_srch.erase(raw_srch.begin());

		for (std::string raw : raw_srch) {
			list->push_back(Anime(
				util::findBetween(raw, "href=\"", "\" class"),	// url
				util::findBetween(raw, "<p>", "</p>")			// title
			));
		}

		return list;
	}

	EpList PelisPlusHD::epReq(const Anime &anime) {
		EpList list = new std::vector<Episode>();
		network::req epsreq{anime.url};

		//Special case for movies
		if (anime.url.find("pelicula") != std::string::npos) {
			list->push_back(Episode(
				anime.url,
				anime.title
			));
			return list;
		}

		std::vector<std::string> raw_eps = util::split(
			util::findBetween(epsreq.body, "tab-content", "\"clear\""),
			"tabpanel"
		);
		raw_eps.erase(raw_eps.begin());

		for (std::string raw : raw_eps) {
			std::vector<std::string> curr = util::split(raw, "<a");
			curr.erase(curr.begin());
			for (std::string i : curr)
				list->push_back(Episode(
					util::findBetween(i, "href=\"", "\" class"),						// url
					util::findReplace(util::findBetween(i, "block\">", "</a"),"\n", "")	// title
				));
		}

		return list;
	}

	StreamList PelisPlusHD::streamParser(const Episode &episode) {
		StreamList list = new std::vector<Stream>();
		network::req req{episode.url};

		std::vector<std::string> raw_req = util::split(
			util::findBetween(req.body, "slug", "script"),
			"video["
		);
		raw_req.erase(raw_req.begin());

		for (std::string raw : raw_req) {
			std::string currUrl = util::findBetween(raw, "'", "';");

			if(currUrl.find("furl.php") != std::string::npos) {
				StreamList fembedList = fembedParser(currUrl);
				list->insert(list->end(), fembedList->begin(), fembedList->end());
				delete fembedList;
			}
			else if ((currUrl.find("embed.php") != std::string::npos)|| (currUrl.find(".moe/video/") != std::string::npos)) { 
				StreamList moeList = moeParser(currUrl);
				list->insert(list->end(), moeList->begin(), moeList->end());
				delete moeList;
			}
			else
				continue;
		}

		return list;
	}

	StreamList PelisPlusHD::fembedParser(const std::string &url, bool directApi, std::string baselabel) {
		StreamList list = new std::vector<Stream>();
		std::cout << "Found fembed "+baselabel << std::endl;
		std::string newurl;
		if (!directApi) {
			network::req req{url};
			newurl = util::findReplace(
				util::findBetween(req.body, "replace(\"", "\")"),
				"/v/",
				"/api/source/"
			);
		}
		else newurl = util::findReplace(url, "/v/", "/api/source/");

		network::req req2{newurl, false, "wea=wea"};

		Json json = Json::parse(req2.body);

		for (Json item : json["data"]) {
			std::string label = item["label"];
			if (baselabel == "")
				label = "Fembed: "+label;
			else
				label = baselabel+": "+label;
			list->push_back(Stream(
				item["file"],	// url
				label,			// stream label
				nullptr			// callback decoding function
			));
		}
		return list;
	}

	StreamList PelisPlusHD::moeParser(const std::string &url) {
		StreamList list = new std::vector<Stream>();
		network::req req{url};

		std::vector<std::string> raw_urls = util::split(req.body, "go_to_player");
		raw_urls.erase(raw_urls.begin());

		for (std::string raw : raw_urls) {
			std::string currRedirect;
			std::string currUrl;

			if (raw.find("api.mycdn.moe") != std::string::npos) {
				currRedirect = util::findBetween(raw, "('", "')");
				network::req req2{currRedirect, true};
				currUrl = util::findBetween(req2.body, "location.href='", "';</script>");
			}
			else {
				currRedirect = "https://api.mycdn.moe/player/?id="+util::findBetween(raw, "('", "',");
				network::req req2{currRedirect, true};
				currUrl = util::findBetween(req2.body, "src=\"", "\" frame");
			}

			if (currUrl.find("/v/") != std::string::npos) {
				StreamList fembedList = fembedParser(currUrl, true, "MoeFembed");
				list->insert(list->end(), fembedList->begin(), fembedList->end());
				delete fembedList;
			}
		}

		return list;
	}
}
