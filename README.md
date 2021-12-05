
Omusubin embed binary data in an executable binary

Get Started

execute

./autogen.sh

this make omusubin binary in src folder

Usage

omusubin need conf file 


Format of conf file

make list of file to be embeded
 
FILE
 [file name(file id)] [file extension] [file path]
 ...
END_FILE

TARGET [executable binary file path]

example : see the format in test/test/asset/test1_omusubin.conf

to insert binary

omusubin insert [conf file path]
 
to extract binary

omusubin extract [target executable file path]
 
