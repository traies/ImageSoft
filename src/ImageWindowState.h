#pragma once 
#include <optional>
#include <glad/glad.h>  // Initialize with gladLoadGL()

struct ImageWindowState {
	GLuint texture;
	int width, height;
	float zoom;
	const char * filename;
};

std::optional<ImageWindowState> LoadImageFile(const char * filepath);