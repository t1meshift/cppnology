#include <cppnology/Instance.h>
#include <cppnology/API/Auth/login.h>

namespace cppnology {
	Instance::Instance(const Url& url, const std::string& account, const std::string& password) :
		url_(url) {
		if (!account.empty()) {
			authorize(account, password);
		}
	}
	Instance::Instance(Url&& url, const std::string& account, const std::string& password) :
		url_(std::move(url)) {
		if (!account.empty()) {
			authorize(account, password);
		}
	}
	Instance::Instance(QuickConnect&& qc, const std::string& account, const std::string& password) :
		url_(std::move(qc).findURL()) {
		if (!account.empty()) {
			authorize(account, password);
		}
	}

	Instance& Instance::authorize(const std::string& account, const std::string& password) {
		// TODO make ability to set custom session name
		nlohmann::json response = call(API::Auth::login<6> {
			account, password, "DownloadStation", API::Auth::login<6>::format::cookie });

		// TODO make a parser for errors (response["success"].get<bool>() == false)
		// TODO parser for responses too
		did_ = response["data"]["did"].get<std::string>();
		sid_ = response["data"]["sid"].get<std::string>();
		authorized_ = true;
		return *this;
	}
}  // namespace cppnology
