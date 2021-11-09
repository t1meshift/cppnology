#pragma once

#include <optional>
#include <refl.hpp>
#include <magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "Attributes.h"

namespace cppnology::CprSerializer {
	template<typename T,
			 typename = std::enable_if_t<std::is_arithmetic_v<std::remove_reference_t<T>>>>
	std::optional<std::string> serializeValue(const T& value) {
		if constexpr (std::is_arithmetic_v<T>) {
			return std::to_string(value);
		} else if constexpr (std::is_enum_v<T>) {
			return magic_enum::enum_name(value);
		}
	}

	std::optional<std::string> serializeValue(const std::string& value) {
		return value;
	}

	std::optional<std::string> serializeValue(const nlohmann::json& value) {
		return value.dump();
	}

	std::optional<cpr::File> serializeValue(const std::string& value, Attribute::TreatAsFilename) {
		return cpr::File { value };
	}

	template<typename T>
	auto serializeValue(const std::optional<T>& value) {
		return value.has_value() ? serializeValue(value.value()) : std::nullopt;
	}

	template<typename MethodType,
			 bool HasFile = refl::descriptor::has_attribute<Attribute::FileUploadMethod>(
				 refl::reflect<MethodType>()),
			 typename ReturnType = std::conditional_t<HasFile, cpr::Multipart, cpr::Payload>>
	ReturnType serialize(MethodType method) {
		constexpr auto type = refl::reflect(method);

		static_assert(refl::descriptor::has_attribute<Attribute::APIMethod>(type));

		ReturnType result {};

		constexpr auto apiAttr = refl::descriptor::get_attribute<Attribute::APIMethod>(type);
		const auto apiName = apiAttr.name;
		const auto apiVersion = std::to_string(apiAttr.version);
		const auto methodName = refl::descriptor::get_simple_name(type).c_str();

		if constexpr (std::is_same_v<ReturnType, cpr::Payload>) {
			result.Add(cpr::Pair { "api", apiName });
			result.Add(cpr::Pair { "version", apiVersion });
			result.Add(cpr::Pair { "method", methodName });
		} else if constexpr (std::is_same_v<ReturnType, cpr::Multipart>) {
			result.parts.push_back(cpr::Part { "api", apiName });
			result.parts.push_back(cpr::Part { "version", apiVersion });
			result.parts.push_back(cpr::Part { "method", methodName });
		}

		for_each(refl::reflect(method).members, [&](auto member) {
			auto value = serializeValue(member(method));
			if (!value.has_value()) {
				return;
			}
			if constexpr (std::is_same_v<ReturnType, cpr::Payload>) {
				result.Add(cpr::Pair { member.name.c_str(), value.value() });
			} else if constexpr (std::is_same_v<ReturnType, cpr::Multipart>) {
				result.parts.push_back(cpr::Part { member.name.c_str(), value.value() });
			}
		});
		return result;
	}
}  // namespace cppnology::CprSerializer
