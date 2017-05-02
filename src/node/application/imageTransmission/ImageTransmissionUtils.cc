// Universidade Estadual de Feira de Santana
// Author: Lucas Vinicius dos Santos Assis
// Class to make traces from image transmission application

#include "ImageTransmissionUtils.h"

// Function to write data of simmulation in output file
// it tries open the visualCastalia output file and then, if successful, stores the trace
void ImageTransmissionUtils::writeTrace(int numNode, double simTime, string infoToTrace) {
	// Used to write traces into files
	ofstream trace;

	while (true) {
		// It opens an output file , with the output being appended in the end of the file
		trace.open (traceName, fstream::out | fstream::app);
		if( trace.is_open() ){
			trace << "SimTime: " << numberToString (simTime) << "\t\t";
			trace << " \tNode: " << numberToString (numNode) << "\t";
			trace << " " << infoToTrace << "\n";
			trace.close();
			break;
		}
	}
}

void ImageTransmissionUtils::writeCustomTrace(string traceFileName,
												int numNode,
												string imageSourceNumber,
												string imageSetNumber,
												string imageFormat,
												double resultToTrace) {	
	// writeLTrace(IDNODE+":"+imageSourceNumber+"-"+imageSetNumberStr+imageFormat+" | "+deliveryTime); 
	// Used to write traces into files
	ofstream trace;
	string infoToTrace;
	while (true) {
		// It opens an output file , with the output being appended in the end of the file
		trace.open (traceFileName, fstream::out | fstream::app);
		if( trace.is_open() ){
			infoToTrace = numberToString(numNode)+":"+imageSourceNumber+"-"+imageSetNumber+imageFormat+" | "+numberToString(resultToTrace);
			trace << infoToTrace << "\n";
			trace.close();
			break;
		}
	}
}

// Convertion of integers numbers to string
string ImageTransmissionUtils::numberToString(int integer){
	// Used to write traces into files
	ofstream trace;

	string charArray;
	ostringstream convert;	
	convert << integer;
	charArray = convert.str();	
	return charArray;
}

// Convertion of doubles numbers to string
string ImageTransmissionUtils::numberToString(double Double){
	// Used to write traces into files
	ofstream trace;

	string charArray;
	ostringstream convert;	
	convert << Double;
	charArray = convert.str();	
	return charArray;
}