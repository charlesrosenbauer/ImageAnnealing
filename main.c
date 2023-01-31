#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#include "SDL2/SDL.h"


typedef struct{
	int*	samples;
	int*	sums;
}FreqTable;

FreqTable	makeFreqTable(){
	FreqTable ret;
	ret.samples	= malloc(sizeof(int) * 65536);
	ret.sums	= malloc(sizeof(int) *   256);
	for(int i = 0; i < 65536; i++) ret.samples[i] = 0;
	for(int i = 0; i <   256; i++) ret.sums   [i] = 0;
	return ret;
}

void insertFreqTable(FreqTable tab, uint8_t a, uint8_t b){
	uint16_t ix = a;
	ix = (ix << 8) | b;
	tab.samples[ix]++;
	tab.sums   [ a]++;
}

int main(int ac, char** av){
	SDL_Init(SDL_INIT_EVERYTHING);
	FreqTable	rs[6];
	FreqTable	gs[6];
	FreqTable	bs[6];
	FreqTable	rg, rb, gb;
	for(int i = 0; i < 6; i++){
		rs[i] = makeFreqTable();
		gs[i] = makeFreqTable();
		bs[i] = makeFreqTable();
	}
	rg = makeFreqTable();
	rb = makeFreqTable();
	gb = makeFreqTable();
	
	printf("%i files\n", ac-1);
	for(int i = 1; i < ac; i++){
		printf("%s\n", av[i]);
		SDL_Surface* img = SDL_LoadBMP(av[i]);
		img =  SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_ARGB8888, 0);
		
		uint32_t* pix = img->pixels;
		int        ix = 0;
		for(int j = 1; j < img->h; j++){
			for(int k = 1; k < img->w; k++){
				uint32_t a = pix[((j-1)*img->w)+k-1];
				uint32_t b = pix[((j-1)*img->w)+k  ];
				uint32_t c = pix[((j  )*img->w)+k-1];
				uint32_t d = pix[((j  )*img->w)+k  ];
				
				insertFreqTable(rs[0], a >> 16, b >> 16);
				insertFreqTable(rs[1], a >> 16, c >> 16);
				insertFreqTable(rs[2], a >> 16, d >> 16);
				insertFreqTable(rs[3], b >> 16, c >> 16);
				insertFreqTable(rs[4], b >> 16, d >> 16);
				insertFreqTable(rs[5], c >> 16, d >> 16);
				
				insertFreqTable(gs[0], a >>  8, b >>  8);
				insertFreqTable(gs[1], a >>  8, c >>  8);
				insertFreqTable(gs[2], a >>  8, d >>  8);
				insertFreqTable(gs[3], b >>  8, c >>  8);
				insertFreqTable(gs[4], b >>  8, d >>  8);
				insertFreqTable(gs[5], c >>  8, d >>  8);
				
				insertFreqTable(bs[0], a >>  0, b >>  0);
				insertFreqTable(bs[1], a >>  0, c >>  0);
				insertFreqTable(bs[2], a >>  0, d >>  0);
				insertFreqTable(bs[3], b >>  0, c >>  0);
				insertFreqTable(bs[4], b >>  0, d >>  0);
				insertFreqTable(bs[5], c >>  0, d >>  0);
				
				insertFreqTable(rg   , a >> 16, b >>  8);
				insertFreqTable(rb   , a >> 16, b >>  0);
				insertFreqTable(gb   , a >>  8, b >>  0);
			}
		}
		
		SDL_FreeSurface(img);
	}
}
