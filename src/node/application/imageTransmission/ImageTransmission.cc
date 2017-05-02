// Universidade Estadual de Feira de Santana
// Author: Lucas Vinicius dos Santos Assis
// Application based on ThrouthputTest from Castalia

// Application functionalities?
// 1 - Get an image from the especific origin directory
//		The default name of directory is "originalImages". The user can change it.
//		The default format of image is "jpg". The user can change it.
// 2 - Split each image specified by the user according its size into image packets.
// 3 - Send the image packets through the network to the sink node.
// 4 - Store related packets together (according the source image and source node).
// 5 - When the last packet from an image is received, the application joins this data and saves
//		it in the receivedImage directory. The user can verify the integrity of the 
//		images after pass through the network. This process is repeated for each image

#include "ImageTransmission.h"

Define_Module(ImageTransmission);

void ImageTransmission::startup()
{
	// Get the number of images sources from omnetpp.ini simulation file
	numImageSources = par("numImageSources");
	// Get the next recipient to send packets from omnetpp.ini simulation file
	// Using for static routing
 	nextRecipient = par("nextRecipient").stringValue();
 	// Get the next recipient to send packets from omnetpp.ini simulation file
 	imageFrequency = par("imageFrequency");
	
	// Set the format of the images sources
	imageFormat = ".";
	imageFormat += par("imageFormat").stringValue();

	// Set the folder to store the received images in the sink node
	receivedImagesFolder = string( par("receivedImagesFolder").stringValue() ) +"/";
	// Different configurations are stored into different folders
	// receivedImagesFolder += string( par("configurationName").stringValue() ) +"/";
	

	// Set the current num node in the image manager - used for traces
	imageManager.numNode = int( ( unsigned short ) self);
	sinkNodeManager.numNode = int( ( unsigned short ) self);

	// Verifying if some node is sink or a normal node
	// normal nodes send images
	if (imageFrequency > 0 && numImageSources > 0 && 
		nextRecipient.compare(SELF_NETWORK_ADDRESS) != 0){
			// How many images were sent by a single 
			howManyImagesSent = 0;
			// Set the size of image packets, in bytes
			imgPckSize = par("constantDataPayload");
			// Set the size of the header of image packets, in bytes
	 		imgPckHeaderSize = par("packetHeaderOverhead");
	 		// Set Delay to start the application in the node
	 		startupDelay = par("startupDelay");
			// Set how style to send packets -> 1 - periodic | 2 - burst
			imgPckSendingMode = par("imgPckSendingMode");

			// Set the image source folder if I am not sending self generated images
			originalImagesFolder = string( par("originalImagesFolder").stringValue()) +"/";
			
			// ============ RELATED WITH SELFGENERATEDIMAGES
			// If true, the application must to generate the images sources
			selfGeneratedImg =  par("selfGeneratedImg");
			// It works if selfGeneratedImg is true. Set the image height of generated images
			imgHeight = par("imgHeight");
			// It works if selfGeneratedImg is true. Set the image width of generated images
			imgWidth = par("imgWidth");
			// It works if selfGeneratedImg is true.
			// If true the generated image is colored, otherwise it is grey scale.
			useColoredImages =  par("useColoredImages");
			//==============
			
			// Initializing the variable that stores how many image packet the app in all the simulation
			currSentSampleSN = 0;
			// Initializing the current image number. That means, the number of current image source.
			currentImageSource = -1;
			// Initializing the order of creation of the sent image
			imageSetNumber = -1;

			// Set the spacing among images according to the imageFrequency
			image_spacing = imageFrequency > 0 ? 1/imageFrequency : -1;

			// Traces the following info in VisualCastaliaTrace: int numNode, double simTime, string infoToTrace
			ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 SIMTIME_DBL( simTime() ),
							 "Startup ");

			// In case of self gerating images, the application must generate the images sources
			if(selfGeneratedImg){

				// By default, the generated images are jpeg images
				imageFormat = ".jpg";
				
				for (int drawNum = 0; drawNum < numImageSources; drawNum++){
					// Conversion of the number of random image source from integer to string
					string drawNumStr = ImageTransmissionUtils::numberToString(drawNum);
					// Associate the new default image name with the current number of random image source
					currentImageName = originalImagesFolder + "auto_" + drawNumStr + imageFormat;
					// Generate one image, according the "drawNum" case
					imageManager.simulationTime = SIMTIME_DBL ( simTime() );
					imageManager.generateRandomImages(imgHeight,
										 			imgWidth,
										 			currentImageName.c_str(),
										 			drawNum);
				}
				
			}
			// It starts the abstraction of camera in the application (considering the startup delay)
			setTimer(GET_A_NEW_IMAGE, startupDelay + image_spacing );
	}
	else {
		// This function checks if there are the source folders of image files
		// in the simulation root folder
		if (int( ( unsigned short) self ) == 0){
			// Erasing latency trace file
			int sysStatus = system( "rm Latency-Trace.txt" );
			sysStatus = system( "rm Psnr-Trace.txt" );
			trace() << "Sink node";
			ImageTransmissionUtils::writeTrace( int( ( unsigned short) self ),
							 SIMTIME_DBL( simTime() ), "Sink node");
			folderCheck();
		}
		else {
			trace() << "I am not sending images";
			ImageTransmissionUtils::writeTrace( int( ( unsigned short) self ),
							 SIMTIME_DBL( simTime() ),
							 "I am not sending images");	
		}
		
	}
	// This method declares a new castalia output by its name.
	declareOutput("Image Packets received per node");

}

