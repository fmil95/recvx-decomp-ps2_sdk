[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Program for the Key-Frame Matrix Animation by Interpolating 
the Axis of Rotation 

< Description of the Sample >
	This sample program allows to perform the key-frame animation by 
	interpolating the axis of rotation with the articulated matrix.

	Interpolating the axis of rotation is one of matrix interpolations
	 * Easy to reduce the number of the key-frame with the fine rotation.
	 * Parameter data amount to be retained is relatively small.
	 * Advantage of this method is that the relationship of the 
	   parameter and the actual physical volume can be easily seen 
	   and handled.

< File >
	mkdata/
		In this directory, calculates the parameter for interpolating
		the axis of rotation from the key-frame column of an 
		original matrix. 
	runtime/
		Performs the actual key-frame matrix animation 
		using the parameter created by mkdata/.

< Activatig the Program >
	Refer to the document in their respective directories.

< Using the Controller >
	Refer to the document in their respective directories.

< Note >
[What is interpolating the axis of rotation?]
	This is to interpolate multiple matrices using the matrix rotation
	Method of the interpolation between two matrices is as follows:

	To translate the matrix, executes the simple linear interpolation.
	We are explaining the rotating part only in here.

	If interpolating the articulated matrix MA of a	frame and the 
	articulated matrix MB following the n frame, the MB is considered 
	to be identical to what MA is rotated.  Following is the matrices 
	when interpolated on the assumption that the axis of rotation is 
	alpha and the rotational angle is theta:
		
		0th frame: MA
		1st frame: MA rotated by theta/n around the axis of 
			   rotation alpha
		2nd frame: MA rotated by 2*(theta/n) around the axis of 
			   rotation alpha
		3rd frame: MA rotated by 3*(theta/n) around the axis of 
			   rotation alpha
		  :         :
		kth frame: MA rotated by k*(theta/n) around the axis of 
			   rotation alpha
		  :         :
		nth frame: MB (MA rotated by n*(theta/n) around the axis 
			   of rotation alpha)

[Data to be retained]
	Data required for runtime is as follows:
		Axis of Rotation Alpha: Three parameter (ax,ay,az) vectors
					This is to define the direction of
					the rotational angle so 12 bits 
					integer each is well enough.
		Rotational Angle (theta/n): Generally, float data is 
					required but it depends on data.
		Interpolating Frame Number n: Integer corresponding to the
					longest interpolating frame.
		Translating Volume(T/n):Three parameter (tx,ty,tz) vectors
					 Generally, float data is required

	Although it depends on data, based on the general calculation, 
	approximately 64 bits per key-frame are required for the 
	rotational angle(theta/n) and translating amount (T/n) and another
	64 bits for the	axis of rotation alpha and interpolating frame 
	number n thus 128 bits are required in total.
	
[Applications]
	Interpolating the axis of rotation provides some applications such
	as "changing the axis of rotation according to the status" or 
	"creating the animation which runs through the middle of the 
	multiple animations by interpolating the axes of rotation".
	
[Calculation of interpolating the axis of rotation]
	1. How to obtain the axis of rotation alpha and the rotational 
	   angle theta
		Assuming that MB=MR MA, a specific vector of MR is the 
		axis of rotation alpha.  The MR is easily obtained on the 
		assumption that MR= MB MAt. 
		For obtaining a specific vector, refer to the 
		EigenVector() function in the mkdata/matrix.c. or 
		commercially available books on the matrix math operations

	2. How to obtain the matrix which rotates the axis of rotation 
	   alpha around theta/n
	   	Assuming that alpha=(x,y,z), s=sin(theta/n), c=cos(theta/
		n),the calculation of the rotational part of the matrix 
		Mr which rotates the angle r around the axis of rotation 
		alpha is as follows:
		
		{[ (1-c) x^2 + c,   (1-c) x y + z s, (1-c) x z - y s ],
		 [ (1-c) x y - z s, (1-c) y^2 + c,   (1-c) y z + x s ],
		 [ (1-c) x z + y s, (1-c) y z - x s, (1-c) z^2 + c ]}

		Also, refer to the function a2m() in the runtime/axis.c. 

