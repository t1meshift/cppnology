#pragma once

#include <string>
#include <vector>
#include <refl.hpp>
#include <cppnology/Attributes.h>

namespace cppnology::API::Info {
	template<int V>
	struct query;

	template<>
	struct query<1> {
		std::optional<std::string> query;
	};
}  // namespace cppnology::API::Info

REFL_AUTO(type(cppnology::API::Info::query<1>, cppnology::Attribute::APIMethod("SYNO.API.Info", 1)),
		  field(query))
