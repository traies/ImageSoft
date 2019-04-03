#pragma once
#include <glad/glad.h>
#include <filesystem>

void InitVertexBuffer();

/**
 * Every filter does the following:
 * * Initializes the shader.
 * * Keeps track of necessary state for each shader.
 * * Render UI controls.
 *
 **/
struct IFilter {
	int _width;
	int _height;

	char * _name;
	GLuint _programID;
	GLuint _outputFramebuffer;
	GLuint _outputTexture;
	virtual void InitShader() = 0;
	virtual void RenderUI() = 0;
	virtual GLuint ApplyFilter(GLuint prevTexture) = 0;
};

struct MainFilter: IFilter {
	GLuint _textureSampler;

	MainFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Main";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct SingleBandFilter: IFilter {
	GLuint _textureSampler;
	GLuint _glBand;
	int _band = 0;

	SingleBandFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "SingleBand";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct SubstractionFilter: IFilter {
	GLuint _textureSampler;
	GLuint _secondTex;
	GLuint _secondSampler;
	GLuint _factor;
	GLuint _glMin, _glMax;
	bool _subtract;
	float _min[3] = {0}, _max[3] = {1, 1, 1};
	std::string _path;
	
	SubstractionFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Substraction";
		_path = std::filesystem::current_path().string();
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct NegativeFilter: IFilter {
	GLuint _textureSampler;
	NegativeFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Negative";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct ScalarFilter: IFilter {
	GLuint _textureSampler;
	GLuint _factorGl;
	float _factor;

	ScalarFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Scalar";
		_factor = 1;
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct DynamicRangeCompressionFilter: IFilter {
	GLuint _textureSampler;
	GLuint _glC;
	unsigned char _minr, _ming, _minb, _maxr, _maxg, _maxb;

	bool _calcToggle = true;
	DynamicRangeCompressionFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Dynamic range compression";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct ThresholdFilter: IFilter {
	GLuint _textureSampler;
	GLuint _glThreshold;
	float _threshold[3] = {0.5f, 0.5f, 0.5f};
	ThresholdFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Threshold";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct ContrastFilter: IFilter {
	GLuint _textureSampler;
	GLuint _glLower, _glLowerTo;
	GLuint _glHigher, _glHigherTo;

	float _lower[3] = {0, 0, 0};
	float _lowerTo[3] = {0, 0, 0};
	float _higher[3]  = {1, 1, 1};
	float _higherTo[3]  = {1, 1, 1};

	ContrastFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Contrast";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct EqualizationFilter: IFilter {
	GLuint _textureSampler;
	GLuint _eqSampler;
	GLuint _eqTexture;
	bool _eqCalcTexture = true;

	EqualizationFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Equalization";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct ExponentialNoiseFilter: IFilter {
	GLuint _textureSampler;
	GLuint _randomTex;
	GLuint _randomSampler;
	GLuint _glLambda;
	GLuint _glContamination;
	int _seed = 0;
	float _lambda = 2;
	float _contamination = 0.1;
	ExponentialNoiseFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Exponential Noise Filter";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct RayleighNoiseFilter: IFilter {
	GLuint _textureSampler;
	GLuint _randomTex;
	GLuint _randomSampler;
	GLuint _glXi;
	GLuint _glContamination;
	int _seed = 0;
	float _xi = 0.1;
	float _contamination = 0.1;
	RayleighNoiseFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Rayleigh Noise Filter";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct GaussianNoiseFilter: IFilter {
	GLuint _textureSampler;
	GLuint _randomTex;
	GLuint _randomSampler;
	GLuint _randomTex2;
	GLuint _randomSampler2;
	GLuint _glSigma;
	GLuint _glMu;
	GLuint _glContamination;
	float _contamination = 0.1;
	int _seed[2] = {0, 100};
	float _sigma = 1;
	float _mu = 0;
	GaussianNoiseFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Gaussian Noise Filter";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct SaltAndPepperNoiseFilter: IFilter {
	GLuint _textureSampler;
	GLuint _randomTex;
	GLuint _randomSampler;
	GLuint _glContamination1;
	GLuint _glContamination2;

	float _contamination1 = 0.01, _contamination2 = 0.09;
	int _seed = 0;
	
	SaltAndPepperNoiseFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Salt And Pepper Noise Filter";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
};

struct MeanFilter: IFilter {
	GLuint _textureSampler;
	GLuint _glMaskSize;
	GLuint _glWidth, _glHeight;
	GLuint _glMaskSampler;
	GLuint _maskWeightsTexture;
	GLuint _glMaskDivision;

	int _maskSize = 3;
	float _maskDivision = 9;
	
	MeanFilter(int w, int h) {
		_width = w;
		_height = h;
		_name = "Mean Filter";
	}

	void InitShader() override;
	void RenderUI() override;
	GLuint ApplyFilter(GLuint prevTexture) override;
	void InitMask();
};