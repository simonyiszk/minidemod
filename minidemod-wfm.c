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

To play with mplayer (recommended):

rtl_sdr -s 250000 -f 89500000 -g 20 - | tcc -lm -run minidemod-wfm.c | sox -t raw -r 250000 -e unsigned -b 8 -c 1 - -t raw - rate 48000 | mplayer -quiet -rawaudio samplesize=1:channels=1:rate=48000 -demuxer rawaudio -

To play with ALSA:

rtl_sdr -s 240000 -f 89500000 -g 20 - | tcc -lm -run minidemod-wfm.c | sox -t raw -r 250000 -e unsigned -b 8 -c 1 - -t raw - rate 48000 | aplay -f U8 -c1 -r 48000 --buffer-size=200000

Some parameters of rtl_sdr - e.g. frequency and gain - should be acquired by experimentation with a correct SDR software (gqrx or SDR#).

*/

#include <math.h>
#include <stdio.h>

int main()
{
	double i1=0, i2, q1=0, q2, s;
	for(;;)
	{
		i2=((unsigned char)getchar()-127); q2=((unsigned char)getchar()-127);
		
		//Formula: http://www.dsprelated.com/showmessage/142012/1.php
		s=100*((i2*(q2-q1)) - (q2*(i2-i1)))/(i2*i2+q2*q2);

		//Formula: phase change	
		//s=100*(atan(i2/q2)-atan(i1/q1));

		putchar((unsigned char)(s+127));
		i1=i2; q1=q2;
	}
}
