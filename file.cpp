/*
 *  file.c
 *  proclab
 *
 *  Created by xphere on 07/06/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdio.h"
#include "stdlib.h"

// Loads a file to an empty buffer of chars
// Returns: number of read bytes
int file2char (char filename[], unsigned char* &buffer)
{
	FILE *fd;
	long filesize = 0;
	unsigned char letter;
	
	if ((fd=fopen (filename,"rb"))==NULL)
		return -1;
	else
	{
		fseek (fd, 0, SEEK_END);
		filesize = ftell(fd);
		fseek (fd, 0, SEEK_SET);
		buffer = (unsigned char *)malloc (filesize*sizeof(unsigned char));
		for (int i=0; i<filesize; i++)
		{
			fscanf (fd, "%c", &letter);
			buffer[i] = letter;
		}
		fclose(fd);
	}
	return filesize;
}
