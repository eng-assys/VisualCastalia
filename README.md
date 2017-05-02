# VisualCastalia
An extension to support image transmission on Castalia/OMNet++ 

# Description

The VisualCastalia is an extension of the Castalia, a Wireless Sensor Networks (WSN) simulation framework based on the OMNet++ simulator. Castalia provides resources for simulations of WSN applications. 

Although many types of simulations can be performed using Castalia, all simulations are based on transmissions of scalar data. For transmission of visual data (still images and video streams), Castalia does not provide resources to allow realistic simulations of Wireless Visual Sensor Networks (WVSN). 

In this context, the VisualCastalia comes as an extension of the original Castalia framework, bringing new resources for simulations of image transmissions in wireless sensor networks. The idea is to provide a more realistic solution for simulations of such networks. 

This extension was developed in the Laboratory of Multimedia Communications (NetMedia) of the State University of Feira de Santana (UEFS), Brazil. It was originally developed by Prof. Dr. Daniel G. Costa and Lucas Vinicius Assis, with support of CNPq brazilian research agency. 

The VisualCastalia code is still in its alpha version. 
Installation instructions are presented at the end of this page. 

# Characteristics

The VisualCastalia framework is focused on image sensing and transmissions of image packets in WSN. There are many relevant issues concerning Wireless Visual Sensor Networks ([1]) that may be simulated to provide numerical results for different kinds of performance analysis, as visual sensing coverage, deployment, transmission, media coding, compressing, security, among others. In this complex scenario, the VisualCastalia extension is focused on image packets transmissions, addressing packet generation and transmission. 

The main characteristics of this extension are:
The Castalia's Node is extended to support image transmissions. Using VisualCastalia, a Node can transmit image packets considering a transmission frequency (snapshots per second).

Image sensing and transmission are centered in a new application, ImageTransmission, which provides many parameters for configurations of particular aspects of image sensing and transmissions.

Users must define the characteristics of visual data and the simulator transforms visual data into data packets, which are then transmitted using the resources already provided by Castalia.

The VisualCastalia transparently handles image sensing and transmission, properly fragmenting images into data packets. For that, the payload size of data packets is considered.

Image generation is done using OpenCV, which is automatically added and configured when installing VisualCastalia.

Images in VisualCastalia may be real or virtual. Real images are created from image files defined during simulation, allowing visual verification of data corruption during transmissions. On the other hand, virtual images are emulated and their contents are meaningless. Users can choose what kind of image generation will be considered.

VisualCastalia introduces new results: (1) PSNR measure of transmitted and received images, (2) End-to-end latency of image sensing/reconstruction and (3) Specific traces for images handling and processing.
VisualCastalia is focused on packet transmissions and thus some issues of wireless visual sensor networks are not addressed by this simulation extension. Field of View of camera-enabled sensor nodes are not modeled in Visual Castalia. Coverage metrics are not employed and deployment of visual nodes is not addressed as well. 

# Design and specification

VisualCastalia is based on Castalia framework, version 3.2. In such way, the structure of Castalia was carefully considered when defining the VisualCastalia. Actually, many files were inserted but we avoided to remove or alter files of original Castalia. Thus, VisualCastalia can be installed transparently for users who already have Castalia installed and working. Doing so, we made of VisualCastalia a new extension, and not a new simulator. 

All modifications are made in Module Node. New files are inserted into the Node module of Castalia, adding new functionalities. The new files of VisualCastalia are presented in this class diagram, where configuration parameters and the interaction between classes are outlined: 

An ImageTransmission defines an application associated to the defined node. The ImageTransmission inherents from VirtualApplication class of Castalia, which is required by Castalia. 
In short, if a Node uses the ImageTransmission application, it can transmit and receive image packets, turning it into a visual node. Any number of nodes of a simulation may use this application, and we may also have different nodes with different applications (visual and scalar nodes in the same simulation). 

The new files of ImageTransmission are ImageTransmission.cc e ImageTransmission.h. These files specify the basic procedures a visual node implements to transmit image packets. Packet capturing (from OpenCV), packetizing, transmission schedule and packet reception are managed by the ImageTransmission. There are many parameters in the ImageTransmission, where some of them may be directly configured by users. 

