#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int finalList[500];
int originArray[500]; 		//global int array to store Integer list
int i;		     		//counter varibale that will be used in many places.


/*Structure for passing data to threads*/
typedef struct{
    int starting_index;
    int ending_index;
    }parameters;		//Struct defination was copied from Assignment 3 template    
    
void *sorter(void *params);	//thread that performs sorting
void *merger(void *params);	// thread to perform merging of results


int main(int argc, const char * argv[])
{
    FILE *inputFile;                              //Initiate file
    char temp[500];                               //to store as string
    char *cutter;                                //will be used for tokenizing
    inputFile = fopen("IntegerList.txt","r");    //indicates to read file [1]
    fscanf(inputFile, "%s", temp);               //readws content of input file as string to temp char array [1]
    
    int a=0;                                    //to keep count of total integers stored
    cutter = strtok(temp, ",");			//set delimeter to comma
    while(cutter != NULL){                      //while file is not empty 
        originArray[a]= atoi(cutter);           //atoi is used to convert the string to int		[1]
        cutter = strtok(NULL, ",");		//[1] please see refernce list
        a++;                                    //increment after integer is added to array
    }
    

   /*Create parameter data for two arrays*/
    parameters *set1 = (parameters *)malloc(sizeof(parameters));
    parameters *set2 = (parameters *)malloc(sizeof(parameters)); 
    //data set for merger	
    parameters *finalSet = (parameters *)malloc(sizeof(parameters));


    //Setting start and end index, if a is even then divide in half
    set1->starting_index = 0;
    set1->ending_index = ((a/2)-1);  //This will always assign firts half to set1 as int division rounds down
    
    set2->starting_index =(a/2);     //Start from next index where set1 left off
    set2-> ending_index = (a-1);      //takes extra element in this set if a is odd
    
    //data set for merging
    finalSet->starting_index = 0;
    finalSet->ending_index = (a-1);
    
    //First Process
    pthread_t tid1;
    pthread_create(&tid1,NULL, sorter, set1);
    
    //Second Process
    pthread_t tid2;
    pthread_create(&tid2,NULL, sorter, set2);
    
    //joining two sorting threads
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    //Merging Process
    pthread_t tid3;
    pthread_create(&tid3, NULL, merger, finalSet);
    pthread_join(tid3,NULL);
    

    
    printf("\nFinal list will be printed\n");
    
    for(i=0; i< a; i++){
        printf("%d ", finalList[i]);
    }
    
    //Writing to text file
    FILE *outPut;
    outPut = fopen("SortedIntegerList.txt", "w");  //w specifies write to file 
    for(i=0; i<a; i++){
        fprintf(outPut, "%d,", finalList[i]);	//print to file SortedInteger
    }
    fclose(outPut);				//close file
}  

void *sorter(void *params){                     //I will be using Selection Sort with O(n^2)
    int starting = ((parameters *)params)->starting_index;
    int ending = ((parameters *)params)->ending_index;
    printf("S = %d   E = %d\n", starting, ending);
    
    //Starting from here to the end of this method i took help from: https://www.tutorialspoint.com/data_structures_algorithms/selection_sort_program_in_c.htm
    int fLoop, sLoop, min, temp;              
    for(fLoop = starting; fLoop < ending; fLoop++){ // looping through whole list
        min=fLoop;          //set current as minimum index                              
       
        for(sLoop = fLoop+1; sLoop<=ending; sLoop++){       //check next elemnts in list
            if(originArray[sLoop]<originArray[min]){        //smaller element is found
               //update min with nex minimum value
               min=sLoop;
            }
        }
           //now check if both min and fLoop are not pointing to same element
                if(fLoop!=min){
		    //Now time to swap the elements!
                    temp=originArray[min];
                    originArray[min]=originArray[fLoop];
                    originArray[fLoop] = temp;    
                }
                
               
        }
    } 

void *merger(void *params){
        //For my merging, I have implementyed a full sort of my list.
        //I then updated the originla array to final array
        
        int start = ((parameters *)params)->starting_index;
        int end = ((parameters *)params)->ending_index;
        
        int fLoop, sLoop, min, temp;              
        for(fLoop = start; fLoop < end; fLoop++){ // looping through whole list
        min=fLoop;          //set current as minimum index                              
       
        for(sLoop = fLoop+1; sLoop<=end; sLoop++){       //check next elemnts in list
            if(originArray[sLoop]<originArray[min]){        //smaller element is found
               //update min with nex minimum value
               min=sLoop;
            }
        }
           //now check if both min and fLoop are not pointing to same element
                if(fLoop!=min){
                    temp=originArray[min];
                    originArray[min]=originArray[fLoop];
                    originArray[fLoop] = temp;    
                }
                
               
        }
        //Copying in to finalArray
        int a;
        for(a=0; a <=end; a++){
            finalList[a]=originArray[a];
        }
        
        
    }


//help for reading and storing int array is shown as [1] beside code:
//https://www.youtube.com/watch?v=hiG5G2caZ38&ab_channel=NadjibSoft