void ImageTransmission::timerFiredCallback(int index) 
{
	switch (index) {
		case GET_A_NEW_IMAGE: {
			// Increasing the number of the current image, to get the next image.
			currentImageSource++;

			if( currentImageSource < numImageSources ){
				// Increasing the set number, to put the correct delivery order to the next image
				imageSetNumber++;
				// Reset the current image packet number
				currentImagePacket = 0;
				// Setting the name of the current image
				string currentImagestr = ImageTransmissionUtils::numberToString(currentImageSource);

				if(selfGeneratedImg)
					currentImageName = originalImagesFolder + "auto_" + currentImagestr + imageFormat;
				else
					currentImageName = originalImagesFolder + currentImagestr + imageFormat;

				// Get the moment when the simulation image is taken
				currentSnapshotTime = simTime();

				ImageTransmissionUtils::writeTrace( int( ( unsigned short) self ),
							 SIMTIME_DBL( currentSnapshotTime ),
							 "Getting a new image from image Source: "+currentImageName);

				// Store the image file into the memory buffer
				imageManager.simulationTime = SIMTIME_DBL ( simTime() );
				int bufferSize = imageManager.imgBufferPush(currentImageName, imageFormat, useColoredImages);

				// It makes the calculus to discover how many image packets must to be sent
				double totalImgPkt = bufferSize/float(imgPckSize);
				int totImgPkt = totalImgPkt;
				if( (totalImgPkt - totImgPkt) > 0 ) {
					howManyImagePkts =	totImgPkt + 1;
				}
				else {
					howManyImagePkts = 	totImgPkt;
				}
				// Spacing among packets according the current image size and the image_spacing
				packet_spacing = howManyImagePkts > 0 ? image_spacing / float(howManyImagePkts) : -1;
				// Avoid buffer overflow error
				if (packet_spacing < max_packet_spacing){
					packet_spacing = max_packet_spacing;
				}
				// Update how many image were sent
				howManyImagesSent++;

				// It Defines the sending packet type, burst or periodic. Each one have distinct characteristics
				if (imgPckSendingMode == SEND_PERIODIC_IMG_PKT){
					setTimer(SEND_PERIODIC_IMG_PKT, packet_spacing);
				}
				else if (imgPckSendingMode == SEND_BURST_IMG_PKT){
					setTimer(SEND_BURST_IMG_PKT, max_packet_spacing);
				}
				else{
					opp_error ("Invalid number to imgPckSendingMode parameter. Use 1 to Periodic or 2 to Burst");
					ImageTransmissionUtils::writeTrace( int( ( unsigned short) self ),
							 						SIMTIME_DBL( simTime() ), 
							 						"Invalid number to imgPckSendingMode parameter. Use 1 to Periodic or 2 to Burst");
				}
			}
			else {
				// If the current image number is greater than the number of images sources
				// Set the currentImage number to -1, and than restart the count
				currentImageSource = -1;
				setTimer(GET_A_NEW_IMAGE, image_spacing);
			}
			break;
		}
		case SEND_PERIODIC_IMG_PKT: {
			// Get a fragment of the buffer to store it into an image packet and send it to the sink node
			imageManager.simulationTime = SIMTIME_DBL ( simTime() );
			vector<unsigned char> bufferFragment = imageManager.imgBufferPop( imgPckSize );
			
			if( bufferFragment.size() != 0 ){
				if (imgPckSendingMode == SEND_PERIODIC_IMG_PKT)
					trace() << "Periodic Sending: Total number of imgPkt sent: " << currSentSampleSN;

				else
					trace() << "Burst Sending: Total number of imgPkt sent: " << currSentSampleSN;
				trace() << "Sending imgPkt #"<< currentImagePacket << " from the image #" << currentImageName;

				// This function creates an image packet with the buffer fragment, returned from imgBufferPop,
				// of the current image being sent by the node
				createImgPck(bufferFragment);
				// Sending a new image package (until drain out the buffer that contains the current image)
				setTimer(SEND_PERIODIC_IMG_PKT, packet_spacing);
			}
			else {
				trace() << "GET A NEW IMAGE";
				// When the buffer of current image is emptied, a new image is taken from the next image source
				setTimer(GET_A_NEW_IMAGE, image_spacing);
			}
			break;
		}

		case SEND_BURST_IMG_PKT:{
			setTimer(SEND_PERIODIC_IMG_PKT, max_packet_spacing);
			break;
		}
	}
}

