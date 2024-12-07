#include <extractor/extr_jkanime.hpp>
#include <util.hpp>

namespace extractor {
	/*============================================================*/
	// Base extractor class redefinitions
	JKAnime::JKAnime() {
		this->id = "JKANIME";
		this->base_url = "https://jkanime.net";
		this->pop_url = base_url + "/top/";
		this->search_url = base_url + "/buscar/";
	}

	AnimeList JKAnime::popReq() {
		AnimeList list = new std::vector<Anime>();
		network::req popreq{pop_url};

		std::vector<std::string> raw_pop = util::split(
			util::findBetween(popreq.body, "contador,1000", "\"loadmore\""),
			"id=\"conb\""
		);
		raw_pop.erase(raw_pop.begin());

		for (std::string raw : raw_pop)
			list->push_back(Anime{
				base_url+"/"+util::findBetween(raw, "jkanime.net/","/\" rel")+"/",	// url
				util::findBetween(raw, "title=\"", "\" href")						// title
			});
		return list;
	}

	AnimeList JKAnime::srchReq(const std::string &query) {
		AnimeList list = new std::vector<Anime>();
		std::string base_url = search_url+util::findReplace(query, " ", "_")+"/";

		for (int i = 1; 1; ++i) {
			network::req srchreq {base_url+std::to_string(i)+"/"};
			std::vector<std::string> raw_srch = util::split(
				util::findBetween(srchreq.body, "Filtrar</button>","navigation"),
				"\"anime__item\""
			);

			if (raw_srch.size() == 1) break;
			else {
				raw_srch.erase(raw_srch.begin());
				for (std::string raw : raw_srch)
					list->push_back(Anime{
						this->base_url+"/"+util::findBetween(raw, "jkanime.net/", "/\"")+"/",	// url
						util::findBetween(raw, "\"title\">", "</div>")							// title
					});
			}
		}
		return list;
	}

	EpList JKAnime::epReq(const Anime &anime) {
		EpList list = new std::vector<Episode>();
		network::req anim_html {anime.url};
		std::string json_base = base_url+"/ajax/pagination_episodes/"+util::findBetween(anim_html.body,"data-anime=\"","\" ")+"/";

		for (int i = 1; 1; ++i) {
			network::req jsonreq {json_base+std::to_string(i)+"/"};
			Json json = Json::parse(jsonreq.body);
			if (json.size() == 0) break;
			else {
				for (Json jsonval : json)
					list->push_back(Episode(
						anime.url+jsonval["number"].get<std::string>()+"/",// url
						jsonval["title"].get<std::string>()					// title
					));
			}
		}
		return list;
	}

	StreamList JKAnime::streamParser(const Episode &episode) {
		StreamList list = new std::vector<Stream>();
		network::req req {episode.url, true};

		std::vector<std::string> raw_req = util::split(
				util::findBetween(req.body, "var video", "video_data"),
		";");
		raw_req.erase(raw_req.begin()); raw_req.pop_back();

		for (std::string stream : raw_req) {
			std::string currUrl=util::findBetween(stream, "src=\"", "\" ");
			//std::string title;
			//StreamTypeJK type;

			if (currUrl.find("um2.php") != std::string::npos) {
				list->push_back(Stream(
					currUrl,
					"Nozomi",
					JKAnime::nozomiStream
				));
				//title = "Nozomi";
				//type = StreamTypeJK::JK_NOZOMI;
			}
			/*
			else if (streamurl.find("um.php") != std::string::npos) {
				title = "Desu";
				type = StreamTypeJK::JK_NOZOMI;
			}
			else if (streamurl.find("jkokru.php") != std::string::npos) {
				title = "Okru";
				type = StreamTypeJK::JK_OKRU;
			}
			else if (streamurl.find("jkfembed.php") != std::string::npos) {
				title = "Fembed";
				type = StreamTypeJK::JK_FEMBED;
			}
			*/
			else
				continue;

			/*
			list->push_back(Stream(
				type,				// stream type
				base_url+streamurl,	// url
				title				// title
			));
			*/
		}
		return list;
	}

	/*
	std::string JKAnime::streamExtr(const Stream* stream) {
		switch (stream->id) {
			case StreamTypeJK::JK_NOZOMI:
				return nozomiStream(stream->url);
				break;
			default:
				return "";
				break;
		}
	}
	*/
	
	// Stream extractors
	std::string JKAnime::nozomiStream(const std::string &url){
		network::req req {url};
		std::string hash, form;

		hash = util::findBetween(req.body, "value=\"", " />");
		form = "data="+hash;

		req = {"https://jkanime.net/gsplay/redirect_post.php", false, form};
		hash = util::findBetween(req.header, "player.html#", "\n");
		form = "v="+hash;

		for (int i = 0; i < 4; ++i) { // La API a veces devuelve errores porque si
			req = {"https://jkanime.net/gsplay/api.php", false, form};
			Json resp = Json::parse(req.body);
			if (!resp["file"].is_null())
				return resp["file"];
			else
				continue;
		}
		return NULL;
	}
	/*============================================================*/
}
