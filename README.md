# Desperado-server
a simple web server using c

## Tales about the name
simple, the most famous web server around is apache, so i wanted something similar just for fun

## How to run
1- compile the files
`gcc -c server.c && gcc -c main.c`  
2- link the files (i just called the final executable file main, you can name it whatever you want)  
`gcc -o server.o && gcc -o main.o -o main`  
3- run the file (needs sudo permission since it's accessing 80 port which is *privileged* port)  
`sudo ./main`

## notice
i created a simple todo app just to exploit more the server, obviously there some problems with the handleing of it and all but it's partially working and that's what matter, the point is not this but making the server in the first place 😊
