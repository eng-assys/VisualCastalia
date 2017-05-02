// Universidade Estadual de Feira de Santana
// Author: Lucas Vinicius dos Santos Assis
// Class to make traces from image transmission application

#ifndef _IMAGETRANSMISSION_UTILS_H_
#define _IMAGETRANSMISSION_UTILS_H_

#include "Dependencies.h"

using namespace std;

// Default name of output file
const string traceName = "Visual-Castalia-Trace.txt";

class ImageTransmissionUtils
{
 private:

 public:
 	// Function to write data of simmulation in output file
	static void writeTrace(int numNode, double simTime, string infoToTrace);
	// Funtions to convert numbers to strings
	static string numberToString(int integer);
	static string numberToString(double Double);
	static void writeCustomTrace(string traceFileName,
									int numNode,
									string imageSourceNumber,
									string imageSetNumber,
									string imageFormat,
									double resultToTrace);

};

#endif				// _IMAGETRANSMISSION_UTILS_H_
