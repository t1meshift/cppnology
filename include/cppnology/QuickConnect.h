#pragma once

#include <queue>
#include <set>
#include <string>
#include <utility>
#include <url.hpp>

namespace cppnology {
	class QuickConnect {
	public:
		QuickConnect(const std::string& id);
		Url findURL();

	private:
		std::set<Url> getDirectUrls();
		Url getTunnelUrl(const std::string& controlHostName);
		bool isHostAvailable(Url host);

	private:
		std::string id_;
		std::string controlHostName_;
	};
}  // namespace cppnology
