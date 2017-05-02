#include "ImageManager.h"

int ImageManager::imgBufferPush(string imageName, string imageFormat, bool useColoredImages) {
	localBuffer = imageEncoder(imageName, imageFormat, useColoredImages);
	for (int i = localBuffer.size() - 1; i >=0 ; i--) {
		localBufferStack.push( localBuffer[i] );
	}

	return localBuffer.size();
}

vector<unsigned char> ImageManager::imgBufferPop(int imgPktSize) {
	vector<unsigned char> auxLocalBuffer;
	int i = 0;
	while( i < imgPktSize && !localBufferStack.empty() ){
		auxLocalBuffer.push_back( localBufferStack.top() );
		localBufferStack.pop();
		i++;
	}

	ImageTransmissionUtils::writeTrace( numNode, 
						simulationTime,
						"Get a fragment of buffer of "+ImageTransmissionUtils::numberToString( int(auxLocalBuffer.size()) )+ " bytes");
	return auxLocalBuffer;
}

void ImageManager::storeRcvImage (vector<unsigned char> buffer, 
									string path_to_save, 
									int source) {
	imageDecoder(buffer, path_to_save, source);
}

//						==============================
// 						FUNCTIONS RELATED WITH OPENCV
//						==============================

// This function encode an image and returns the generated buffer
vector<unsigned char> ImageManager::imageEncoder(string imageName, string imageFormat, bool useColoredImages){
	vector<unsigned char> buffer;
	Mat image;
	// Windows bitmaps - *.bmp, *.dib (always supported)
	// JPEG files - *.jpeg, *.jpg, *.jpe (see the Notes section)
	// JPEG 2000 files - *.jp2 (see the Notes section)
	// Portable Network Graphics - *.png (see the Notes section)
	if ( !useColoredImages ){
		image = imread(imageName.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	}
	else {
		image = imread(imageName.c_str(), IMREAD_UNCHANGED);
	}
	
	// Check for invalid input
	if( image.empty() ) {
        string infoToTrace;
        infoToTrace =  "Could not open or find the image files at simulation directory, path: " + imageName;
        ImageTransmissionUtils::writeTrace( numNode, 
        								simulationTime,
					 					infoToTrace);
    }
	else {
		//namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    	//imshow( "Display window", image );                // Show our image inside it.
    	//waitKey(1); // Wait for a keystroke in the window
		
		int compressionMode;		
    	if(imageFormat == ".jpeg" || imageFormat == ".jpg" || imageFormat == ".jpe" || imageFormat == ".jp2")
    		compressionMode = JPG_MODE;
    	else if (imageFormat == ".png")
    		compressionMode = PNG_MODE;
    	else if (imageFormat == ".bmp" || imageFormat == ".dib")
    		compressionMode = BITMAP_MODE;

    	vector<int> compression_param = vector<int>(2);
		int flag;		
		int compressionLevel;
		string imageFormat;

		switch(compressionMode){
			case JPG_MODE: {
				// Max quality of compression - almost no compression
				flag = IMWRITE_JPEG_QUALITY;
				compressionLevel = 100;
				imageFormat = ".jpg";
				break;
			}
			case BITMAP_MODE: {
				imageFormat = ".bmp";
				break;
			}
			case PNG_MODE: {
				// For PNG, it can be the compression level from 0 to 9. A higher value means a 
				// smaller size and longer compression time. Default value is 3.
				imageFormat = ".png";
				flag = IMWRITE_PNG_COMPRESSION;
				compressionLevel = 0;
				break;
			}
			default: {

			}
		}
		compression_param[0] = flag;
		compression_param[1] = compressionLevel;

		// The function imencode compresses the image and stores it in the memory buffer
		// that is resized to fit the result. See cv::imwrite for the list of supported 
		// formats and flags description.

		imencode(imageFormat.c_str(), image, buffer, compression_param);
		ImageTransmissionUtils::writeTrace( numNode, 
										simulationTime,
										"Buffer to image: " +
										imageName +
										" created - size: "+
										ImageTransmissionUtils::numberToString( int(buffer.size()) ) );
	}
		return buffer;
}

void ImageManager::imageDecoder(vector<unsigned char> buffer, 
								string path_to_save, 
								int source){
	// Decode buffer into an image
	// The function reads an image from the specified buffer in the memory.
	// If the buffer is too short or contains invalid data, the empty matrix/image is returned.
	Mat decodedImage = imdecode( Mat(buffer), IMREAD_UNCHANGED );

	if( decodedImage.empty() ) {
		ImageTransmissionUtils::writeTrace( numNode, 
										simulationTime,
					 					"Fail in decoding process of buffer from image: "+path_to_save);
	}
	else{
		// This function saves the decoded image
		bool isSaved = imwrite( path_to_save.c_str(), decodedImage );
		
		if (isSaved){
			// Writing trace to indicate when the image is saved
			ImageTransmissionUtils::writeTrace( numNode, 
											simulationTime,
									 		"Saving image from source node #"+
									 		ImageTransmissionUtils::numberToString(source)+
									 		" in: "+path_to_save);
		}
		else{
			ImageTransmissionUtils::writeTrace( numNode, 
											simulationTime,
						 					"Received image is corrupted and cannot be saved in: "+path_to_save);
		}
	}

}

void ImageManager::generateRandomImages(int imgHeight, 
										int imgWidth, 
										string path_to_save, 
										int caseToDraw){
	// Creating an empty image frame
	Mat image(imgHeight, imgWidth, CV_8UC3);
	// Creating the opposite points to position the inner rectangle,
	// according the dimensions of image
	Point pt1, pt2;
	
	// Type of line of the geometry
	int lineType = 8;
	// when negative, it indicates that the geometry is completely filled
	int thickness = -1;
	
	// Coloring the image according RGB codification
	image = get_new_color();
	
	// Number of random elements to generate
	int NUMBER = 10;

	// Converting the case number to string to put it like inner text in the generated image
	// ostringstream text_number;	
	// text_number << caseToDraw;
	string case_to_draw_str = ImageTransmissionUtils::numberToString(caseToDraw);

	if(caseToDraw == 0){
		// Creating a rectangle inside the image
		pt1.x = -imgWidth/2;
		pt1.y = -imgHeight/2;

		pt2.x = imgWidth/2 - 30;
		pt2.y = imgHeight/2 - 20 ; 
		
		// Drawing the number of case in the generated image
		thickness = 3;
		putText(image, case_to_draw_str, pt2, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), thickness, 8, false);	

		pt2.x = imgWidth/2;
		pt2.y = imgHeight/2;
		
		for (int i = 0; i < NUMBER; i++){
			rectangle( image, pt2, pt1, get_new_color(), thickness, lineType );
			pt2.x += i;
			pt2.y += i;
		}

		
	}
	else{
		pt1.x = -imgWidth;
		pt1.y = -imgHeight;
		pt2.x = imgWidth;
		pt2.y = imgHeight;
		if (caseToDraw == 1){
			draw_random_lines( image, lineType, pt1, pt2, NUMBER, case_to_draw_str);
		}
		else if (caseToDraw % 2 == 0){
			draw_random_filled_circle(image, pt1, lineType, imgWidth, NUMBER, case_to_draw_str);
		}
		else if (caseToDraw % 3 == 0){
			draw_random_lines( image, lineType, pt1, pt2, NUMBER, case_to_draw_str);
			draw_random_filled_circle(image, pt1, lineType, imgWidth, NUMBER, case_to_draw_str);	
		}
		else if (caseToDraw % 4 == 0){
			draw_random_rectangule (image, pt1, pt2, lineType, NUMBER, case_to_draw_str);
		}
		else if (caseToDraw % 5 == 0){
			draw_random_lines( image, lineType, pt1, pt2, NUMBER, case_to_draw_str);
			draw_random_filled_circle(image, pt1, lineType, imgWidth, NUMBER, case_to_draw_str);
			draw_random_rectangule (image, pt1, pt2, lineType, NUMBER, case_to_draw_str);
		}
	} 
	
	// Setting the png parameters to save the image in this format
    vector<int> compression_params;
    // Type of compression
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    // Compression level - 3: default value
    compression_params.push_back(100);

    // Save the generated images
	imwrite( path_to_save, image, compression_params );	
    
    ImageTransmissionUtils::writeTrace( numNode, simulationTime,
					 "Image: " + path_to_save + " generated");
}

