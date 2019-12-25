#pragma once

struct Renderable
{
	explicit Renderable(const char* mesh, const char* texture) : mesh(mesh), texture(texture) {  }

	const char* mesh { };
	const char* texture { };
};

