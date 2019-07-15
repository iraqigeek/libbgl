#include <stdio.h>

#include <libbgl.h>

int main(int argc, char *argv[]){
	bglFile *bf;
	unsigned short i;

	bf = bglOpen(argv[1]);

	printf("Magic Number: %lx\n", bf->rawHeader.magicNumber);
	printf("Header size: %lx\n", bf->rawHeader.headerSize);
	printf("Low date-time: %lx\n", bf->rawHeader.lowDateTime);
	printf("High date-time: %lx\n", bf->rawHeader.highDateTime);
	printf("Magic Number 2: %08lx\n", bf->rawHeader.magicNumber2);
	printf("Number of sections: %lx\n", bf->rawHeader.numSections);

	for (i = 0; i < 8; i++){
		printf("QMID %i: %lx\n", i, bf->rawHeader.qmid[i]);
	}
}