// ============ DRAW RANDOM GENERATED ELEMENTS TO SELF GENERATED IMAGES

// Get a new random color - RGB format
Scalar ImageManager::get_new_color(){
 	// RGB codification to set different colors to each image case
 	int Red, Blue, Green;
 	// The function rand() generates numbers between 0 and 255 in this case
 	Blue = (rand() % 255);
    Green = (rand() % 255);
    Red = (rand() % 255);
 	// Coloring the image according RGB codification
 	return Scalar(Green, Blue, Red);
}

// Draw random lines in inserted image
void ImageManager::draw_random_lines( Mat image, 
										int lineType,
										Point p1,
										Point p2,
										int NUMBER,
										string case_to_draw_str) {
	Point pt1, pt2;

	for( int i = 0; i < NUMBER; i++ ) {
		pt1.x = rand() % p1.x;
		pt1.y = rand() % p1.y;
		pt2.x = rand() % p2.x;
		pt2.y = rand() % p2.y;

		line( image, pt1, pt2, get_new_color(), (rand() % 10), lineType );
		putText(image, case_to_draw_str, pt1, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), (rand() % 5), 8, false);	
	}
}

// Draw random filled circles in inserted image
void ImageManager::draw_random_filled_circle( Mat image,
												Point center,
												int lineType,
												int width,
												int NUMBER,
												string case_to_draw_str) {
	Point pt1;
	for( int i = 0; i < NUMBER; i++ ) {
		pt1.x = rand() % center.x;
		pt1.y = rand() % center.y;
		circle( image, pt1, width/32.0, get_new_color(), (rand() % 10), lineType );
		putText(image, case_to_draw_str, pt1, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), (rand() % 5), 8, false);	
	}
}
// Draw random retangles in inserted image
void ImageManager::draw_random_rectangule (Mat image,
											Point p1,
											Point p2,
											int lineType,
											int NUMBER,
											string case_to_draw_str) {

	Point pt1, pt2;
	for( int i = 0; i < NUMBER; i++ ) {
		pt1.x = rand() % p1.x;
		pt1.y = rand() % p1.y;
		pt2.x = rand() % p2.x;
		pt2.y = rand() % p2.y;
		rectangle( image, pt1, pt2, get_new_color(), (rand() % 10), lineType);
		putText(image, case_to_draw_str, pt1, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), (rand() % 5), 8, false);	
	}
}

