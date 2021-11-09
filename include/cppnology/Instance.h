#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <url.hpp>
#include <cpr/cpr.h>
#include "QuickConnect.h"
#include "Request.h"

namespace cppnology {
	class Instance {
	public:
		Instance(const Url& url, const std::string& account = "", const std::string& password = "");
		Instance(Url&& url, const std::string& account = "", const std::string& password = "");
		Instance(QuickConnect&& qc, const std::string& account = "", const std::string& password = "");

		Instance& authorize(const std::string& account, const std::string& password);

		template<typename MethodType>
		nlohmann::json call(MethodType method) {
			auto request = Request(url_, method);
			if (authorized_) {
				request.authorize(did_, sid_);
			}
			cpr::Response response = request.response();

			return nlohmann::json::parse(response.text);
		}

	private:
		Url url_;
		bool authorized_ = false;
		std::string did_;
		std::string sid_;
	};
}  // namespace cppnology