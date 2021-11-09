#pragma once

#include <optional>
#include <refl.hpp>

namespace cppnology::Attribute {
	struct APIMethod : refl::attr::usage::type {
		const char* name;
		const int version;
		const char* path;

		constexpr APIMethod(const char* name, int version, const char* path = "entry.cgi") :
			name(name),
			version(version),
			path(path) {}
	};
	struct AuthRequired : refl::attr::usage::type {};
	struct FileUploadMethod : refl::attr::usage::type {};

	struct TreatAsFilename : refl::attr::usage::member {};
}  // namespace cppnology::Attribute
