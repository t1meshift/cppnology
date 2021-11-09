#include <cppnology/QuickConnect.h>
#include <cppnology/exception/CppnologyException.h>
#include <cpr/cpr.h>
#include <fmt/format.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool operator<(const Url& a, const Url& b) {
	return a.str() < b.str();
}

namespace cppnology {
	static const std::string QUICKCONNECT_GLOBAL_HOST = "global.quickconnect.to";

	QuickConnect::QuickConnect(const std::string& id) : id_(id) {}

	Url QuickConnect::findURL() {
		// First, let's try to connect with direct URLs
		auto directUrls = getDirectUrls();
		for (const auto& url : directUrls) {
			if (isHostAvailable(url)) {
				return url;
			}
		}

		// Now try to obtain a tunnel
		// It takes a looooong time (~10 seconds), so this is last resort
		auto tunnelUrl = getTunnelUrl(controlHostName_);
		if (isHostAvailable(tunnelUrl)) {
			return tunnelUrl;
		}

		throw NASOffline();
	}

	std::set<Url> QuickConnect::getDirectUrls() {
		json requestJson = { { "version", 1 },
							 { "command", "get_server_info" },
							 { "id", "dsm_portal_https" },
							 { "serverID", id_ } };

		auto quickConnectGlobal =
			Url().scheme("https").host(QUICKCONNECT_GLOBAL_HOST).path("/Serv.php");

		auto response = cpr::Post(cpr::Url { quickConnectGlobal.str() },
								  cpr::Header { { "Content-Type", "application/json" } },
								  cpr::Body { requestJson.dump() });

		auto responseJson = json::parse(response.text);

		// TODO check errno
		switch (responseJson["errno"].get<int>()) {
			case 0:
				break;
			case 4:
				throw NASNotFound(id_);
			default:
				throw UnknownError();
		}

		controlHostName_ = responseJson["env"]["control_host"].get<std::string>();

		std::set<Url> entries;

		// The first deduction is to use local addresses of the NAS
		uint16_t localPort = responseJson["service"]["port"].get<uint16_t>();
		for (const auto& candidateJson : responseJson["smartdns"]["lan"]) {
			auto candidateHost = candidateJson.get<std::string>();

			Url candidate;
			candidate.scheme("https").host(candidateHost);
			if (localPort != 0) {
				candidate.port(localPort);
			}

			entries.emplace(candidate);
		}

		// The second one is to use external IP of the NAS
		uint16_t externalPort = responseJson["service"]["ext_port"].get<uint16_t>();
		std::string externalHost = responseJson["smartdns"]["external"].get<std::string>();

		Url externalUrl;
		externalUrl.scheme("https").host(externalHost);
		if (externalPort != 0) {
			externalUrl.port(externalPort);
		}

		entries.emplace(externalUrl);

		return entries;
	}

	Url QuickConnect::getTunnelUrl(const std::string& controlHostName) {
		json requestJson = { { "version", 1 },
							 { "command", "request_tunnel" },
							 { "id", "dsm_portal_https" },
							 { "serverID", id_ } };

		auto quickConnectUrl = Url().scheme("https").host(controlHostName).path("/Serv.php");

		auto response = cpr::Post(cpr::Url { quickConnectUrl.str() },
								  cpr::Header { { "Content-Type", "application/json" } },
								  cpr::Body { requestJson.dump() });

		auto responseJson = json::parse(response.text);

		// TODO check errno

		uint16_t relayPort = responseJson["service"]["relay_port"].get<uint16_t>();
		std::string relayHost = responseJson["service"]["relay_dualstack"].get<std::string>();

		return Url().scheme("https").host(relayHost).port(relayPort);
	}

	bool QuickConnect::isHostAvailable(Url host) {
		cpr::Response pingPongResponse =
			cpr::Get(cpr::Url { host.path("/webman/pingpong.cgi").str() });

		auto response = json::parse(pingPongResponse.text);

		return response["boot_done"].get<bool>();
	}
}  // namespace cppnology
