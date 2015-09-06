#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>

void 
usage(char *prog) 
{
    fprintf(stderr, "usage: %s <-i input file> <-o output file> <-l lowvalue> <-h highvalue>\n", prog);
    exit(1);
}

//http://www.anyexample.com/programming/c/qsort__sorting_array_of_strings__integers_and_structs.xml
int int_cmp(const void *a, const void *b)
{
  rec_t *ia = (rec_t *)a;
  rec_t *ib = (rec_t *)b;
  return (*ia).key - (*ib).key;
}

int
main(int argc, char *argv[])
{
  assert(sizeof(rec_t) == 100);
    // arguments
    char *inFile = "/no/such/file";
    char *outFile = "/no/such/file";
    int lowvalue = 0;
    int highvalue = 0;
    // input params

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "i:o:l:h:")) != -1) {
	switch (c) {
	case 'i':
	    inFile = strdup(optarg);
	    break;
	case 'l':
	    lowvalue = atoi(optarg);
	    break;
	case 'h':
	    highvalue = atoi(optarg);
	    break;
	    // output file name 
	case 'o':
	    outFile = strdup(optarg);
	    break;
	default:
	    usage(argv[0]);
	}
    }

    // create output file
    int fo = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    int fi = open(inFile,  O_RDONLY);
    if (fi < 0) {
	perror("open");
	exit(1);
    }
    if (fo < 0) {
	perror("open");
	exit(1);
    }
    
    rec_t r;
    rec_t *arrayRec = (rec_t *)malloc(100 * sizeof(rec_t));
    if(!arrayRec){
      exit(0);
    }

    int arrcount = 0;
    int tolcount = 0;
    while (1) {	
	int rci;
	// test if there is a line
	rci = read(fi, &r, sizeof(rec_t));
	if (rci == 0) // 0 indicates EOF
	    break;
	if (rci < 0) {
	    perror("read");
	    exit(1);
	}
	// put inside array
	if(r.key < highvalue && r.key > lowvalue){
	  arrayRec[arrcount] = r;
	  arrcount++;
	}
	tolcount++;
    }
    qsort(arrayRec,arrcount,sizeof(rec_t),int_cmp);
    /*
    FOR TEST
    printf("inside arrayRec %lu", sizeof(arrayRec));
    printf("count number %d", arrcount);
    printf("total number %d", tolcount);
    */
    int i;    
    for (i = 0; i < arrcount; i++) {
	int rc = write(fo, &arrayRec[i], sizeof(rec_t));
	if (rc != sizeof(rec_t)) {
	    perror("write");
	    exit(1);
	}
    }
    (void) close(fi);
    (void) close(fo);
    return 0;
}