An ImagePacket is used to transmit data of (emulated) sensed images. It inherents from ApplicationPacket of Castalia and uses a instance of headerImagePacket to add flexibility and modularity to the solution. Control information may be inserted into the headerImagePacket to allow QoS-based processing of data packets, e.g. marking packets with priority tags. 
The new file inserted into Castalia is ImagePacket.msg. 

The SinkNodeManager defines the behavior of sink nodes, concerning image processing. It is used to manage the reconstruction of received images, saving them in proper directories (specified by users). 
The new file inserted into Castalia are SinkNodeManager.cc and SinkNodeManager.h. 

The ImageManager was designed to support image handling, directly interacting with OpenCV. The ImageHandler provides images for the ImageApplication, and performs reconstruction of received images. 
The new file inserted into Castalia are ImageManager.cc and ImageManager.h. 

# Simulations

Simulations are performed creating an omenetpp.ini file specifying nodes and configurations. Actually, the same parameters used for standard Castalia simulation an been used for VisualCastalia, and some of them are indeed mandatory. 

In VisualCastalia, the time for simulation, nodes and protocols definitions are done in the same way of standard Castalia simulations. When employing VisualCastalia, the only difference is that some or all sensor nodes will use the ImageTransmission application. And, in this case, some basic configurations of that application are required.

All configuration parameters for users are provided by ImageTransmition application. A description for each of those parameters is presented in the following table: 

## Parameter and	Description
| Parameter  |  Description |
|---|---|
| packetHeaderOverhead  | Size of packet header, in bytes  |
| constantDataPayload  | Size of packet payload, in bytes  |
| imageFormat  | Format of considered images (same format for all images). It may be only JPEG (Others formats not yet supported) |
| originalImagesFolder  | Folder of source images to be transmitted in a simulation. The simulation time is controlled by Castalia (sim-time) and thus images are transmitted in a loop sequence until the simulation ends (the same image may be transmitted several times).
receivedImagesFolder	Folder where received images are saved. Those images can be viewed by users or processed for latency and PSNR results  |
| numImageSources  | Indicates the number of different images sent by the source nodes  |
| imageFrequency  | It indicates the number of images (snapshots) transmitted by source nodes every second. It may be lower than 1
selfGeneratedImage	If true, the transmitted images are automatically created by OpenCV. In other words, we may want to use VisualCastalia just to assess image transmissions, without concern to the images contents  |
| selfGeneratedImage  | If selfGeneratedImage=true, it specifies the height of automatically generated images  |
| imgWidth  | If selfGeneratedImage=true, it specifies the width of automatically generated images  |
| useColoredImage  | If selfGeneratedImage=true, it specifies if automatically generated images will be colored (true)  |

An example of part of a omnetpp.ini file is presentes as folows, highlighting configuration of ImageTransmission application: 

 

When VisualCastalia is used, the sink node must use the ImageTransmission application, sinde image reception and reconstruction is only performed at the sink. 


Here are some omnetpp.ini files previously configured that can be used to test the operation of VisualCastalia: 

* Scenario 1. Two nodes (one source and one sink), with source transmitting 1 image per second: here 

* Scenario 2. Four nodes (one source and one sink), with static routing and source transmitting at different frequencies: here 

* Scenario 3. Five nodes, with two sources transmitting through a single multihop path toward the sink and assuming different frequencies: here 

# Results

VisualCastalia provides some additional results to the ones provided by Castalia. Actually, standard CastaliaResults program, presented in Castalia /bin directory, may be used to produce graphics with information of energy consumption and successful packet reception. 

Examples os results of energy consumption for Scenarios 1, 2 and 3 are presented as follows: 
[Results will be delivered soon]

Besides results produced by CastaliaResults, we also defined new results for image transmission. A new script, VisualCastaliaResults, generates results as PSNR and latency. 
[Results will be delivered soon]

# Installation

The installation process of Visual Castalia is explained below: 

* The installation process was tested in operational system Ubuntu 14.04 LTS, thus install VisualCastalia in another Linux distributions can be a little different. 
* Complete information of OMNet++ instalation process can be found here 

The first step is to install OMNet++ simulator: 

