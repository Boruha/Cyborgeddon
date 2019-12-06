#pragma once

#include <string>
#include <utility>

struct Renderable
{
	Renderable() = default;
	explicit Renderable(const char* mesh, const char* texture) : mesh(mesh), texture(texture) {  }
	~Renderable() = default;

	const char* mesh {};
	const char* texture {};
};

