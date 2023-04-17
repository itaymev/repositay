/*
This project simulates matching individuals for organ transplants. 
A transplant requires a recipient (R), a donor (D) with compatible blood type, 
and a surgeon (S) who is available to perform the procedure. The goal is to 
read an argument-provided text file, (see repository for example input) where each line of the 
file represents the event of a new recipient, donor, or surgeon becoming available, listed in 
chronological order (top-to-bottom). We assume transplants should occur on a first-come, first-served basis. 
*/

#include <stdio.h>   /* printf */
#include <stdlib.h>  /* malloc */
#include <string.h>  /* strcmp */

#define QUEUE_SIZE 12  /* Note: Works with 12 on bloodtypes-input.txt */
#define BLOOD_TYPES 4
#define LINE_LENGTH 80
#define BLOOD_TYPE_ORDER {"AB", "B", "A", "O"}

struct queue
{
	char *strings[QUEUE_SIZE];
	int front;
	int back;
	int population;
};
typedef struct queue Queue;


void match_print(char *donor, char *recipient, char *surgeon)
{
  printf(
      "MATCH: %s donates to %s via Dr. %s\n",
      donor,
      recipient,
      surgeon);
}
    
void initializeQueue(Queue *q)
{
	q->front = 0;
	q->back = 0;
	q->population = 0;
  for(int i=0;i<QUEUE_SIZE;i++)
    q->strings[i]=NULL;
	//printf("front is %d, back is %d, population is %d\n", q->front, q->back, q->population);
}

void enqueue(char *element, Queue *q)
{
 // printf("ENQUEUE %s %d in %p\n", element,q->back,q);
	if(q->population >= QUEUE_SIZE) {
		printf("Queue %p is full cant add %s\n",q,element);
		return;
	}

	if(strlen(element) == 0) {
		printf("Element is empty\n");
		return;
	}

	//printf("%d, %s\n", q->back, element);
	q->strings[q->back] = malloc(sizeof(char) * strlen(element));
	strcpy(q->strings[q->back], element);
	q->back++;

	if(q->back >= QUEUE_SIZE)
		q->back = 0; 

	q->population++; 
	//printf("%d\n", q->population);
}

char *dequeue(Queue *q)
{
	if(q == NULL || q->population < 1){
    printf("Error trying to dequeue from %p f %d p %d queue\n",q,q->front,q->population);
    return NULL;
  }
    
  q->front++;
	q->population--;
	//printf("DEQUEUE %s from %d in %p\n",q->strings[q->front-1],q->front-1,q);
  return q->strings[q->front-1];
} 


int isDonorToRecipient(char *donor, char *recipient)
{
  if (strcmp(donor, "O") == 0 || strcmp(donor, recipient) == 0)
    return 1;
  if (strcmp(donor, "A") == 0 && strcmp(recipient, "AB") == 0)
    return 1;
  if (strcmp(donor, "B") == 0 && strcmp(recipient, "AB") == 0)
    return 1;
  return 0;
}
int isDonorToRecipientInd(int donor, int recipient )
{
  char bt[4][4]= BLOOD_TYPE_ORDER;
  return isDonorToRecipient(bt[donor],bt[recipient]);
  

}


void printQueue(struct queue *q)
{
  printf("Printing queue %p\n", q);
  printf("\tThe index for the front of the queue is %d\n", q->front);
  printf("\tThe index for the back of the queue is %d\n", q->back);
  if (q->population == 0)
  {
    printf("\tThe queue is empty.\n");
  }
  else
  {
    for (int i = 0; i < q->population; i++)
    {
      int index = (q->front + i) % QUEUE_SIZE;
      printf("\t\tEntry[%d]: \"%s\"\n", index, q->strings[index]);
    }
  }
}
int process_s(Queue * recipients, Queue * donors,char *s_name) //receive a suregon name and find a match, return 1 if found, 0 if not found
{

    //A recipient with blood type AB is compatible with AB, B, A, and O donors, preferring matches in that order.
    if(recipients[0].population>0){
    //  printf("AB pop 0 %d\n",recipients[0].population);
      if(donors[0].population>0)       { match_print(dequeue(&donors[0]),dequeue(&recipients[0]),s_name); return 1;}//AB with AB 
      if(donors[1].population>0)       { match_print(dequeue(&donors[1]),dequeue(&recipients[0]),s_name); return 1;}//AB with B 
      if(donors[2].population>0)       { match_print(dequeue(&donors[2]),dequeue(&recipients[0]),s_name); return 1;}//AB with A
      if(donors[3].population>0)       { match_print(dequeue(&donors[3]),dequeue(&recipients[0]),s_name); return 1;}//AB with O
    }
    //A recipient with blood type B is compatible with B and O donors, preferring matches in that order.
    if(recipients[1].population>0){
     /// printf("B pop 1 %d \n",recipients[1].population);
      if(donors[1].population>0)       { match_print(dequeue(&donors[1]),dequeue(&recipients[1]),s_name); return 1;}//B with B 
      if(donors[3].population>0)       { match_print(dequeue(&donors[3]),dequeue(&recipients[1]),s_name); return 1;}//B with O 
    }
    //A recipient with blood type A is compatible with A and O donors, preferring matches in that order.
    if(recipients[2].population>0){
     // printf("A pop 2 %d \n",recipients[2].population);
      if(donors[2].population>0)       { match_print(dequeue(&donors[2]),dequeue(&recipients[2]),s_name); return 1;} //A with A 
      if(donors[3].population>0)       { match_print(dequeue(&donors[3]),dequeue(&recipients[2]),s_name); return 1;}//A with O
    }
    //A recipient with blood type O is compatible with O donors.
    if(recipients[3].population>0){
      //printf("O pop 3 %d \n",recipients[3].population);
      if(donors[3].population>0)       { match_print(dequeue(&donors[3]),dequeue(&recipients[3]),s_name); return 1;}//O with O 
    }
    return 0;
}

