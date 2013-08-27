/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Demo.h"
#include <MAUtil/Vector.h>
using namespace MAUtil;

static Vector<Effect*> sEffects;
static int sEffectIndex = 0;

void Surface::blit(Surface *surf, int x, int y, int flags) {
	int imageWidth = surf->w;
	int imageHeight = surf->h;
	int surfaceWidth = this->w;
	int surfaceHeight = this->h;
	byte* img = surf->pixels;
	byte* backBuffer = pixels;

	int topLeftX = x-(imageWidth>>1);
	int topLeftY = y-(imageHeight>>1);
	int bottomRightX = x+(imageWidth>>1);
	int bottomRightY = y+(imageHeight>>1);
	int realWidth = imageWidth;
	int realHeight = imageHeight;

	if(topLeftX>surfaceWidth) return;
	if(topLeftY>surfaceHeight) return;
	if(bottomRightX<0) return;
	if(bottomRightY<0) return;

	int offsetX = 0;
	int offsetY = 0;

	if(topLeftX<0) {
		offsetX = -topLeftX;
		topLeftX = 0;
		realWidth -= offsetX;
	}

	if(topLeftY<0) {
		offsetY = -topLeftY;
		topLeftY = 0;
		realHeight -= offsetY;
	}

	if(offsetX>=imageWidth)
		return;
	if(offsetY>=imageHeight)
		return;

	if(bottomRightX>=surfaceWidth)
		realWidth -= (bottomRightX - surfaceWidth);

	if(bottomRightY>=surfaceHeight)
		realHeight -= (bottomRightY - surfaceHeight);

	if(realWidth<=0) return;
	if(realHeight<=0) return;

	img += offsetX + offsetY * imageWidth;
	backBuffer += topLeftX + topLeftY * surfaceWidth;

	switch(flags&0x1) {
	case BLIT_REPLACE:
		for(int j = 0; j < realHeight; j++) {
			for(int i = 0; i < realWidth; i++) {
				backBuffer[i] = img[i];
			}
			img += imageWidth;
			backBuffer += surfaceWidth;
		}
		break;
	case BLIT_ADD:
		for(int j = 0; j < realHeight; j++) {
			for(int i = 0; i < realWidth; i++) {
				int c = (int) backBuffer[i] + (int) img[i];
				if(c>255) c = 255;
				backBuffer[i] = (byte)c;
			}
			img += imageWidth;
			backBuffer += surfaceWidth;
		}
		break;
	}
}


float startTime = 0.0;

void addEffect(Effect* effect) {
	sEffects.add(effect);
}

void setEffectIndex(int i) {
	if(i<0) i = 0;
	if(i>=sEffects.size()) i = sEffects.size()-1;
	sEffectIndex = i;
	if(!sEffects.size()) return;
	startTime = maGetMilliSecondCount()*0.001;
	sEffects[sEffectIndex]->prepare();
}

int getEffectIndex() {
	return sEffectIndex;
}

void initEffects(Surface* surf) {
	for(int i = 0; i < sEffects.size(); i++) {
		sEffects[i]->init(surf);
	}
	setEffectIndex(0);
}

void renderEffect(Surface* surface) {
	if(sEffects.size()==0) return;
	sEffects[sEffectIndex]->render(surface, (float)maGetMilliSecondCount()*0.001f - startTime);
}
