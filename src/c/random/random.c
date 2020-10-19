#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int doFile( char* filein ) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int i;
    int j;
    int size;
    char c;
    int bits[8];

    long long ceros = 0;
	long long unos = 0;

    fp = fopen(filein, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
    	//line
    	size = strlen(line);
    	for( i=0; i<size; i++ ) {
    		c = line[i];
    		for( j=0; j<8; j++ ) {
    			bits[7-j] = (c >> j) & 1;
    			if( bits[7-j] == 0 ) {
    				ceros++;
    			} else {
    				unos++;
    			}
    			//printf("bit[%d][%d]=[%d]\n",i,(7-j),bits[7-j]);
    		}
    	}
    }

    printf("(0)-(1)=(%lld)(%lld)\n",ceros,unos);

    long total = ceros + unos;
    double cerosP = (ceros/(double)total);
    double unosP = (unos/(double)total);
    printf("(0)-(1)=(%.27g%%)(%.27g%%)\n",cerosP,unosP);
    if( cerosP <= 0.5 ) {
    	printf("(error)=(%.27g)-(%.27e)\n",0.5-cerosP,0.5-cerosP);
    } else {
    	printf("(error)=(%.27g)-(%.27e)\n",cerosP-0.5,cerosP-0.5);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

	if( argc == 2 ) {
		doFile( argv[1] );
	}
}