void prettyPrintQueue(struct queue *q, char *label, char *type)
{
  if (q->population == 0)
  {
    printf("No unmatched entries for %s (%s)\n", label, type);
  }
  else
  {
    printf("Unmatched %s (%s):\n", label, type);
    for (int i = 0; i < q->population; i++)
    {
      int index = (q->front + i) % QUEUE_SIZE;
      printf("%s\n", q->strings[index]);
    }
  }
}

int bt_string_to_index(char *bt)
{
    if(strcmp(bt, "AB") == 0)
      return 0;
    if(strcmp(bt, "B") == 0)
      return 1;
    if(strcmp(bt, "A") == 0)
      return 2;
    if(strcmp(bt, "O") == 0)
      return 3;
}

int main(int argc, char **argv)
{
  
char *types[BLOOD_TYPES] = {"AB", "B", "A", "O"};
struct queue donors[BLOOD_TYPES];
struct queue recipients[BLOOD_TYPES];
struct queue surgeons;

  initializeQueue(&surgeons);

  for(int i = 0; i < BLOOD_TYPES; i++) {
	  initializeQueue(&donors[i]);
	  initializeQueue(&recipients[i]);
	}

  FILE *fp = fopen(argv[1], "r");
  if(fp == NULL) {
  	printf("cannot open file\n");
	  exit(EXIT_FAILURE);
  }

  char *str = malloc(sizeof(char)*LINE_LENGTH);
  char *name;
  int bt_ind;
  while(fgets(str, LINE_LENGTH, fp) != NULL) 
  {
	
    if(str[0] == 'S'){
      name=strtok(&str[2],"\n"); //if its a S the name is right after the ':'
      //printf("**S**   %s\n", name);
      //check to see if there is a match waiting just for S and if not Q it
      if(process_s(recipients, donors,name)==0) //not found a match - need to queue the surgeon
        enqueue(name, &surgeons);
      continue;
    }
    else{ //if its not a S lets parse the blood type and the name 
      
      char *bt = strtok(&str[2], ":");
      name = strtok(NULL, "\n");
      bt_ind = bt_string_to_index(bt);
      int match=0;
      if(str[0] == 'R') {
          //printf("**R** %s\n", name);
          if(surgeons.population>0){ //we have a surgeon available 
            for(int i=0;i<4;i++){ 
              if(donors[i].population > 0){ //if we have a donor
                if(match==0 && isDonorToRecipientInd(i,bt_ind)){ //check compatability
                  match_print(dequeue(&donors[i]),name,dequeue(&surgeons));//process it
                  match=1;
                }  
              } 
            }
          }
          if(match==0){
            enqueue(name, &recipients[bt_ind]); 
            match=0;
          }
          
      }

      if(str[0] == 'D'){ 
          //printf("**D** %s\n", name);
          if(surgeons.population>0){ //we have a surgeon available 
            for(int i=0;i<4;i++){ 
              if(recipients[i].population > 0){ //if we have a donor
                if(match==0 && isDonorToRecipientInd(bt_ind,i)){ //check compatability
                  match_print(name, dequeue(&recipients[i]),dequeue(&surgeons));//process it   
                  match=1;
                }
              }
            }
          }
          if(match==0){
            enqueue(name, &donors[bt_ind]);
            match=0;
          }
      }
      }
    

    
  }

  for (int r = 0; r < BLOOD_TYPES; r++)
    prettyPrintQueue(&recipients[r], "recipients", types[r]);
  for (int d = 0; d < BLOOD_TYPES; d++)
    prettyPrintQueue(&donors[d], "donors", types[d]);
  prettyPrintQueue(&surgeons, "surgeons", "type-agnostic");

  //free all dynamic memory strings allocations 
	
  for(int i= 0; i < QUEUE_SIZE; i++){
    for (int r = 0; r < BLOOD_TYPES; r++){
      if(recipients[r].strings[i]!=NULL)
        free(recipients[r].strings[i]);
      if(donors[r].strings[i]!=NULL)
        free(donors[r].strings[i]);
    }
      if(surgeons.strings[i]!=NULL)
        free(surgeons.strings[i]);
    }
  
  
  fclose(fp);      
  free(str);

  return EXIT_SUCCESS;
}


/* End. */
