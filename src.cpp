#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#define SIZE 3


void Binarization(BYTE * In, BYTE * Out, int Th, int W, int H)
{
	int Size = W * H;
	for (int i = 0; i < Size; i++)
	{
		if(In[i] > Th) Out[i] = 255;
		else Out[i] = 0;
	}
}
int DetermThreshold(BYTE *Img, int W, int H) {
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
	}
	return threshold_new;
}


void LowPassFilter(BYTE * Image, BYTE * Output, double* m, 
	const int S, int W, int H)
{
	//const int Size = S;
	int Margin = SIZE / 2;
	double Mask[SIZE][SIZE];
	for (int i = 0; i < SIZE*SIZE; i++) 
		Mask[i / S][i%S] = m[i];
	double temp = 0.0;
	for (int i = Margin; i < H - Margin; i++) { // 마스크 중앙의 세로방향 이동
		for (int j = Margin; j < W - Margin; j++) { // 마스크 중앙의 가로방향 이동
			for (int m = -Margin; m <= Margin; m++) { // 마스크 중앙 기준 세로방향 주변화소 접근
				for (int n = -Margin; n <= Margin; n++) { // 마스크 중앙 기준 가로방향 주변화소 접근
					temp += (Image[(i + m)*W + (j + n)] * Mask[m + Margin][n + Margin]);
				}
			}
			Output[i*W + j] = (BYTE)temp;
			temp = 0.0;
		}
	}
}
void HighPassFilter(BYTE * Image, BYTE * Output, int* m,
	const int S, int W, int H)
{
	int Margin = SIZE / 2;
	double Mask[SIZE][SIZE];
	for (int i = 0; i < SIZE*SIZE; i++)
		Mask[i / SIZE][i%SIZE] = m[i];
	int temp = 0;
	for (int i = Margin; i < H - Margin; i++) { // 마스크 중앙의 세로방향 이동
		for (int j = Margin; j < W - Margin; j++) { // 마스크 중앙의 가로방향 이동
			for (int m = -Margin; m <= Margin; m++) { // 마스크 중앙 기준 세로방향 주변화소 접근
				for (int n = -Margin; n <= Margin; n++) { // 마스크 중앙 기준 가로방향 주변화소 접근
					temp += (Image[(i + m)*W + (j + n)] * Mask[m + Margin][n + Margin]);
				}
			}
			Output[i*W + j] = abs(temp)/4;
			temp = 0;
		}
	}
}

void main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
	FILE *fp;
	fp = fopen("LENNA.bmp", "rb");
	if (fp == NULL) return;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Output = (BYTE *)malloc(ImgSize);
	BYTE * save = (BYTE *)malloc(ImgSize);
	BYTE * save2 = (BYTE *)malloc(ImgSize);
	BYTE * save3 = (BYTE *)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	/* 영상처리 */

	int Threshold = 0;
	int m[SIZE*SIZE] = { -1, -2, -1,
						0, 0, 0,
						1, 2, 1 };
	HighPassFilter(Image, save, m, SIZE, hInfo.biWidth, hInfo.biHeight);
	int n[SIZE*SIZE] = { -1,0,1,
					-2,0,2,
					-1,0,1 };
	HighPassFilter(Image, save2, n, SIZE, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0;i < ImgSize;i++) {
		if (save[i] >=  save2[i]) {
			save3[i] = save[i];
		}
		else save3[i] = save2[i];
	}
	Threshold=DetermThreshold(save3, hInfo.biWidth, hInfo.biHeight);
	Binarization(save3, Output, Threshold, hInfo.biWidth, hInfo.biHeight);

	/* 영상처리 */
	fp = fopen("output___.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output);
	free(save);
	free(save2);
	free(save3);
}