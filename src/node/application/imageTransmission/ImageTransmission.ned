// Universidade Estadual de Feira de Santana
// Author: Lucas Vinicius dos Santos Assis
// Application based on ThrouthputTest from Castalia

package node.application.imageTransmission;

// Application functionalities - considering each node?
// 1 - Get an image from the especific "origin directory"
//		The default name of directory is "originalImages". The user can change it.
//		The default format of image is "jpg". The user can change it.
// 2 - Split each image specified by the user, according its size, into image packets.
// 3 - Send the image packets through the network to the sink node.
// 4 - Store related packets together (according the source image and source node).
// 5 - When the last packet from an image is received in sink node, the application joins
// its data and saves it in the receivedImage directory. The user can verify the integrity
// of the images after pass through the network. This process is repeated for each image

// The sensor node module. Connects to the wireless channel in order to communicate 
// with other nodes.

simple ImageTransmission like node.application.iApplication {
 parameters:
 	//=====================================================
 	// Application parameters of Castalia
 	//=====================================================
 	string applicationID = default ("ImageTransmission");
	bool collectTraceInfo = default (false);
	int priority = default (1);
	// Packet Header Overhead in bytes
	int packetHeaderOverhead = default (27);		
	// Default size of application packet in bytes
	int constantDataPayload = default (100);
	
	//=====================================================
	// Variables inherited from ThroughputTest application
	//=====================================================
	// Destination for packets produced in this node. 
	// This parameter can be used to create an
	// application-level static routing. This way we can
	// have a multi-hop image transmission
	string nextRecipient = default ("0");
	// Delay in seconds before the app starts producing packets	
	double startupDelay = default (0);
	
	//===================================================
	// Default variables of ImageTransmission simulation
	//===================================================
	// How many image sources the node must to have
	// A image source is one of the images in the originalImages folder
	int numImageSources = default (0);
	// Number of images sent to network in one second.
	double imageFrequency = default (0);
	// Default name of the source directory of the original images
	string originalImagesFolder = default ("originalImages");
	// Default name of the directory whose will storage received images from the network
	string receivedImagesFolder = default ("receivedImages");
	// Default format of the images to be sent
	string imageFormat = default ("jpg");

	// ====================================
	// Related whith self generated images
	// ====================================
	// The application generates random images
	bool selfGeneratedImg = default (false);
	// Default Height of generated random images
	int imgHeight = default (64);
	// Default Width of generated random images
	int imgWidth = default (64);
	// It defines if the images in simulation are colored
	bool useColoredImages = default(true);

	// Indicates how the app send packets, periodic (value = 1) or burst (value = 2)
	int imgPckSendingMode = default (1);

	// Get the running configuration name
	// string configurationName = default ("");

 gates:
 	output toCommunicationModule;
	output toSensorDeviceManager;
	input fromCommunicationModule;
	input fromSensorDeviceManager;
	input fromResourceManager;
}
