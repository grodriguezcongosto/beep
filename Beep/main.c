// ----------------------------------------------------------------------------
/**
   File: beep.c

   System:			Hanscan Micropayment
   Component Name: 	Micropayment Beep
   Status: 			Version 1.0 Release 1
   Language: C

   Licensed Material - Property of Hanscan Spain S.A.
   Developed by Ingeniería y Diseño Animatronico y Mecatronico S.L.

   (c) Copyright Hanscan Spain S.A. 2012
   All Rights Reserved

   Address:
      C/ Laureles 21, 1ºB
      28005 Madrid, Spain

   Author: Julio Cesar Pea
   E-Mail: julio.pea@animec.es

   Description: Program file for Micropayments Beep. This Program Handles
   	   	   	    the Buzzer integrated in Micropayment's Hardware.

   Limitations: Don't Allow a continuous Beep sound.

   Usage: beep N t

   Inputs: N: Number of beeps.
   	   	   t : Time between beeps in milliseconds, default 0.5s

   Input Files: none

   Output Files: none

   Example: beep 2 1000
   	   	    Sounds 2 beeps with 1s of duration and 1s interval in between

   Functions: 1) void print_help ()
              2) int beep (int number, int delay)
              3) int main(int argc, char** argv)

   Thread Safe: Yes

   Platform Dependencies: Linux/EXM32

   Toolchain: arm-none-linux-gnueabi

   Compiler Options:

   Change History:
   Version		Date        Author       Description
   V1.0			19/11/12	J.Pea		 Initial Release
*/
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define BUZZER_GPIO "/sys/class/gpio/pioC18/value"  //GPIO Address where is installed the buzzer

// ----------------------------------------------------------------------------
/*
   Routine: print_help()
   Inputs:
      Arguments: None
      Externals: None
      Others: None

   Outputs:
      Arguments: None
      Externals: None
      Returns: Void
      Errors: None

   Routines Called: None

   Description: Prints the help message about the correct usage or the application
*/
// ----------------------------------------------------------------------------
void print_help (){
	printf ("Beep Buzzer V1.0\n");
	printf ("\n Use: beep N t \n");
	printf ("    N : Number of beeps.\n");
	printf ("    t : Time between beeps in milliseconds, default 0.5s\n");
	printf ("\n\nCopyright Hanscan Spain S.A.\n");
	printf ("All Rights Reserved");
}

// ----------------------------------------------------------------------------
/*
  Routine: int beep(int number, int delay)
  Inputs:
	 Arguments: number, delay
	 Externals: none
	 Others: none

  Outputs:
	 Arguments: none
	 Externals: none
	 Returns: int
	 Errors: 0 if Ok, 1 if error

  Routines Called: none

  Description: Opens the GPIO Port where is connected the buzzer and activates it
   	   	   	   the number of times in number during the time in delay
*/
// ----------------------------------------------------------------------------
int beep (int number, int delay){

	FILE *fp;
	char set_value[4];
	int i;

	for (i = number; i > 0; i--){
		if ((fp = fopen(BUZZER_GPIO, "rb+")) == NULL){
			printf("Cannot open value file.\n");
			exit(1);
		}
		//Set pointer to beginning of the file
		rewind(fp);
		//Write our value of "1" to the file to start beep
		strcpy(set_value,"0");
		fwrite(&set_value, sizeof(char), 1, fp);
		fclose(fp);
		//printf("Beep On\n");
		usleep (delay);

		if ((fp = fopen(BUZZER_GPIO, "rb+")) == NULL){
			printf("Cannot open value file.\n");
			exit(1);
		}
		//Set pointer to beginning of the file
		rewind(fp);
		//Write our value of "0" to the file to stop beep
		strcpy(set_value,"1");
		fwrite(&set_value, sizeof(char), 1, fp);
		fclose(fp);
		//printf("Beep Off\n");
		if (i != 0)
			usleep (delay);
	}
	return 0;
}

// ----------------------------------------------------------------------------
/*
   Routine: Main()
   Inputs:
      Arguments: number, delay
      Externals: none
      Others: none

   Outputs:
      Arguments: none
      Externals: none
      Returns: int
      Errors: 0 if Ok, 1 if Error

   Routines Called:
   	  print_help()
   	  beep(number, delay)

   Description: Main program, gets the number of beeps and the delay and calls the functions
   	   	   	   	In case it didn't gets those values, shows the help.
*/
// ----------------------------------------------------------------------------
int main(int argc, char** argv){


	switch (argc){

	case 1: //No arguments
		print_help();
		break;

	case 2: //Only number argument, default delay 0,5s
		beep (atoi(argv[1]), 380000);
		break;

	case 3: //Two arguments
		beep (atoi(argv[1]), (atoi(argv[2])*760));
		break;

	default: //In case of error
		perror ("Error! Incorrect parameter\n");
		print_help();
		break;
	}

	return 0;
}