// This function calculates the Peak Signal-To-Noise image quality metrics
double ImageManager::getPsnr(string original, string received){
	Mat image_source;
	Mat image_received;
	double psnr_opencv;

	// Loading pictures
	image_source = imread(original, CV_LOAD_IMAGE_GRAYSCALE);
	image_received = imread(received, CV_LOAD_IMAGE_GRAYSCALE);

	if( !image_source.data ){
		ImageTransmissionUtils::writeTrace( numNode, 
										simulationTime,
					 					"PSNR Metrics: could not open or find the original image: "+original);
		return -1;
	}
	if( !image_received.data ){
		ImageTransmissionUtils::writeTrace( numNode, 
										simulationTime,
					 					"PSNR Metrics: could not open or find the received image: "+received);
		return -2;
	}

	try {
		psnr_opencv = PSNR(image_source, image_received);
		ImageTransmissionUtils::writeTrace( numNode, 
							simulationTime,
					 		"PSNR between "+original+" and "+received+": "+ImageTransmissionUtils::numberToString(psnr_opencv));
	}
	catch( cv::Exception& e ) {
		const char* err_msg = e.what();
		string message = err_msg;
		ImageTransmissionUtils::writeTrace( numNode, 
											simulationTime,
									 		"Exception on PSNR calculation: "+message);
		return -99;
	}

	
	return psnr_opencv;
}