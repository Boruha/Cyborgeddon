#pragma once

#include <string>

namespace Sun {
	struct OutOfRangeException : public std::exception {
		template<typename T>
		explicit OutOfRangeException(std::string type, T min, T max) {
			message = std::move(type);
			message += " value is out of range (" + std::to_string(min) + " - " + std::to_string(max);
		}

		[[nodiscard]] const char *what() const noexcept override {
			return message.c_str();
		}

		std::string message;
	};
}