// This fuction creates an image packet and send it to the sink node
void ImageTransmission::createImgPck(vector<unsigned char> bufferFragment){
	// Creating a new image packet
	ImagePacket *imagePacket = new ImagePacket("Image Packet created", APPLICATION_PACKET);
	// Set the sequence number of packet, according all sent packet from this node
	imagePacket->setSequenceNumber(currSentSampleSN);
	// Set the size of array to store the buffer fragment of the current image source int he packet
	imagePacket->setBufferFragmentArraySize(bufferFragment.size());
	for(int i = 0; i < bufferFragment.size(); i++){
		// Fulfil the buffer of the packet with the buffer fragment of the current image source
		imagePacket->setBufferFragment(i, bufferFragment[i]);
	}
	// Creating the struct of header of image packet
	ImagePacketHeader header;
	// Set the current buffer fragment size
	header.bufferSize = bufferFragment.size();
	// Set the number of node. This parameter allows identify the packets from this node in sink node
	header.nodeID = (unsigned short)self;
	// Set the current position in X axis of this node. Currently it is not used.
	header.locX = mobilityModule->getLocation().x;
	// Set the current position in Y axis of this node. Currently it is not used.
	header.locY = mobilityModule->getLocation().y;
	// Store how many image pkts must to be sent to deliver an image to sink
	header.totalImgPackets = howManyImagePkts;
	// Number of the current image packet
	header.imagePktNumber = currentImagePacket;
	// The "setNumber" indicates the order of creation of an image in the current node
	header.setNumber = imageSetNumber;
	// Set the simulation time whose the image was taken by the node
	header.currentSnapshotTime = currentSnapshotTime;
	// The number of image source that generated the image of the packet
	header.imageSourceNumber = currentImageSource;
	// The source folder of original images - used to find the original image to compute PSNR
	header.originalImagesFolder = originalImagesFolder;
	// Put the headerInformations into image packet
	imagePacket->setImgPktHeader(header);
	// Send to the next recipient the image packet created
	//toNetworkLayer(imagePacket, par("nextRecipient"));
	toNetworkLayer(imagePacket, nextRecipient.c_str());

	// Updating total number of packet sent
	currSentSampleSN++;
	// Updating image packet from current image
	currentImagePacket++;

	
}

