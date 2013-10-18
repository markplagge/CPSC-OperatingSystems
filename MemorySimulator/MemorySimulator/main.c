//
//  main.c
//  MemorySimulator
//
//  Created by Mark Plagge on 12/3/12.
//  Copyright (c) 2012 csu. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>

char* fileFormat = "%d\t\t%d";
int PID[4];
int offset[4];
int pgNum[4];
long FIFOTimeStamp[4];
int LFUAccessCount[4];
long LRUTimeStamp[4];


int currentFrame = 0;
//Current method of replacement:
char replacementMethod = '1';

///*****************STRUCTS****************************

//Page Table Entry - Represents a Page Table Entry
struct PageTableEntry
{
    bool isValid;
    int pgFrameNum;
    int timeStamp;
};

//Process - Represents a process.
struct Process
{
    int ID;
    struct PageTableEntry pageTable[16];
};

struct timeval tv;

//**********ALGORITHM AREA *************//
//first in first out
int FIFO()
{
    long shortestTime = FIFOTimeStamp[0];
    int index = 0;
    for(int i = 1; i < 4; i ++)
    {
        if ( shortestTime > FIFOTimeStamp[i])
        {
            shortestTime = FIFOTimeStamp[i];
            index = i;
        }
    }
    return index;
}
//Least Freq. Used
int LFU()
{
    int leastUsed = LFUAccessCount[0];
    int index = 0;
    for(int i = 1; i < 4; i ++)
    {
        if ( leastUsed > LFUAccessCount[i])
        {
            leastUsed = LFUAccessCount[i];
            index = i;
        }
    }
    return index;
    
}

//Least Recently Used
int LRU()
{
    long shortestTime = LRUTimeStamp[0];
    int index = 0;
    for(int i = 1; i < 4; i ++)
    {
        if ( shortestTime > LRUTimeStamp[i])
        {
            shortestTime = LRUTimeStamp[i];
            index = i;
        }
    }
    return index;
}

void insertFrame(int fNum, int nPage,int nOffset, int procID)
{
    PID[fNum] = procID;
    offset[fNum] = nOffset;
    pgNum[fNum] = nPage;
    gettimeofday(&tv, 0);
    FIFOTimeStamp[fNum] = tv.tv_sec;
    LRUTimeStamp[fNum] = tv.tv_sec;
    LFUAccessCount[fNum] = 1;
    
}
//This is called when a process access memory that already exists in the frame buffer.
//this assumes that the process has already checked to see if the frame is valid.
void updateFrame(int fNum)
{
    gettimeofday(&tv, 0);
    LRUTimeStamp[fNum] = tv.tv_sec;
    LFUAccessCount[fNum] += 1;
}

///***END ALGORITHM AREAS ************//

int getReplacedFrame(char algorithmSel)
{
    switch (replacementMethod) {
        case '1':
            currentFrame = FIFO();
            break;
        case '2':
            currentFrame = LFU();
            break;
        case '3':
            currentFrame = LRU();
            break;
        default:
            printf("Invalid input. Default method is FIFO, selecting.");
            replacementMethod = '1';
            //do FIFO method
            
            break;
    }
    
    return 0;
}



