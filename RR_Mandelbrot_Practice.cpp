// RR_Mandelbrot_Practice.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <fstream>
#include <istream>
#include <iostream>
#include <string>

using namespace std;

/*
while (x*x + y*y < 2 * 2 AND iteration < max_iteration)
{
	xtemp = x*x - y*y +x0
		y = 2 * x*y + y0
		x = xtemp
		iteration = iteration + 1
}
*/

int findMandelbrot(double cr, double ci, int max_iterations)
{
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while(i < max_iterations && zr * zr + zi * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}

	return i;
}

double mapToReal(int x, int imageWidth, double minR, double maxR)
{
	double range = maxR - minR;
	//[0, width]
	//[0, maxR = minR] - val * range / width
	//[minR, maxR] - last step + minR
	return x * (range / imageWidth) + minR;
}

double mapToImaginary(int y, int imageHeight, double minI, double maxI)
{
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;
}

int main()
{
	//Get the required input values from the file
	ifstream fin("input.txt");
	int imageWidth, imageHeight, maxN;
	double minR, maxR, minI, maxI;
	if (!fin)
	{
		cout << "Could not open the file" << endl;
		cin.ignore();
		return 0;
	}
	fin >> imageWidth >> imageHeight >> maxN;
	fin >> minR >> maxR >> minI >> maxI;
	fin.close(); //Not necessary, good practice though

	//Open the output file, write the PPM header
	ofstream fout("output_image.ppm");
	fout << "P3" << endl; //"Magic Number" - PPM file
	fout << imageWidth << " " << imageHeight << endl; //Dimensions of image
	fout << "256" << endl; //Max value of a pixel R,G,B value

	//For every pixel
	for (int y = 0; y < imageHeight; y++) //Rows
	{
		for (int x = 0; x < imageWidth; x++) //Pixels in row
		{
			//Find the real and imaginary values for c, corresponding
			//to that x,y pixel in the image
			double cr = mapToReal(x, imageWidth, minR, maxR);
			double ci = mapToImaginary(y, imageHeight, minI, maxI);
			
			//Find the number of iterations in the Mandelbrot formula
			//using said c
			int n = findMandelbrot(cr, ci, maxN);

			//Map the resulting number to an RGB value
			int r = ((int)(n * sinf(n)) % 256); //Change for more interesting colors!
			int g = ((int)(n * n * sinf(n)) % 256); //Change for more interesting colors!
			int b = ((int)(n * cosf(n)) % 256); //Change for more interesting colors!

			//Output it to the image
			fout << r << " " << g << " " << b << " ";
		}
		fout << endl;
	}
	fout.close();
	cout << "Finished" << endl;
	return 0;
}