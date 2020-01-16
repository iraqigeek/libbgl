#include <stdio.h>

#include <libbgl.h>

int main(int argc, char *argv[]){
	bglFile *bf;
	unsigned short numSections, numAirports;

	bf = bglOpen(argv[1]);

	numSections = bglCountSections(bf);
	numAirports = bglCountSectionsByType(bf, BGL_SECTION_AIRPORT);
	
	printf("Number of sections: %ld\n", numSections);
	printf("Number of airports: %ld\n", numAirports);
}
