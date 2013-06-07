LATEST ON RECORD
--
Working at NPL on an experiment that should help understand the dynamics of a 2D dipole lattice!

OLD INTRODUCTION
--

This repository contains all Recorded Academic Progress made by me for completion of courses @ IISER M, that I've shared.

I hope to make all the information accessible for assisting, in whichever way, anyone who's interested.

Although this repository was created on August 10, 2012, I would gradually upload all the usable information from the previous years for completion.

CV
--
I don't quite have enough achievements as yet, but we live with what we have.
https://github.com/toatularora/IISER_repo/blob/master/Semester_3/Summers/backup/cv.pdf?raw=true


SUMMER '13 TIMELINE
--
* June 7, Friday: [Project Milestone] It was realized that neither the sachetIO was required (the data length can be increased to 128 bytes without any difficulty just by changing the report length in the USB HID configurations), nor was a current amplifier. However, today a dipole was made to turn continuously using the latticeAnalyser and the temperature. It was found that the current is more than sufficient for the current task. Further, for now, pins of the MCU can directly be used. Stage one has been accomplished. Now it remains to reduce friction (for the damping is far too high) and to finalize the circuit for the setup. Then the actual experiment begins.
* * June 6, Thursday: Added a buffer function to the sachetIO library. Looked up methods for current amplification
* * June 5, Wednesday: Created and tested sachetIO, a library for sending large arrays, required for communicating over HID USB. This was debugged and tested. Non buffered version is ready.
* June 4, Tuesday: Succesffully linked the latticeAnalyser (after compiling half of it with C, the rest with C++). Modified temperature to include a simple protocol and using that controlled the electromagnet of a single dipole from latticeAnalyser.
* June 3, Monday: Started with the hardware. Configured SP12 (the programmer) then bootloadHID, flashed a board to support a bootloadHID interface.

** June 1 and 2, Saturday and Sunday: Updated the documentation

* May 31, Friday: Tried all sorts of methods for air suspension which failed to work satisfactorily. Despite suspension, we couldn't achieve a state that had low enough torque (either perturbation or friction, atleast one was visible)
* May 30, Thursday: The vacuum cleaner setup had to be used. The box was drilled accordingly and the test failed partially anyway. Which is to say that if it is light enough, the suspension does take place, however the vertical oscillations can not be removed.
* May 29, Wednesday: The pump was built but it failed the test. The air pressure generated was negligible. i7 was configured alongside and the Lattice Analyser was built on it (had to make the multi threading optional using macros) using OpenCV with OpenTBB. Tests were run and it was found to be fast enough for an 8 x 8 dipole matrix.
* May 28, Tuesday: Multi-threading retried, wasn't quite functional last time. Looked up various techniques for multi-threading and froze an algorithm. The pump couldn't be built for parts weren't available.
* May 27, Monday: Multithreading attempted and succeeded, although not a very good release. Making progress in installing IPP.

* May 21 - 26, Tuesday to Sunday: Not well; Succeeded in compiling the code in windows. It's slower. Looked up multithreading using C++ 11

* May 20, Monday: Time Lag measured and found to be roughly 3 to 4 frames behind. Not quite acceptable. Attempting to install OpenCV with IPP

** May 18 and 19, Saturday and Sunday: Completing the documentation for the same. Thought of a way of testing the time lag.

* May 17, Friday: The algorithm was successfully completed to measure 360 degrees. PLUS, completed the frame recording, identification of each dipole as unique and dumping the data out in file AND its testing with uniform motion which it passed with flying colours (which is to say in the visible range!, because proper standard deviation tests haven't quite been done yet) The vision part of the anlyser is almost done.

* May 16, Thursday: Working on dipole detection. The algorithm has started to work partially. It still does a mod 180 detection.

* May 15, Wednesday: The magnetic lifting worked, but friction reduction failed. Rather interestingly the dipole would align to the suspension magnet's field. Plus, today the spot recognition algorithm was finalized and it seemd to be perfect.

* May 14, Tuesday: Trying to get the webcam to work, eventually acceded to installing everything on a desktop machine. Worked on reducing the friction further

* May 13, Monday: Completed the proof of concept version of the latticeAnlyser. Tomorrow we plan to print the coloured ovals and test

** May 11 and 12, Saturday and Sunday: Read the opencv tutorials when the algorithms started appearing and fitting the bill!

* May 10, Friday: Continued with the setup, finetuning, installing other applications, making a documentation alongside for better support next time, added a shared folder between windows and linux

* May 9, Thursday: Managed to get a few things up and running, still setting up ubuntu to run with hardware acceleration, failed at trying to get the webcam to work, installed the build tools, opencv etc.

* May 8, Wednesday: Met with Dr. X (forgot the name of the person at NPL I'm working with) and concluded OpenCV and linux are what I'll use. Initiated the downloading of required applications, including virtual box and an ubuntu image
