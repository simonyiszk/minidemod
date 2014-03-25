minidemod
=========

The purpose of <tt>minidemod-wfm</tt> is to have a few lines long C program that can actually demodulate WFM from an I/Q input. 

The whole DSP chain is built by OS pipes, just like this:

<tt>rtl_sdr -s 250000 -f 89500000 -g 20 - | tcc -lm -run minidemod-wfm.c | sox -t raw -r 250000 -e unsigned -b 8 -c 1 - -t raw - rate 48000 | mplayer -quiet -rawaudio samplesize=1:channels=1:rate=48000 -demuxer rawaudio -</tt>

Read like this:

<tt>rtl_sdr (acquires samples) | minidemod-wfm (demodulates) | sox (low pass filter + decimation) | mplayer (audio output) </tt>

To run it, you will need a Linux box with <tt>rtl_sdr tcc sox mplayer</tt> installed.

##Authors

András Retzler <ha7ilm@sdr.hu>