// This virtual method is mandatory for the new app module to define. It is called when a packet
// is received from the communication stack. Its arguments include the packet itself, the RSSI
// and LQI values for this packet (measured by the radio), as well as the network source address
// the packet originated (a network address is a string),
void ImageTransmission::fromNetworkLayer(ApplicationPacket * rcvPacket, const char *source, double rssi, double lqi) 
{	
	// Get the sequence number of received image packet
	int sequenceNumber = rcvPacket->getSequenceNumber();
	// It creates a image packet to store the information of the received image packet
	ImagePacket *imagePacket = check_and_cast<ImagePacket*>(rcvPacket);
	// It creates a image packet header to store the information of the header from the received image packet
	ImagePacketHeader header = imagePacket->getImgPktHeader();

	// When the node is sink node this "if" condition is fulfilled
	if ( nextRecipient.compare(SELF_NETWORK_ADDRESS) == 0 ) {

		// Creates the folder to store images from nodes that do not have receivedImages folder yet
		folderCheck( int( header.nodeID ) );

		trace() << "Received image packet #" << sequenceNumber << " from node " << source;
		ImageTransmissionUtils::writeTrace( int( ( unsigned short) self ),
							 			SIMTIME_DBL( simTime() ), 
							 			"Received image packet #"+ImageTransmissionUtils::numberToString(sequenceNumber)+" from node "+string(source));

		// It takes the image identification which buffer fragment belongs
		// string imageID = string(header.imageID.c_str());
		// Temp vector to store the buffer fragment in the sink node
		vector<unsigned char> buffFragment;
		// It gets the buffer fragment size
		int sizeOfBuffer = imagePacket->getBufferFragmentArraySize();
		// Storing the content of buffer fragment into auxiliary vector
		for(int i = 0; i < sizeOfBuffer; i++){
			buffFragment.push_back( imagePacket->getBufferFragment(i) );
		}

		// Time of function for traces
		sinkNodeManager.simulationTime = SIMTIME_DBL ( simTime() );
		// Receiving an image packet. If the received packet is duplicated returns false to avoid
		// receive duplicated information in sink node data base
		// bool isNotDuplicated = 

		// Receiving an image packet in sink node - store its information in a buffer to that specific node
		sinkNodeManager.receiveImagePacket (header.nodeID,
											header.locX,
											header.locY,
											buffFragment,
											header.imagePktNumber,
											header.totalImgPackets,
											header.setNumber);

		map<int, lastPacketInfo>::iterator it;
		it = sinkNodeManager.lastPacketEachNode.find( int(header.nodeID) );
		
		// Set the information received by the last packet from the current nodeID
		if( it != sinkNodeManager.lastPacketEachNode.end() ){
					// Verifying if the image buffer is complete
			if ( sinkNodeManager.isANewImageSource ( int(header.nodeID), header.setNumber) ){

				// Set the identification of the image, according the number of source node and its setNumber
				// to store correctly in corresponding received image folder
				string nodeIDStr = ImageTransmissionUtils::numberToString( it->first );
				string imageSourceNumber = ImageTransmissionUtils::numberToString(it->second.imageSourceNumber);
				string imageSetNumberStr = ImageTransmissionUtils::numberToString(it->second.setNumber);

				string path_to_save = receivedImagesFolder + nodeIDStr + "/" + imageSourceNumber + "-" + imageSetNumberStr + imageFormat;

				trace() << "Saving image " << path_to_save << " from node " << header.nodeID;

				sinkNodeManager.simulationTime = SIMTIME_DBL ( simTime() );
				// From sink node data base, gets all the fragmented data of a single image and groups it in "buffer"
				vector<unsigned char> buffer = sinkNodeManager.getCompleteBuffer(it->first,
																				it->second.setNumber,
																				it->second.bufferSize,
																				it->second.totalImgPackets);
				imageManager.simulationTime = SIMTIME_DBL ( simTime() );
				// Decodification and storage of an image buffer into an image file
				imageManager.storeRcvImage (buffer, path_to_save, int(header.nodeID));

				// Trace of the time to delivery a single image
				double deliveryTime = it->second.receivementSnapshotTime -  it->second.currentSnapshotTime;
				//string IDNODE = ImageTransmissionUtils::numberToString( int(header.nodeID) );
				ImageTransmissionUtils::writeCustomTrace("Latency-Trace.txt",
															int(header.nodeID),
															imageSourceNumber,
															imageSetNumberStr,
															imageFormat,
															deliveryTime);

				string original_image_path = it->second.originalImagesFolder+imageSourceNumber+imageFormat;
				double psnr_trace = imageManager.getPsnr(original_image_path, path_to_save);

				ImageTransmissionUtils::writeCustomTrace("Psnr-Trace.txt",
															int(header.nodeID),
															imageSourceNumber,
															imageSetNumberStr,
															imageFormat,
															psnr_trace);

				//writeLTrace(IDNODE+":"+imageSourceNumber+"-"+imageSetNumberStr+imageFormat+" | "+deliveryTime); 
			}

			// it.second.nodeID = int(header.nodeID);
			it->second.imageSourceNumber = header.imageSourceNumber;
			it->second.setNumber = header.setNumber;
			it->second.receivementSnapshotTime = sinkNodeManager.simulationTime;
			it->second.bufferSize = header.bufferSize;
			it->second.totalImgPackets = header.totalImgPackets;
			it->second.currentSnapshotTime = SIMTIME_DBL(header.currentSnapshotTime);
			it->second.originalImagesFolder = header.originalImagesFolder.c_str ();
		}
		else {
			lastPacketInfo tmp;
			// tmp.nodeID = int(header.nodeID);
			tmp.imageSourceNumber = header.imageSourceNumber;
			tmp.setNumber = header.setNumber;
			tmp.receivementSnapshotTime = sinkNodeManager.simulationTime;
			tmp.bufferSize = header.bufferSize;
			tmp.totalImgPackets = header.totalImgPackets;
			tmp.currentSnapshotTime = SIMTIME_DBL(header.currentSnapshotTime);
			tmp.originalImagesFolder = header.originalImagesFolder.c_str ();
			sinkNodeManager.lastPacketEachNode.insert( pair<int, lastPacketInfo>( int(header.nodeID) , tmp) );
		}
		
	// Packet has to be forwarded to the next hop recipient
	// only if the sender is different of nodeId in the packet
	// } else if( header.nodeID != (unsigned short)self ) {
	// } else if( sinkNodeManager.isNotDuplicatedPacket(header.nodeID, header.setNumber, header.imagePktNumber) ) {
	} else  {
		trace() << "Passing the imgPkt #" << sequenceNumber << " from node " << source;
		ImageTransmissionUtils::writeTrace( int( ( unsigned short) self ),
							 			SIMTIME_DBL( simTime() ), 
							 			"Passing the imgPkt #"+ImageTransmissionUtils::numberToString(sequenceNumber)+" from node "+string(source));
		//collectOutput("Resent packets per node", atoi(source));
		ImagePacket *fwdImgPacket = imagePacket->dup();
		// Reset the size of the packet, otherwise the app overhead will keep adding on
		fwdImgPacket->setByteLength(imgPckSize);
		// Resent the packet to the network
		toNetworkLayer(fwdImgPacket, nextRecipient.c_str());
	}
	collectOutput("Image packets received per node", atoi(source));
}

