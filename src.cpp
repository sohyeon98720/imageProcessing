#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include<iostream>
#include<fstream>
#include<string>
#include "src.h"
#pragma warning (disable:4996)

using namespace std;

void ObtainHisto(BYTE * Img, int * Histo, int W, int H)
{
	int Size = W * H;
	for (int i = 0; i < Size; i++)
		Histo[Img[i]]++;
	FILE * fp3 = fopen("Histo.txt", "wt");
	for (int i = 0; i < 256; i++)
		fprintf(fp3, "%d\t%d\n",i,Histo[i]);	
	fclose(fp3);
}
void Binarization(BYTE * In, BYTE * Out, int Th, int W, int H)
{
	int Size = W * H;
	for (int i = 0; i < Size; i++)
	{
		if(In[i] > Th) Out[i] = 255;
		else Out[i] = 0;
	}
}

int DetermThreshold(BYTE *Img, int *Histo,int W, int H) {
	int Size = W * H;
	int max = Img[0];
	int min = Img[0];	
	int threshold = 0;
	int threshold_new = 0;
	int G1 = 0;
	int G11 = 1;
	int G2 = 0;
	int G22 = 1;
	int average1 = 0;
	int average2 = 0;
	for (int i = 0;i < Size;i++) {
		if (max < Img[i]) {
			max = Img[i];
		}
	}
	for (int i = 0;i < Size;i++) {
		if (min > Img[i]) {
			min = Img[i];
		}
	}
	int average = (max + min) / 2;
	printf("초기 임계값은 : %d\n", average);
	threshold = average;

	for (int i = 0;i < Size;i++) {
		if (Img[i] > threshold) {
			G1 += Img[i];
			G11++;
		}
		else {
			G2 += Img[i];
			G22++;
		}
	}
	average1 = G1 / G11;
	average2 = G2 / G22;
	threshold_new = (average1 + average2) / 2;
	printf("갱신 임계값은 : %d\n", threshold_new);

	while (threshold - threshold_new > 3 || threshold - threshold_new < -3) {
		threshold = threshold_new;
		G1 = 0;G2 = 0;
		G11 = 1;G22 = 1;
		for (int i = 0;i < Size;i++){
			if (Img[i] > threshold) {
				G1 += Img[i];
				G11++;
			}
			else {
				G2 += Img[i];
				G22++;
			}
		}
		average1 = G1 / G11;
		average2 = G2 / G22;
		threshold_new = (average1 + average2) / 2;
		printf("갱신 임계값은 : %d\n", threshold_new);
	}
	return threshold_new;
}


void main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
	FILE *fp;
	fp = fopen("coin.bmp", "rb");
	if (fp == NULL) return;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Output = (BYTE *)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	/* 영상처리 */
	int Histo[256] = { 0 };
	int total_Threshold = DetermThreshold(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainHisto(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	Binarization(Image, Output, total_Threshold, hInfo.biWidth, hInfo.biHeight);

	



	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output);
}