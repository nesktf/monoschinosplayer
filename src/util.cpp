#include <util.hpp>

namespace util {
	/*============================================================*/
	std::string findBetween(const std::string &str, const std::string &start, const std::string &finish) {
		int head = 0, end = 0;

		head = str.find(start, head);
		end = str.find(finish, head);
		head += start.length();

		return str.substr(head, end-head);
	}
	std::string findReplace(const std::string &str, const std::string &find, const std::string &replace) {
		std::string tmp = str;
		size_t pos = 0;

		while ((pos = tmp.find(find, pos)) != std::string::npos) {
			tmp.replace(pos, find.length(), replace);
			pos += replace.length();
		}

		return tmp;
	}
	std::vector<std::string> split(const std::string &str, const std::string &separator) {
		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;

		while((pos = str.find(separator, pos)) != std::string::npos) {
			std::string substring(str.substr(prev_pos, pos-prev_pos));
			output.push_back(substring);
			pos += separator.size();
			prev_pos = pos;
		}

		output.push_back(str.substr(prev_pos, pos-prev_pos)); // Last word

		return output;
	}
	/*============================================================*/
}
namespace network {
	/*============================================================*/
	req::req(const std::string &url, const bool redirect, const std::string &post_data) {
		auto curl = curl_easy_init();

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
			curl_easy_setopt(curl, CURLOPT_REFERER, url.c_str());

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->body);

			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, this->writeCallback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, &this->header);
			
			if (redirect)
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			if (post_data.length() > 0)
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

			curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
	}
	/*============================================================*/
	size_t req::writeCallback(void *content, size_t size, size_t nmemb, std::string *data) {
		data->append((char*) content, size * nmemb);
		return size * nmemb;
	}
	/*============================================================*/
}
