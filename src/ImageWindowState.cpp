#include "ImageWindowState.h"
#include "FilesystemAdapter.h"
#include <SOIL.h>
#include <optional>
#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <functional>
#include <tuple>
#include <cstring>

using namespace std;

static int imageID = 0;

optional<ImageWindowState> LoadImageFile(const char * filepath)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint tex_2d = SOIL_load_OGL_texture(
		filepath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);
	if (tex_2d == 0) {
		cout << SOIL_last_result() << endl;
		return nullopt;
	} else {
		glBindTexture(GL_TEXTURE_2D, tex_2d);

		int w, h;
    int miplevel = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

    unsigned char * pixels = new unsigned char[w * h * 3];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    auto v = vector<IFilter*>();
    IFilter * fil = new MainFilter(w, h);
    fil->InitShader();
    v.push_back(fil);

    ImageWindowState im = {
        tex_2d,
        w,
        h,
        1.0f,
        string(filepath).c_str(),
        imageID++,
        pixels,
        GL_RGB,
        fs::current_path().string(),
        v
    };
    return make_optional(im);
	}
}



optional<ImageWindowStateVideo> LoadVideoFile(const char* path) {

	auto it = fs::directory_iterator(path);
	size_t counter = 0;
	for (const auto &entry : it) {
		counter++;
	}

	auto names = new const char*[counter];

	size_t c = 0;
	for (const auto &entry : fs::directory_iterator(path)) {
		 names[c] = (new string(entry.path().string()))->c_str() ;
		 c++;
	}

	std::sort( names, names + counter,
			   []( const char *s1, const char *s2 ) { return std::strcmp( s1, s2 ) < 0; } );

	auto images = new GLuint[counter];
	auto data = new unsigned char * [counter];
	int w, h;

	for (size_t i = 0; i != counter; ++i) {
		cout << i << endl;
		GLuint tex_2d = images[i] = SOIL_load_OGL_texture(
				names[i],
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS
		);
		if (tex_2d == 0) {
			cout << SOIL_last_result() << endl;
			return nullopt;
		} else {
			glBindTexture(GL_TEXTURE_2D, tex_2d);


			int miplevel = 0;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

			data[i] =  new unsigned char[w * h * 3];
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
			glBindTexture(GL_TEXTURE_2D, 0);

		}


		delete  names[i];
	}

	delete[] names;

	auto v = vector<IFilter*>();
	IFilter * fil = new MainFilter(w, h);
	fil->InitShader();
	v.push_back(fil);

	ImageWindowStateVideo im =  {
			0,
			w,
			h,
			1.0f,
			string(path).c_str(),
			imageID++,
			nullptr,
			GL_RGB,
			fs::current_path().string(),
			v,
			images,
			data,
			counter
	};

	return make_optional(im);
}

optional<ImageWindowState> LoadImageFileRaw(const char * filepath, int width, int height) {
	ifstream input(filepath, ios::binary);
	vector<unsigned char> buffer(istreambuf_iterator<char>(input), {});

	unsigned char * flipBuffer = new unsigned char[width * height];

	for (int i = 0; i < height; i++) 
		for (int j = 0; j < width; j++) 
			flipBuffer[ (height-i-1) * width + j] = buffer[i * width + j];

	GLuint texture_map;
	glGenTextures(1, &texture_map);
	glBindTexture(GL_TEXTURE_2D, texture_map);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, flipBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	auto v = vector<IFilter*>();
	IFilter * fil = new MainFilter(width, height);
	fil->InitShader();
	v.push_back(fil);

	glBindTexture(GL_TEXTURE_2D, 0);

	ImageWindowState i = {
		texture_map,
		width,
		height,
		1.0f,
		"",
		imageID++,
		flipBuffer,
		GL_LUMINANCE,
		fs::current_path().string(),
		move(v)
	};
	return make_optional(i);
}

bool SaveImageFile(const char * filepath, ImageWindowState * image)
{
	glBindTexture(GL_TEXTURE_2D, image->filters[image->filters.size() - 1]->_outputTexture);

	auto buffer = new unsigned char [4 * image->height * image->width];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	SOIL_save_image(filepath, SOIL_SAVE_TYPE_BMP, image->width, image->height, 3, buffer);
	delete[] buffer;

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool ReloadImage(ImageWindowState * image) {
	glBindTexture(GL_TEXTURE_2D, image->texture());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, image->colorFormat, GL_UNSIGNED_BYTE, image->data());
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

optional<ImageWindowState> CreateImage(unsigned char * pixels, int w, int h)
{
	GLuint texture_map;
	glGenTextures(1, &texture_map);
	glBindTexture(GL_TEXTURE_2D, texture_map);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	auto v = vector<IFilter*>();
	IFilter * fil = new MainFilter(w, h);
	fil->InitShader();
	v.push_back(fil);

	ImageWindowState im = {
			texture_map,
			w,
			h,
			1.0f,
			string("Generated").c_str(),
			imageID++,
			pixels,
			GL_RGB,
			fs::current_path().string(),
			move(v)
	};
	return make_optional(im);
}

void fillBuffer(unsigned char* buffer, int w, int h,
		const function<tuple<u_char, u_char, u_char>(int, int)> &value) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			auto val = value(i, j);
			buffer[3 * (i * w + j)] = get<0>(val);
			buffer[3 * (i * w + j) + 1] = get<1>(val);
			buffer[3 * (i * w + j) + 2] = get<2>(val);
		}
	}
}
