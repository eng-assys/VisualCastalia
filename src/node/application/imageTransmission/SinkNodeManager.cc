// Author: Lucas Vinicius dos Santos Assis
// Universidade Estadual de Feira de Santana

#include "SinkNodeManager.h"

// When a duplicated packet is received returns false
void SinkNodeManager::receiveImagePacket (unsigned short nodeID,
											double locX,
											double locY,
											vector<unsigned char> bufferFragment,
											int imgPckNumber,
											int totalImagePkts,
											int setNumber){

	string key = getImgBufferKey (nodeID, setNumber, imgPckNumber);

	ImageTransmissionUtils::writeTrace( numNode, simulationTime, "Received ImgPck - Key: " + key);
	imagePacketBuffer.insert(pair< string, vector<unsigned char> >(key, bufferFragment));

	// if( isNotDuplicatedPacket(nodeID, setNumber, imgPckNumber) ){
	// 	visualCastaliaTrace.writeTrace( numNode, simulationTime, "Received ImgPck - Key: " + key);
	// 	imagePacketBuffer.insert(pair< string, vector<unsigned char> >(key, bufferFragment));
	// 	return true;
	// }
	// else{
	// 	visualCastaliaTrace.writeTrace( numNode, simulationTime, "Received repeated ImgPck - Key: " + key);
	// 	return false;
	// }	
}

// Return true if the received packet is the last one from the same image source
bool SinkNodeManager::isLastPacket (int totalImgPackets, int imagePktNumber){
	// Definition of the last imgPacket
	if( imagePktNumber == totalImgPackets-1 ){
		return true;
	}
	return false;
}

// Return true if the packet received is the last valid packet of the same image source
// When the sink receives another packet from the same node, but from the next image source
// It is necessary generate the image from the before received image packets

bool SinkNodeManager::isANewImageSource (int nodeID, int setNumber){
	map<int, lastPacketInfo>::iterator it;
	it = lastPacketEachNode.find( nodeID );
	if( (it != lastPacketEachNode.end()) && (it->second.setNumber != setNumber ) ) {
		return true;
	}
	return false;
}

// From sink node data base, gets all the fragmented data of a single image and groups it in "buffer"
vector<unsigned char> SinkNodeManager::getCompleteBuffer(unsigned short nodeID,
												int setNumber, 
												int bufferSize, 
												int totalImagePkts){
	vector<unsigned char> buffer;
	map<string, vector <unsigned char> >::iterator it;

	string key;
	for (int i = 0; i < totalImagePkts; i++){
		key = getImgBufferKey (nodeID, setNumber, i);
		it = imagePacketBuffer.find(key);
		if( it != imagePacketBuffer.end() ){
			ImageTransmissionUtils::writeTrace( numNode, 
											simulationTime,
					 						"ImgPck - Key: " + key +" found. Grouping into complete buffer");
			buffer.insert(buffer.end(), it->second.begin(), it->second.end());
			
			// Erasing the data from data base to return it for image transmission app
			imagePacketBuffer.erase(key);
			ImageTransmissionUtils::writeTrace( numNode, 
											simulationTime,
					 						"ImgPck - Key: " + key +" erased from sink data base");
		}
		else{
			ImageTransmissionUtils::writeTrace( numNode, 
											simulationTime,
					 						"ImgPck - Key: " + key +" not found. Filling the space with zeros");

			vector<unsigned char> buffer_aux = vector<unsigned char>(bufferSize, '0');
			buffer.insert(buffer.end(), buffer_aux.begin(),  buffer_aux.end());
		}
	}
	// Storing how many images were received from a particular noede
	map<int,int>::iterator it_int;

	it_int = howManyImgReceived.find( int(nodeID) );
	if( it_int != howManyImgReceived.end() ) {
		it_int->second += 1;
	}
	// If any image were received, it's necessary to create a new buffer to store this information
	else {
		howManyImgReceived.insert(pair< int, int>(int(nodeID) , 1));
	}
	
	return buffer;
}

string  SinkNodeManager::getImgBufferKey (unsigned short nodeID, int setNumber, int imgPckNumber){
	int idNode = int(nodeID);
	// key = nodeID-setNumber-imgPckNumber;
	string imgBufferKey;
	ostringstream convert, convert1, convert2;
	convert << idNode;
	imgBufferKey = convert.str();
	imgBufferKey += "-";
	
	convert1 << setNumber;
	imgBufferKey += convert1.str();
	imgBufferKey += "-";
	
	convert2 << imgPckNumber;
	imgBufferKey += convert2.str();
	return imgBufferKey;
}

// bool SinkNodeManager::isNotDuplicatedPacket(unsigned short nodeID, int setNumber, int imgPckNumber) {
// 	string key = getImgBufferKey (nodeID, setNumber, imgPckNumber);
// 	set<string>::iterator it;
// 	// Looking for the packet key in data base to avoid accept duplicated packets
// 	imgPckKeys.find(key);
// 	// Only store a new packet if it is not duplicated from data base
// 	if( it != imgPckKeys.end() ){
// 		imgPckKeys.insert (key);
// 		return true;
// 	}
// 	visualCastaliaTrace.writeTrace( numNode, simulationTime, "It is duplicated");
// 	return false;
// }
