#include "Models.h"

Animation::Animation(Vector3 position, int frameWidth, int frameHeight, Image* img, int noOfFrames, double totalPlayTimeInSeconds) : noOfFrames(noOfFrames), totalPlayTimeInSeconds(totalPlayTimeInSeconds), startTimeInSeconds(0.0), framePeriod(totalPlayTimeInSeconds / noOfFrames) {
	
	texture = new Texture2D(position, frameWidth, frameHeight, img, GL_TEXTURE9);
	texture->Init();

	totalTextureWidth = img->width;
	totalTextureHeight = img->height;
}

void Animation::Render(double currentTime) {
	texture->Draw();
}

Animation::~Animation() {
	if (texture) {
		delete texture;
		texture = nullptr;
	}
}