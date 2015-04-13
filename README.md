## Synopsis
	Implementation of shell - a system application for Unix based systems that is able to read 
	and parse commands typed by the user and execute commands given that they are specified in 
	Unix system. In addition, additional built-in commands are added to this project: kill, 
	exit, history. 
	
	In addition, job scheduling mechanism is added to this project. 

## Design (To-do)
	This project is split between three parts, the front-end (parsing), the management of 
	commands and job scheduling.
	
	The parsing is done as it pertains, the user types the commands and this filters these 
	commands based on the type of user-input - piping, redirection of files and background 
	symbols. The commands  are then saved in a text based struct with the purpose to use 
	when showing user history. 
	
	The management of commands and job scheduling should be combined but for the purpose of
	simplicity, I've decided to split this parts to two ways for the sake of bettering 
	readability and maintenance. I have intentions of adding components that will enable this 
	shell to be close in functionalities as other shells.
	
	The management of commands holds the appropriate signal handlers for commands executed. 
	I've followed the signals as it shows in the GNU C library on Job Control Signals. 
	For this part, I will be handling the following signals: 
	
* SIGCHLD 
* SIGINT 
* SIGTSTP 
* SIGCONT 
* SIGTTOU 
* SIGTTIN
	
	I've decided to encapsulate the job scheduler to its own class. This class determines which 
	job to run based on its niceness and other things (To-do) 

## Motivation
	I wanted to have a better grasp on signal handlers, processes and job scheduling. My aim is to 
	improve my knowledge on kernel development and security. 

	In addition, I hope to use this shell as a systems application for small-scale machines such
	as mobile phones and/or arduinos. 
	
## Installation
	Run make and type ./shell
	
## Tests
	To-be implemented
	
## Contributors
	Jouella Fabe
