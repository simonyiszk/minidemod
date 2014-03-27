/*

Copyright (c) 2014 Andras Retzler <ha7ilm@sdr.hu>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-----------------------------------------------------------------------------

To play with mplayer:

rtl_sdr -s 240000 -f 89500000 -g 20 - | tcc -lm -run minidemod-wfm-atan.c | sox -t raw -r 240000 -e signed -b 16 -c 1 - -t raw - rate 48000 | mplayer -quiet -rawaudio samplesize=2:channels=1:rate=48000 -demuxer rawaudio -

Some parameters of rtl_sdr - e.g. frequency and gain - should be acquired by experimentation with a correct SDR software (gqrx or SDR#).

*/

#include <math.h>
#include <stdio.h>
#include <limits.h>
#define PI M_PI

double get_phase(double i, double q)
{
	double phi=atan(q/i); //It is correct in quadrant I.
	if(i<0&&q>0) phi += PI;            // Correction for quadrant II.
	else if(i<0&&q<0) phi += PI;       // Correction for quadrant III.
	else if(i>0&&q<0) phi +=2*PI;      // Correction for quadrant IV.
	else if(i==0&&q>0) phi = PI/2;     // 90 degrees
	else if(i==0&&q<0) phi = 3*(PI/2); // 270 degrees
	else if(i<0&&q==0) phi = PI;       // 180 degrees
	else if(i>0&&q==0) phi = 0;        // 0 degrees
	return phi; // between 0 and 2*PI.
}

int main()
{
	double i1=0, i2, q1=0, q2;
	short s;
	for(;;)
	{
		i2=((unsigned char)getchar()-127); q2=((unsigned char)getchar()-127);

		//Frequency deviation is actually the time derivative of phase. s(t)=dPhi / dt
		double phi1=get_phase(i1,q1);
		double phi2=get_phase(i2,q2);
		/*
			If we calculated output by just substracting phases from each other, jumping from 359° to 1° would lead to
			faulty behaviour (indicating 358° phase change, while it is really 2° in the other direction...)

			In fact, by interpreting the Nyquist theorem on complex signals, one can find out that signal phase change 
			should be less than 180 degrees while stepping one single sample. So we do this:
		*/

		double dphi=phi2-phi1;
		if(dphi<-PI) dphi+=2*PI; //dphi below -180°? Let's interpret it in the other direction!
		if(dphi>PI) dphi-=2*PI;  //dphi above 180°? Let's interpret it in the other direction!

		// Now let's use signed 16 bit output rather than 8 bit unsigned.
		s=((SHRT_MAX-1)/PI)*dphi; //Okay so let's get the actual sample. dphi should be between -PI and +PI.

		//Note that you can't do this on CPU / DSP architectures that can't do floating point operations.
		//Hence the easy formula in minidemod-wfm.c

		fwrite(&s, sizeof(short), 1, stdout);
		i1=i2; q1=q2;
	}
}
