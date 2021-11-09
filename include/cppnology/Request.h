#pragma once

#include <type_traits>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include <refl.hpp>
#include <url.hpp>
#include <nlohmann/json.hpp>
#include "CprSerializer.h"
#include "Attributes.h"
#include "exception/CppnologyException.h"

namespace cppnology {
	template<typename MethodType, typename = std::enable_if_t<refl::is_reflectable<MethodType>()>>
	class Request {
	public:
		Request(Url baseUrl, MethodType method) : baseUrl_(baseUrl), method_(method) {}

		Request& authorize(const std::string& did, const std::string& sid) {
			did_ = did;
			sid_ = sid;
			hasAuth_ = true;
			return *this;
		}

		cpr::Response response() {
			constexpr auto methodType = refl::reflect<MethodType>();

			// Check if method type has APIMethod attribute
			static_assert(refl::descriptor::has_attribute<Attribute::APIMethod>(methodType));

			constexpr auto apiAttr =
				refl::descriptor::get_attribute<Attribute::APIMethod>(methodType);

			using namespace fmt::literals;
			const auto apiPath = "/webapi/{}"_format(apiAttr.path);
			const char* apiName = apiAttr.name;
			// TODO maybe we need to deduct version from methodType.name.c_str()
			constexpr int apiVersion = apiAttr.version;
			const char* methodName = refl::descriptor::get_simple_name(methodType).c_str();
			auto methodParams = CprSerializer::serialize(method_);

			cpr::Cookies cookies;
			if (refl::descriptor::has_attribute<Attribute::AuthRequired>(methodType) && !hasAuth_) {
				throw AuthRequired(apiName, methodName);
			}

			if (hasAuth_) {
				cookies["did"] = did_;
				injectAuth(methodParams);
			}

			return cpr::Post(cpr::Url { baseUrl_.path(apiPath).str() }, methodParams);
		}

	private:
		void injectAuth(cpr::Payload& params) {
			params.Add(cpr::Pair { "_sid", sid_ });
		}

		void injectAuth(cpr::Multipart& params) {
			params.parts.push_back(cpr::Part { "_sid", sid_ });
		}

	private:
		Url baseUrl_;
		MethodType method_;
		bool hasAuth_ = false;
		std::string did_;
		std::string sid_;
	};
}  // namespace cppnology
