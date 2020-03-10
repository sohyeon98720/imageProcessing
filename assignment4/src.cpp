#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#define SIZE 3

//기존 이진화 코드. Binarization_face 함수와 비교바람.
//void Binarization(BYTE * In, BYTE * Out, int Th, int W, int H)
//{
//	int Size = W * H;
//	for (int i = 0; i < Size; i++)
//	{
//		if(In[i] > Th) Out[i] = 255;
//		else Out[i] = 0;
//	}
//}


void Binarization_face(BYTE * In, BYTE * Out, int W, int H)
{
	//int Size = W * H;
	for (int i = 0;i < H;i++) {
		for (int j = 0;j < W;j++) {
			if(118+3<-0.16874*In[i*W * 3 + j * 3 + 2]-0.3313*In[i*W * 3 + j * 3 + 1]+0.500*In[i*W * 3 + j * 3]+128&&
				142+3>-0.16874*In[i*W * 3 + j * 3 + 2] - 0.3313*In[i*W * 3 + j * 3 + 1] + 0.500*In[i*W * 3 + j * 3]+128&&
				125-4<0.500*In[i*W * 3 + j * 3 + 2]-0.4187*In[i*W * 3 + j * 3 + 1]-0.0813*In[i*W * 3 + j * 3]+128&&
				154-4>0.500*In[i*W * 3 + j * 3 + 2] - 0.4187*In[i*W * 3 + j * 3 + 1] - 0.0813*In[i*W * 3 + j * 3]+128){
				Out[i*W*3 + j*3+2] = 0;
				Out[i*W*3 + j*3+1] = 0;
				Out[i*W*3 + j*3] = 0;
			}
			else {
				Out[i*W * 3 + j * 3 + 2] = 255;
				Out[i*W * 3 + j * 3 + 1] = 255;
				Out[i*W * 3 + j * 3] = 255;
			}
		}
	}
}

void drawRed(BYTE *Image,BYTE *Output, int W, int H) {//외접하는 사각형 그리기
	int up_x = 0,up_y=0;
	int down_x = 0,down_y=0;
	int left_x = 9999, left_y = 0;
	int right_x = 0,right_y=0;
	for (int i = 1;i < H-1;i++) {//그냥 이진화한 사진에 왼쪽 맨위에 픽셀값이 255여서 원래는 0부터 시작햇지만 1로 바꿈 ㅠㅠ
		for (int j = 1;j < W-1;j++) {
			if (Output[i*W * 3 + j * 3] == 255) {
				up_x = j * 3;
				up_y = i * 3;
			}
		}
	}
	for (int i = H-1;i > 0;i--) {
		for (int j = W-1;j > 0;j--) {
			if (Output[i*W * 3 + j * 3] == 255) {
				down_x = j * 3;
				down_y = i * 3;
			}
		}
	}
	printf("\nx,y of up and down = %d %d / %d %d", up_x,up_y,down_x,down_y);
	for (int i = 1;i < H - 1;i++) {
		for (int j = 1;j < W - 1;j++) {
			if (Output[i*W * 3 + j * 3] == 255 && left_x > j * 3) {
				left_x = j * 3;
				left_y = i * 3;
			}
		}
	}
	for (int i = 1;i < H - 1;i++) {
		for (int j = 1;j < W - 1;j++) {
			if (Output[i*W * 3 + j * 3] == 255 && right_x < j * 3) {
				right_x = j * 3;
				right_y = i * 3;
			}
		}
	}
	printf("\nx,y of left and right = %d %d / %d %d", left_x,left_y, right_x,right_y);
	for (int i = 0;i < H;i++) {
		for (int j = 0;j < W;j++) {
			if ((left_x < j * 3 && j * 3 < right_x)||(right_x<j*3 && j*3<left_x)) {
				if (up_y == i * 3) {
					Image[i*W * 3 + j * 3 + 2] = 255;
					Image[i*W * 3 + j * 3 + 1] = 0;
					Image[i*W * 3 + j * 3] = 0;
				}
				else if (down_y == i * 3) {
					Image[i*W * 3 + j * 3 + 2] = 255;
					Image[i*W * 3 + j * 3 + 1] = 0;
					Image[i*W * 3 + j * 3] = 0;
				}
			}
			else if ((up_y < i * 3 && i * 3 < down_y)||(down_y<i*3 && i*3<up_y)) {
				if(left_x == j * 3) {
					Image[i*W * 3 + j * 3 + 2] = 255;
					Image[i*W * 3 + j * 3 + 1] = 0;
					Image[i*W * 3 + j * 3] = 0;
				}
				else if (right_x == j * 3) {
					Image[i*W * 3 + j * 3 + 2] = 255;
					Image[i*W * 3 + j * 3 + 1] = 0;
					Image[i*W * 3 + j * 3] = 0;
				}
			}
		}
	}
}

void main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
	FILE *fp;
	fp = fopen("face.bmp", "rb");
	if (fp == NULL) return;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image;
	BYTE * Output;
	if (hInfo.biBitCount == 8) {
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE *)malloc(ImgSize);
		Output = (BYTE *)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	else { // 트루컬러인경우
		Image = (BYTE *)malloc(ImgSize*3);
		Output = (BYTE *)malloc(ImgSize*3);
		fread(Image, sizeof(BYTE), ImgSize*3, fp);
	}
	fclose(fp);
	int W = hInfo.biWidth, H = hInfo.biHeight;
	printf("%d,%d", W, H);

	Binarization_face(Image, Output, W, H);
	drawRed(Image,Output, W, H);

	fp = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	if (hInfo.biBitCount == 8) {
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), ImgSize, fp);
	}
	else {
		fwrite(Image, sizeof(BYTE), ImgSize*3, fp);
	}
	fclose(fp);
	free(Image);
	free(Output);
}
