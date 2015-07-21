#include <string.h>
#include <stdio.h>
#include "cfitsio/fitsio.h"


/*declares data type "node", which contains one item in a linked list and a pointer to the next node*/
typedef struct node 
{
	int *val;
	struct node *next;
} node_t;

/*adds a node to the end of the list*/
void append(node_t* head, int val) 
{
	node_t* curr = head;

	while (curr->next != NULL) 
	{
		curr = curr->next;
	}
	
	curr->next = malloc(sizeof(node_t));
	curr->next->val = val;
	curr->next->next = NULL;
}

/*cycles through the list and prints out each value inside the list.  You can modify this code for any purposes where you're cycling through values in the list*/
void print_list(node_t* head) 
{
	/*creates a "current node" pointer that starts at the head*/
	node_t* curr = head;
	
	/*for every loop, prints out the "value" item (val) of the current node (curr->val indicates the item val inside of our node curr), then sets curr to be curr's "next" item*/
	while (curr != NULL) 
	{
		printf("%d\n", curr->val);
		curr = curr->next;
	}
}

/*this is your main function, which takes each value from a given keyword (that returns an INT only!  Also note that it's not set up only to look at NHITS or NAXIS2, you will need to include that when you run it) and saves it into a linked list*/
int saveAllInstancesofKeyword(char* filename,char* keyword, node_t* head)
{
    fitsfile *fptr;         /* FITS file pointer, defined in fitsio.h */
    char card[FLEN_CARD];   /* Standard string lengths defined in fitsio.h */
    int status = 0;   /* CFITSIO status value MUST be initialized to zero! */
    int single = 0, hdupos, nkeys, ii;
    int value;	

    if (!fits_open_file(&fptr, filename, READONLY, &status))
    {
    	fits_get_hdu_num(fptr, &hdupos);  /* Get the current HDU position */

    	/* List only a single header if a specific extension was given */ 
    	if (hdupos != 1 || strchr(filename, '[')) single = 1;

    	for (; !status; hdupos++)  /* Main loop through each extension */
    	{	
		fits_read_key(fptr, TINT, keyword, &value, NULL, &status);
		
		append(head, value);

		if (status == KEY_NO_EXIST)  status = 0;
      
		fits_get_hdrspace(fptr, &nkeys, NULL, &status); /* <-- gets # of keywords.  Necessary for some as of yet undetermined reason. */

		fits_movrel_hdu(fptr, 1, NULL, &status);  /* try to move to next HDU */
    	}

    	if (status == END_OF_FILE)  status = 0; /* Reset after normal error */

    	fits_close_file(fptr, &status);
    
    }
    
    if (status) fits_report_error(stderr, status); /* print any error message */
    return(status);

}

/*run your function here.  You can comment out parts of the code like print_list to do what you want, but keep in the list declarations*/
int main(int argc, char *argv[])
{
	/*make sure you keep this!  It defines and reserves memory for the beginning of the list that you're saving keywords to*/  
	node_t* head = NULL;  
	head = malloc(sizeof(node_t));  

	/*argv[1] is your fits file, and argv[2] is your keyword.  Use either NHITS or NAXIS2*/
	saveAllInstancesofKeyword(argv[1], argv[2], head);  

	/*just calls the print_list function.  You can comment this out when it isn't necessary anymore*/
	print_list(head);
 	return 0;
}

