/* pageio.c --- 
 * 
 * 
 * Author: James D. Fleming
 * Created: Tue Oct 22 17:05:00 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */



#include <stdio.h>                                                              
#include <stdlib.h>                                                             
#include <string.h>                                                             
#include <inttypes.h>                                                           
#include <unistd.h>                                                             
#include <webpage.h>
#include <ctype.h>

// Internal Function Prototypes
static void RemoveWhitespace(char* str);


/*                                                                             
 * pagesave -- save the page in filename id in directory dirnm                 
 *                                                                             
 * returns: 0 for success; nonzero otherwise                                   
 *                                                                             
 * The suggested format for the file is:                                       
 *   <url>                                                                     
 *   <depth>                                                                   
 *   <html-length>                                                             
 *   <html>                                                                    
 */

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {                                          
  if (pagep == NULL || dirname == NULL || id < 0) return false;
	FILE *save;
	char filename[20];                                    
                                                                                                      
  sprintf(filename, "../%s/%i", dirname, id);                                                         
  if ((save = fopen(filename, "w")) == NULL) {                                                        
    return 1;                                                                                     
  }                                                                                                   
                                                                                                      
  // save with a unique id                                                                            
  fprintf(save, "%s\n", webpage_getURL(pagep));                                                       
  fprintf(save, "%d\n", webpage_getDepth(pagep));                                                     
  fprintf(save, "%d\n", webpage_getHTMLlen(pagep));                                                   
  fprintf(save, "%s", webpage_getHTML(pagep));                                                      
                                                                                                      
  fclose(save);                                                                                       
                                                                                                      
  return 0;
}

/*                                                                             
 * pageload -- loads the numbered filename <id> in direcory <dirnm>            
 * into a new webpage                                                          
 *                                                                             
 * returns: non-NULL for success; NULL otherwise                               
 */                                                                            
webpage_t *pageload(int id, char *dirname){
	// create a new webpage	
	char filename[20];
	sprintf(filename, "../%s/%d", dirname, id);
	FILE *file = fopen(filename, "r");
	if (file == NULL){
		printf("file is null");
	}
	
	
	char URL[100];
	fscanf(file, "%s", URL);
	
	
	int depth;
	fscanf(file, "%d", &depth);
	
	
	// First line is URL
	// Second is depth
	// Now we need to scan the total html
	int length;
	fscanf(file, "%d\n", &length);
 	char *html = (char *)malloc(sizeof(char *)*length);
	char *p = html;
	//	strncat(html, "",1);
	char ch;

	while((ch = fgetc(file)) != EOF){
		*p++ = ch;
		//sprintf(html, "%s%c", html, ch); 
	}
	
	//RemoveWhitespace(html);
	fclose(file);
	webpage_t *result = webpage_new(URL, depth, html);
	return result;
}





/*INTERNAL FUNCTIONS*/

static void RemoveWhitespace(char* str){
	char *end;

	end = str + strlen(str)-1;
	while(end > str && isspace((unsigned char)*end)){
		end--;
	}
	end[1] = '\0';
  str = end;
}
