/****************************************************************************
Author: Lucas Vinicius dos Santos Assis
 ****************************************************************************/

#ifndef _IMAGEMANAGER_H_
#define _IMAGEMANAGER_H_

// VisualCastalia trace structure
#include "ImageTransmissionUtils.h"

using namespace std;
using namespace cv;

enum imageCompressionModes {
	JPG_MODE = 1000,
    PNG_MODE = 2000,
    BITMAP_MODE = 3000
};

class ImageManager {

	private:
		// Variables
		vector<unsigned char> localBuffer;
		stack<unsigned char> localBufferStack;
		
		// It performs VisualCastalia traces
		// ImageTransmissionTraces visualCastaliaTrace;

		// Function related with openCV - used to encode/decode the image in simulation
		vector<unsigned char> imageEncoder(string imageName, string imageFormat, bool useColoredImages);
		void imageDecoder(vector<unsigned char> buffer, string path_to_save, int sourcee);

		// Related with self generated images
		// Generate random lines to the random image
		void draw_random_lines( Mat image,
								int lineType,
								Point p1,
								Point p2,
								int NUMBER,
								string case_to_draw_str);
		// Generate random lines to the random filled circles
		void draw_random_filled_circle( Mat image,
								Point center,
								int lineType,
								int width,
								int NUMBER,
								string case_to_draw_str);
		// Generate random lines to the random rectangules
		void draw_random_rectangule (Mat image,
								Point p1,
								Point p2,
								int lineType,
								int NUMBER,
								string case_to_draw_str);
		// Return a new random color 
		Scalar get_new_color();

	public:
		// Number of the node that contains this imageManager
		int numNode;
		// The simulation time that some function of this imageManager is called
		double simulationTime;

		// It puts the image content into a buffer
		int imgBufferPush(string imageName, string imageFormat, bool useColoredImages);
		// This function returns an empty vector<uchar> if the buffer stack is already empty.
		// It returns imgPktSize cell from the image buffer array.
		vector<unsigned char> imgBufferPop(int imgPktSize);
		// Transform a valid complete buffer into an image file.
		// If it receives an image buffer with valid data, saves it
		// into an image file inside the path_to_save directory
		void storeRcvImage (vector<unsigned char> buffer, string path_to_save, int source);
		// Used to create images using OpenCv - to simulations that do not have original images to send
		void generateRandomImages(int imgHeight,
									int imgWidth, 
									string path_to_save,
									int caseToDraw);
		// This function calculates the Peak Signal-To-Noise image quality metrics
		double getPsnr(string original, string received);

};

#endif		// _IMAGEMANAGER_H_
