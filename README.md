# CSE395 - Group Project I - Ball on a plate
Gebze Technical University

Ball on a plate - Fall 2016

Instructor: Erkan Zergeroglu

Team members: 
- Alper MÜLAYİM
- Kaan UÇAR
- Mehmed MUSTAFA
- Mehmet Gürol ÇAY
- Mutlu POLATCAN
- Özgü GÖKSU
- Selman AHATLI
- Yunus ATMACA

Project's Demo Video: https://www.youtube.com/watch?v=YKCav0eDshk

## Description
The goal of this project is to create a mechanism which will balance a ball on a plate. 
We have used a touch panel which is our plate and is used to track the real-time coordinates of the ball. Two servo motors (one for X and one for Y coordinates) balance the ball by moving the panel according to the real-time location of the ball. The movement of each object (the plate, the ball and the servo motors) is mirrored in a 3D animation in real-time. The Ziegler-Nichols tuning method is used to keep the system in equilibrium. The communication between the Arduino and the user interface is achieved with the RS-232 communication standard, while the transmission of information between the user interface and the 3D module is achieved with a TCP socket. The project can run on Windows and Linux platforms.

## INSTALLATION GUIDE

## Installation of OpenSceneGraph for 3D Simulations

- Clone the source files of <b>OpenSceneGraph</b> from here: https://github.com/openscenegraph/OpenSceneGraph/tree/OpenSceneGraph-3.4.0

### Windows

1. Install <b>CMake</b> and <b>MinGW</b> in order to compile source files of <b>OpenSceneGraph</b>

    <b>CMake</b> download link: https://cmake.org/download/
    <b>MinGW</b> download link: https://sourceforge.net/projects/mingw/files/

2. Open <b>CMake</b> and press **"Browse Source"** button and select **"/OpenSceneGraph-3.4.0/src"**

3. Create a new folder named **"mingw-build"** in the same directory

4. Press the **"Browse Build"** button in <b>CMake</b> and select the folder which has been created in step 3

5. Press the **"Configure"** button and select **"MinGW Makefiles"** from the list

6. After the configuration process is completed succesfully press the **"Generate"** button

7. Close <b>CMake</b> once the Makefile generation completes

8. Change the working directory to **"/OpenSceneGraph-3.4.0/mingw-build"** 

9. Enter the **"mingw32-make"** command in the console to compile of the source codes of the <b>OpenSceneGraph</b>

10. After the compilation completes successfully enter the **"mingw32-make install"** command in order to install the required compiled binary packages

11. Add the following folders to the environment variables:

 - **/OpenSceneGraph/bin/**
 - **/MinGW/bin**

12. Install the <b>CLion</b> IDE, open our project and run it

### Ubuntu

1. Install <b>OpenGL</b> library with the following command in the terminal

- **sudo apt-get install libgl1-mesa-dev mesa-common-dev**

2. Change the working directory to the main directory of the downloaded <b>OpenSceneGraph</b> package

3. Run the **"cmake CMakeLists.txt"** command in order to generate the Makefiles

4. After the generation completes successfully run the **"make"** command to compile the source codes of the <b>OpenSceneGraph</b>

5. After the compilation completes successfully enter the **"make install"** command in order to install the required compiled binary packages

6. Install <b>CLion IDE</b> and open our project