## 1 - Download the OMNet++ simulator: here (Choose the option: OMNeT++ 4.6 (source + IDE, tgz)) 
## 2 - Install the dependences od OMNet++: execute the following command to install the prerequisite packages:
```bash
$ sudo apt-get install build-essential gcc g++ bison flex perl tcl-dev tk-dev blt libxml2-dev zlib1g-dev openjdk-6-jre doxygen graphviz openmpi-bin libopenmpi-dev libpcap-dev 
```
    

## 3 - Compile the OMNet++ 
```bash
    $ cd name_of_omnetpp_directory_location
    $ . setenv # First setup the environment variables
    $ ./configure 
    $ make
```
[Optional] Add the omnetpp to the system path of Linux: 

1 - In home directory
```bash
  $ cd 
```

2 - Open .bashrc file in your preferred text editor
```bash
  $ vi .bashrc 
```

3 - In the file's last line add the following text, then save the file
```bash
    $ export PATH=$PATH:$HOME/name_of_omnetpp_directory_location/bin 
```


4 - Close and Reopen the terminal 

Second step: install OpenCv library 

Complete information of installation process of OpenCv can be found here 

1: 
```bash
    $ sudo apt-get install build-essential 
    $ sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev 
    $ sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev 

```

2 - Download the OpenCv source code in version 3.1 (required by VisualCastalia) - OpenCv for Linux/Mac: here 

3 - Build make file and compile OpenCv
```bash
    $ cd name_of_opencv_directory_location 
    $ mkdir build 
    $ cd build 
    $ cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. 
    $ make -j7 # runs 7 jobs in parallel 
    $ sudo make install 

```

Third step: download and installation of VisualCastalia: 

1 - Download the VisualCastalia Ready to Use at the following URL: 

2 - Compile VisualCastalia (equal to compile process of Castalia)
```bash
    $ cd name_of_visual_castalia_directory_location 
    $ make clean 
    $ ./makemake 
    $ make 

```

[Optional] In case of the Castalia simulator is already installed, the VisualCastalia's files can be later added as follows (without loss of Castalia's functionalities):

1 - Overwrite the original makemake of Castalia to VisualCastalia makemake file (this action allows VisualCastalia compile OpenCv library together to Castalia files) 

2 - Add the following files to the bin/ folder of Castalia:
    Makefile 
    psnr.cc 
    VisualCastaliaResults.cc (not stable yet) 

3 - Add the examples of simulation to the Simulation/ folder of Castalia 

4 - Add the new imageTransmission application to the castalia_folder/src/node/application/ folder. The new application is composed by the following files (all the files are important to the correct operation of this extension)s:
ImageManager.cc and ImageManager.h
ImagePacket.msg
ImageTransmission.cc, ImageTransmission.h and ImageTransmission.ned
ImageTransmissionTraces.cc and ImageTransmissionTraces.h
SinkNodeManager.cc and SinkNodeManager.h
5 - Compile VisualCastalia like explained before; 

[Optional] Add the VisualCastalia to the system path of Linux:

1 - In home directory
   $ cd 

2 - Open .bashrc file in your preferred text editor
   $ vi .bashrc

3 - In the file's last line add the following text, then save the file
```bash
  $ export PATH=$PATH:$HOME/name_of_VisualCastalia_directory_location/bin
```
   
4 - Close and Reopen the terminal 


References

[1] Costa, Daniel G., Guedes, Luiz Affonso. The Coverage Problem in Video-Based Wireless Sensor Networks: A Survey. Sensors, 10, 8215-8247, 2010.

[2] Costa, Daniel G., Guedes, Luiz Affonso. A Survey on Multimedia-Based Cross-Layer Optimization in Visual Sensor Networks. Sensors, 11, 5439-5468, 2011.

[3] Tselishchev, Y., Boulis, A., and Libman, L. Experiences and lessons from implementing a wireless sensor network mac protocol in the castalia simulator. In IEEE Wireless Communications & Networking Conferencee. 2010.

[4] Rosario, D., Zhao, Z., Silva, C., Cerqueira, E., Braun, T. An omnet++ framework to evaluate video transmission in mobile wireless multimedia sensor networks. In International ICST Conference on Simulation Tools and Techniques. 2013

