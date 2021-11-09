#pragma once

#include <stdexcept>
#include <string>

namespace cppnology {
	struct CppnologyException : public std::exception {
		const char* what() const noexcept override;

	protected:
		std::string message_;
	};

	struct UnknownError : public CppnologyException {
		UnknownError();
	};

	struct NASNotFound : public CppnologyException {
		NASNotFound(std::string quickConnectId);

		std::string quickConnectId;
	};

	struct NASOffline : public CppnologyException {
		NASOffline();
	};

	struct AuthRequired : public CppnologyException {
		AuthRequired(std::string api, std::string method);

		std::string api;
		std::string method;
	};
}  // namespace cppnology