// Function called for each node in the end of the simulations
// Performs the final trace of sink node
void ImageTransmission::finishSpecific() 
{
	if( int( ( unsigned short ) self ) == 0 ){
		
		ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 			SIMTIME_DBL( simTime() ),
							 			"\n\n\t\t\t=== End of Simulation ===");	

		for(map<int,int>::iterator it_int = sinkNodeManager.howManyImgReceived.begin(); it_int != sinkNodeManager.howManyImgReceived.end(); it_int++) {
			ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 				SIMTIME_DBL( simTime() ),
							 				"Number of Images received from node # "+ImageTransmissionUtils::numberToString(it_int->first)+": "+ImageTransmissionUtils::numberToString(it_int->second));
		}

		ImageTransmissionUtils::writeTrace( 0, 0.0, "\n\n\t\t\t=== Informations from nodes ===");	
	}
	else{
		ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 SIMTIME_DBL( simTime() ),
							 "Number of Images Sent: "+ImageTransmissionUtils::numberToString(howManyImagesSent));
	}
}

// This function checks if there are the source folders of image files
// in the simulation root folder. If so, removes the deprecated and created a new one
void ImageTransmission::folderCheck() {
	// When the node is sink is necessary verify if the receivedImages folder
	// is created. The folder is used to store the received images from the
	// network
	int systemStatus;
	// First look for the receivedImage folder from the root of simulation
	// Then remove the folder that can have ancient results from another simulations
	string removeDir = "rm -rf " + receivedImagesFolder;
	systemStatus = system( removeDir.c_str() );
	ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 SIMTIME_DBL( simTime() ),
							 "Removing deprecated receivedImages folder, system status: " + ImageTransmissionUtils::numberToString(systemStatus));

	// Last step - create a new receivedImages folder to store the results of the new simulation
	string addDir = "mkdir -p " + receivedImagesFolder;
	systemStatus = system( addDir.c_str() );

	ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 SIMTIME_DBL( simTime() ),
							 "Adding new receivedImages folder, system status: " + ImageTransmissionUtils::numberToString(systemStatus));
}

// This function creates new folders related with the set of image received from each different nodes
void ImageTransmission::folderCheck(int nodeID){
	
	set<int>::iterator it;
	it = sinkNodeManager.nodeIDList.find( nodeID );

	if ( it == sinkNodeManager.nodeIDList.end() ){
		int systemStatus;
		// Each node must create the subfolder int the receivedImages folder to the sink node 
		// store the received image from this singular sender node
		string addNodeDir = "mkdir -p " + receivedImagesFolder;
		addNodeDir += ImageTransmissionUtils::numberToString(nodeID);
		systemStatus = system( addNodeDir.c_str() );

		ImageTransmissionUtils::writeTrace( int( ( unsigned short ) self ),
							 SIMTIME_DBL( simTime() ),
							 "Adding new receivedImages/nodeID folder, system status: " + ImageTransmissionUtils::numberToString(systemStatus));

		sinkNodeManager.nodeIDList.insert(nodeID);
	}
}

// Related with latency traces
// void ImageTransmission::writeLTrace(string infoToTrace) {
// 	latencyTrace.open("Latency-Trace.txt", fstream::out | fstream::app);
// 	latencyTrace << infoToTrace << "\n";
// 	latencyTrace.close();
// }

// This method processes a received carrier sense interrupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void ImageTransmission::handleRadioControlMessage(RadioControlMessage *radioMsg) {
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
        	break;
	}
}
