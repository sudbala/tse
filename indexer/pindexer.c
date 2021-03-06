/* indexer.c --- indexes for tse
 * 
 * 
 * Author: Sudharsan Balasubramani
 * Created: Thu Oct 24 10:19:21 2019 (-0400)
 * Version: 
 * 
 * Description: Reads the html assocaiated with eavery webpage fetched by crawler and 
 * creates in memory an index data structure.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <stdbool.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>

/* INTERNAL FUNCTION PROTOTYPES */
void NormalizeWord(char *word);

typedef struct hashIndex{
	char *word;
	
	// frequency is outdated
	//int frequency;
	// queue pointer of webpages that have this word
	queue_t *pages;
	
	
} hashIndex_t;


// MAKE A STRUCT THAT HOLDS A WEBPAGE and frequency of a word asssociated with that webpage

typedef struct wordPage{
	int id; // webpage id
	int frequency; // word frequency
} wordPage_t;

// make a queue of these types 

wordPage_t *makeWordPage(int id) {
	wordPage_t *page;
	if (!(page = (wordPage_t *)malloc(sizeof(wordPage_t)))){
		return NULL;
	}
	
	page->id = id;
	page->frequency = 1;
	
	return page;
}

hashIndex_t *makeIndex(char *word, int id){
	hashIndex_t *Hi;
	wordPage_t *page;
	if (!(Hi = (hashIndex_t *)malloc(sizeof(hashIndex_t)))){
		return NULL;
	}
	Hi->pages = qopen();
	page = makeWordPage(id);
	qput(Hi->pages, (void *)page);
	
	Hi->word = word;
	return Hi;
}

void deleteWordPage(void *wordPage){
	free(wordPage);
}

void deleteIndex(void *hashIndex){
	hashIndex_t *hi = (hashIndex_t *)hashIndex;
	queue_t *myQueue = hi->pages;
	qapply(myQueue, deleteWordPage);
	qclose(myQueue);
	free(hi->word);
	free(hashIndex);
}





static bool searchfn(void *elementp, const void* searchkeyp){
	hashIndex_t *Hi = (hashIndex_t *)elementp;
	if (strcmp(Hi->word, searchkeyp) == 0){
		return true;
	}
	return false;
}

static bool qsearchfn(void *elementp, const void *searchkeyp) {
	wordPage_t *myWordPage = (wordPage_t *)elementp;
	if(strcmp((const char *)(&(myWordPage->id)), searchkeyp) == 0) {
		return true;
	}
	return false;
}
/*
static int wordCount = 0;

static void addToCount2(void *ep) {
	wordPage_t *wp = (wordPage_t *)ep;
	wordCount = wordCount + (wp->frequency);
}

static void addToCount(void *ep){
	hashIndex_t *hi = (hashIndex_t *)ep;
	qapply(hi->pages, addToCount2); 
	//	wordCount = wordCount + (hi->frequency);
}

static int sumwords(hashtable_t *ht){
	happly(ht, addToCount);
	return wordCount;
	
}
*/

// Main
int main(int argc, char *argv[]) {

	char *usage = "usage: indexer <pagedir> <indexnm> <numthreads>";
	if (argc != 4) {
		printf("%s\n", usage);
		exit(EXIT_FAILURE);
	}
	char *pagedir = argv[1];
	char *indexnm = argv[2];

	// Number of threads and check if its over num pages
	int numthreads = atoi(argv[3]);
	if (num
	
	hashtable_t *index = hopen(50);
	
	
	int id =1;
	webpage_t *page;
	
	while((page = pageload(id, pagedir)) != NULL) {
		int pos = 0;
		char *word;
		
		while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
			NormalizeWord(word);
			if (strlen(word) >= 3){
				hashIndex_t *hw = (hashIndex_t *)(hsearch(index, searchfn, (const char *)word, strlen(word)));
				if (hw == NULL){
					// create a new hashIndex_t
					hashIndex_t *ht = makeIndex(word, id);
					// add it to hashtable
					hput(index, ht, (const char *)word, strlen(word));
					
				} else {
					// update the frequency of the hashindex_t by 1
					free(word);
					wordPage_t *wp = (wordPage_t *)(qsearch(hw->pages, qsearchfn, (const char *)&id));
					if (wp == NULL) {
						// create new wordPage - has a id and frequency!
						wordPage_t *newPage = makeWordPage(id);
						// add it to queue of the word
						qput(hw->pages, (void *)newPage);
					} else {
						// Update a wordPage's frequency by 1
						wp->frequency = wp->frequency +1;
					}					
				}
			} else {
				free(word);
			}
		}
		webpage_delete(page);
		id++;
	 }
	indexsave(index, indexnm);
	happly(index, deleteIndex);
	hclose(index);
	exit(EXIT_SUCCESS);
}


/* INTERNAL FUNCTIONS */


/* NormalizeWord - 
 * Converts a word to lowercase
 *
 * Inputs: word - a string
 */ 
void NormalizeWord(char *word) {

	if (word != NULL) {
		for (char *c = word; *c != '\0'; c++) {
			*c = (char)tolower(*c);
		}
	}

}

