#ifndef EXTR_UTIL_HPP
#define EXTR_UTIL_HPP

#include <string>
#include <vector>
#include <curl/curl.h>

/*============================================================*/
namespace util {
	std::string findBetween(const std::string &str, const std::string &start, const std::string &finish);
	std::string findReplace(const std::string &str, const std::string &find, const std::string &replace);
	std::vector<std::string> split(const std::string &str, const std::string &separator);
}
/*============================================================*/
namespace network {
	class req {
	private:
		static size_t writeCallback(void *content, size_t size, size_t nmemb, std::string *data);
	public:
		req(const std::string &url, const bool redirect = false, const std::string &post_data = "");
		std::string header;
		std::string body;
	};
}
/*============================================================*/
#endif
