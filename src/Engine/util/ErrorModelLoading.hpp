#pragma once

#include <Engine/util/Error.hpp>

struct ErrorModelLoading {
	ErrorModelLoading(const std::string_view _message) : message(_message) { }

	void operator()() {
		error("Error al cargar la malla: " + message);
	}

	std::string message;
};