# Primitive Man - Assignment 2 - COMP 308
=====
Author: Iury Roger Painelli
Student ID: 300333846

=====
## How to run:

####Core mode (show the primitive man)

make clean

make

Run: ./Ass2 priman.asf

####Extension mode (Poses)

Run: ./Ass2 priman.asf pose.amc

#Challenge mode (Mocap Player)

Run: ./Ass2 priman.asf priman.amc (or desired amc file)

=====
IMPORTANT: There is a menu attached with the right-button click to control different parts of the assignment.
		   The menu changes according to the part of the assignment.

=====
## Controls:

####Core mode
'q', 'e', and 'r' keys rotate the skeleton 

####Extension mode
'p' switch poses

####Challenge mode

'z' is Rewind

'x' is Play

'c' is Stop

'v' is Pause

'b' is Fast Forward


=====
## What I did
####Core part

		The application reads the bones and the hierarchy.
		
		After that, the function DrawBone() is called to draw the axis and the bone using the information from the asf file.
		
		The cross product is calculated to draw the cylinder in the correct angle.
		
		Then the bones are drawn and translated recursively in the function readChildBones().
		
		The math operations are in the operation.cpp file
		
####Extension part

		POSES: PUNCHING someone else's face, SITTING in a Stool, and WALKING.
		
		I implemented the readAMC() and readTransfBoneAMC() to read the .amc file and show the poses.
		
		Those function read the amc file and create the keyframes using an array called 'frames', the array points to the Frame class.
		
		After reading the amc file, I change the function drawBone() to multiply the rotation matrix using the amc data.
		
		Using this method, the application show the next frame every time the 'p' key is pressed or the "Next Pose" item in the menu is clicked
		
		I set the poses inside the pose.amc file.
		
####Challenge part

		The extension part helped a lot with the challenge part, so I just had to set an animation control, and solve some problems with the frames.
		
		I also implemented the menu in this part of the assignment, but I adapted the menu to work in the other parts too.
		
		IMPORTANT: the application only consider an .amc file as an animation if the file has MORE than 3 frames. If the .amc file has less than 3 frames, is considered as a pose file.
		
That's it! Thank you for reading the file. :)

