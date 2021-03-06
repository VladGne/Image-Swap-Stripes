// ACGL1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <omp.h>  
#include <opencv2\opencv.hpp>
using namespace cv;

	void _tmain()
	{
		String images[4] = { "C:\\testImg\\Lambert.jpg", "C:\\testImg\\fox.jpg", "C:\\testImg\\castle.jpg", "C:\\testImg\\aoiogata.jpg" };	// Картинки
		int fileAns = 0;

		// Спрашиваем номер картинки
		printf("Enter file number: \n1. Small - Lambert (650x650)\n2. Average - Fox (1191x670)\n3. Big - Castle (1920x1080)\n4. Large - Knight (4960x2984)\n");	
		scanf_s("%d", &fileAns);

		int coloeAns = 1;

		// Спрашиваем номер цветового режима
		printf("Enter image color mode number: \n1. Grayscale\n2. In colors\n");
		scanf_s("%d", &coloeAns);
		coloeAns--;

		// Читаем картинку и её размеры
		Mat defultimage = imread(images[fileAns-1], coloeAns);
		Mat image = defultimage;
		int height = image.rows;
		int width = image.cols;
		int pixels;
		int cnt = 0;		

		// Спрашиваем высоту полоски
		printf("Input row size in pixels:\n");
		scanf_s("%d", &pixels);
		printf("*Work in process...*");
		double start;
		double end;
		start = omp_get_wtime();

		// Замена пикселей
		for (int y = 0; y < height -pixels; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (coloeAns > 0)
				{	// Берём пару пикселей для замены
					Vec3b TpmPixel = image.at<Vec3b>(Point(x, y));				
					Vec3b ChangePixel = image.at<Vec3b>(Point(x, y + pixels));
					// Меняем цвета
					image.at<Vec3b>(Point(x, y)) = ChangePixel;
					image.at<Vec3b>(Point(x, y + pixels)) = TpmPixel;
				}

				else
				{
					char TpmPixel = image.at<char>(Point(x, y));
					char ChangePixel = image.at<char>(Point(x, y + pixels));

					image.at<char>(Point(x, y)) = ChangePixel;
					image.at<char>(Point(x, y + pixels)) = TpmPixel;
				}							
			}
				
			cnt++; // Кол-во пройденных строк
			if (cnt == pixels)	// Когда заменили полоску
			{	// Делаем скачек для следующей полоски
				y += cnt;
				cnt = 0;
			}
		}

		end = omp_get_wtime();
		printf_s("Work took: %f sec. \n", end - start);

		// Подгоняем картинку под размер монитора
		Mat resizedImage;
		resize(image, resizedImage, Size(1920, 1080));

		// Сохраняем и показываем картинку
		imwrite("C:\\New Image.jpg", resizedImage);
		imshow("New Image", resizedImage);
		waitKey(0);
	}