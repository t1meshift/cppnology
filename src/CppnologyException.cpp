#include <cppnology/exception/CppnologyException.h>
#include <fmt/format.h>

namespace cppnology {
	const char* CppnologyException::what() const noexcept {
		return message_.c_str();
	}

	UnknownError::UnknownError() {
		message_ = "An unknown error has occured";
	}

	NASNotFound::NASNotFound(std::string quickConnectId) : quickConnectId(quickConnectId) {
		message_ = fmt::format("NAS with QuickConnect ID {} was not found", quickConnectId);
	}

	NASOffline::NASOffline() {
		message_ = "NAS is offline or does not exist";
	}

	AuthRequired::AuthRequired(std::string api, std::string method) : api(api), method(method) {
		message_ = fmt::format("Authorization is required for {} method \"{}\"", api, method);
	}
}  // namespace cppnology