int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        printf("Run this program by typing MemorySimulator.exe XXXXXXX.TXT, where XXXXXXX.TXT is the name of the file you wish to open to simulate.\n\n");
    }
    else
    {
        //initscr();
        printf("ARGC = %d",argc);
        // Load the file into the system:
        FILE *fr = fopen(argv[1], "rt");
        printf("file loaded");
        
        int currentPID = 0;
        int vAddress = 0;
        printf("\n");
        printf("%s", argv[1]);
        printf("Loaded - Starting \n");
        printf("PAGE REPLACEMENT POLICIES: 1 -> FIFO, 2 -> LFU, 3 -> LRU\n");
        
        
        //Set up a holder
        
        //parse the file items, simulating the memory allocs and accesses while reading it in
        
        
        int biggestProc = 0;
        
        while (fscanf(fr, fileFormat, &currentPID, &vAddress) > 0)
        {
            if(currentPID > biggestProc)
                
            {
                biggestProc = currentPID;
            }
        }
        biggestProc ++;
        struct Process processHolder[biggestProc];
        
        //populate the holder with empty processes
        for (int i = 0; i < biggestProc; i++) {
            struct Process tempProc;
            processHolder[i] = tempProc;
            processHolder[i].ID = i;
            
        }
        fclose(fr);
        //Clean up, then open again.
        fopen(argv[1], "rt");
        //refresh();
        //The process starts here: *******************************************
        while (fscanf(fr, fileFormat, &currentPID, &vAddress) > 0)
        {
            //refresh();
            int currentI;
            
            processHolder[currentPID].ID = currentPID;
            if(vAddress >= 64)
            {
                printf("\nPID %d has an illigal memory address!",currentPID);
            }
            else
            {
                int currentPageNumber = vAddress / 4;
                int currentOffset = vAddress % 4;
                
                //Next, find the item in memory:
                int inRam = 0;
                for(int i = 0; i < 4; i ++)
                {
                    if (PID[i] == currentPID && pgNum[i] == currentPageNumber)
                    {
                        printf("\nPage %d of PID %d is in memory, the physicial address is frame %d, offset %d", currentPageNumber, currentPID, i, offset[i]);
                        inRam = 1;
                        updateFrame(i);
                        currentFrame = i;
                    }
                }
                //if it is not in memory:
                if (inRam == 0) //AUTOMATED MODE:
                {
                    if(argc == 3) //If the user specified that the system should run in automatic, algorithm specificied mode:
                    {
                        printf("ARGV2 = %s",argv[2]);
                        
                        int sel = (rand()%3) + 1;
                        printf("sel = %d",sel);
                        
                        if (sel == 1)
                        {
                            getReplacedFrame('1');
                            printf("\nFIFO\n");
                        }
                        else if (sel == 2)
                        {
                            getReplacedFrame('2');
                            printf("\nLFU\n");
                            
                        }
                        else
                        {
                            getReplacedFrame('3');
                            printf("\nLRU\n");
                            
                        }
                        insertFrame(currentFrame, currentPageNumber, currentOffset, currentPID);
                        
                    }
                    
                    else { // Non automatic mode:
                        
                        printf("\nPAGE FAULT. Select the page replacement policy you want to use to swap in the required page:");
                        //TODO: Need to add input validation here in order to ensure that the user does not break the input! ********
                        //refresh();
                        scanf("%c",&replacementMethod);
                        getchar();
                        printf("\nYou chose method: %c",replacementMethod);
                        //refresh();
                        //get replacement frame:
                        getReplacedFrame(replacementMethod);
                        //do the replacement frame:
                        insertFrame(currentFrame, currentPageNumber, currentOffset, currentPID);
                        printf("\nRequired page is brought into memory. The physical addres is (%d, %d)",currentFrame,currentOffset);
                    }
                    
                }
                else
                {
                    
                }
                
                //Print out the frame ***
                printf("\n\t---------------------------------\n");
                printf("\t|FRAME\t|PID\t|PAGE\t|OFFSET\t|");
                printf("\t  Current Process\n");
                for (int i = 0; i < 4; i ++) {
                    printf("\t|%d\t|%d\t|%d\t|%d\t|",i,PID[i], pgNum[i], offset[i]);
                    if(i == 0)
                    {
                        if(currentPID > 9)
                        {
                            printf("\t\tPID:   %d\n", currentPID);
                        }
                        else
                        {
                            printf("\t\tPID:    %d\n", currentPID);
                        }

                    }
                    else if(i == 1)
                    {
                        printf("\tPage Number:\t%d\n", currentPageNumber);
                    }
                    else if(i == 2)
                    {
                        printf("\t     Offset:\t%d\n", currentOffset);
                    }
                    else if(i == 3)
                    {
                        printf("\t      Frame:\t%d\n", currentFrame);
                    }
                }
                printf("\t---------------------------------\n");
                
                
                getchar();
                
                struct PageTableEntry tpg;
                struct PageTableEntry *newPageTable = &tpg;
                newPageTable->isValid = false;
                
                //processHolder[currentPID].pageTable =
                
                
            }
            printf("\n"); //This puts a new line after each entry
        }
        
        //
        
        
        fclose(fr);
        //endwin();
    }
    
    return 0;
}

