#pragma once

#include <string>
#include <vector>
#include <optional>
#include <refl.hpp>
#include <cppnology/Attributes.h>

namespace cppnology::API::Auth {
	template<int V>
	struct login;

	template<>
	struct login<6> {
		enum class format { cookie, sid };

		std::string account;
		std::string passwd;
		std::optional<std::string> session;
		std::optional<format> format;

		// TODO implement OTP support and its fields for the method:
		// otp_code, enable_device_token, device_name, device_id
	};
}  // namespace cppnology::API::Auth

REFL_AUTO(type(cppnology::API::Auth::login<6>, cppnology::Attribute::APIMethod("SYNO.API.Auth", 6)),
		  field(account),
		  field(passwd),
		  field(session),
		  field(format))
