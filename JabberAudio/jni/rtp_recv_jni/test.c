#include <stdio.h>
#include <stdlib.h>
#include "rtp_recv_amr.h"

FILE *outfile;
int main(int argc ,char *argv[]){

	outfile=fopen(argv[1],"wb");
	if (outfile==NULL) {
		perror("Cannot open file for writing");
		return -1;
	}
	int port = atoi(argv[2]);
	int yy_handle = yy_init(port);
	fwrite("#!AMR\n",1,6,outfile);
	while(1){
	yy_rtp_recv_amr(yy_handle);



	}

}
