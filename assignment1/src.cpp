#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#define SIZE 3

void main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
	FILE *fp;
	fp = fopen("color1.bmp", "rb");
	if (fp == NULL) return;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);//true color는 컬러팔레트가 필요없다.
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE *Image;
	BYTE *Output;

	if (hInfo.biBitCount == 8) {//8 -> gray color인 경우(=index color)
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE *)malloc(ImgSize);
		Output = (BYTE *)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	else {//24 -> true color인 경우
		Image = (BYTE *)malloc(ImgSize*3);
		Output = (BYTE *)malloc(ImgSize*3);
		fread(Image, sizeof(BYTE), ImgSize*3, fp);
	}
	fclose(fp);
	int W = hInfo.biWidth, H = hInfo.biHeight;


	/*영상처리-1*/
	//for (int i = 0;i < ImgSize*3;i++) {//BMP파일이기때문에 BGR순으로 나옴. //RGB가 담기기 때문에 W*H여도 3W*H로 생각하기
	//	Output[i] = Image[i];
	//}
	//for (int i = 0;i < ImgSize;i++) {//위와 같은 코드
	//	Output[i * 3] = Image[i * 3];
	//	Output[i * 3+1] = Image[i * 3+1];
	//	Output[i * 3+2] = Image[i * 3+2];
	//}
	//for (int i = 0;i < H;i++) {//이것도 똑같은 코드, 2차원으로 표현한 것
	//	for (int j = 0;j < W;j++) {
	//		Output[i*W*3 + j*3] = Image[i*W*3 + j*3];//B
	//		Output[i*W*3 + j*3+1] = Image[i*W*3 + j*3+1];//G
	//		Output[i*W*3 + j*3+2] = Image[i*W*3 + j*3+2];//R
	//	}
	//}
	//for (int j = 0;j < W;j++) {
	//	Output[H / 2 * W * 3 + j * 3] = 0;
	//	Output[H / 2 * W * 3 + j * 3 + 1] = 0;
	//	Output[H / 2 * W * 3 + j * 3 + 2] = 255;
	//}
	//double weight;
	//for (int j = 0;j < W;j++) {
	//	weight = j / (double)(W - 1);//가중평균
	//	Output[H / 2 * W * 3 + j * 3] = (1-weight)*0+weight*255;//왼쪽줄세개 -> red, 오른쪽줄세개 -> cyan
	//	Output[H / 2 * W * 3 + j * 3 + 1] = (1 - weight) * 0 + weight * 255;
	//	Output[H / 2 * W * 3 + j * 3 + 2] = (1 - weight) * 255 + weight * 0;
	//}

	/*영상처리-2*/
	double weight;
	for(int i=H/3*2;i<H;i++){
		for (int j = 0;j < W;j++) {
			weight = j / (double)(W - 1);//가중평균
			Output[i * W * 3 + j * 3] = (1 - weight) * 0 + weight * 255;//왼쪽줄세개 -> red, 오른쪽줄세개 -> cyan
			Output[i * W * 3 + j * 3 + 1] = (1 - weight) * 0 + weight * 255;
			Output[i * W * 3 + j * 3 + 2] = (1 - weight) * 255 + weight * 0;
		}
	}
	for (int i = H / 3;i < H / 3 * 2;i++) {
		for (int j = 0;j < W;j++) {
			weight = j / (double)(W - 1);//가중평균
			Output[i * W * 3 + j * 3] = (1 - weight) * 0 + weight * 255;//왼쪽줄세개 -> green, 오른쪽줄세개 -> magenta
			Output[i * W * 3 + j * 3 + 1] = (1 - weight) * 255 + weight * 0;
			Output[i * W * 3 + j * 3 + 2] = (1 - weight) * 0 + weight * 255;
		}
	}	
	for (int i = 0;i < H/3;i++) {
		for (int j = 0;j < W;j++) {
			weight = j / (double)(W - 1);//가중평균
			Output[i * W * 3 + j * 3] = (1 - weight) * 255 + weight * 0;//왼쪽줄세개 -> blue, 오른쪽줄세개 -> yellow
			Output[i * W * 3 + j * 3 + 1] = (1 - weight) * 0 + weight * 255;
			Output[i * W * 3 + j * 3 + 2] = (1 - weight) * 0 + weight * 255;
		}
	}



	fp = fopen("color_output2.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	if (hInfo.biBitCount == 8) {
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), ImgSize, fp);
	}
	else {
		fwrite(Output, sizeof(BYTE), ImgSize*3, fp);
	}
	fclose(fp);
	free(Image);
	free(Output);
}
