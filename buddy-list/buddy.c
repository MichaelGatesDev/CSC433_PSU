#include <stdio.h>

#include "buddy.h"

// global data structure to store the free space
struct buddy_node bl[NUMSIZES];

long splitBlock(long index);

long rightIndex(long size);

void createBuddyList(){

    printf("Starting create\n");

    // walk through the buddy list and initialize sizes and counts
    // at the top, maxsize, each level down will be half of the size
    int i;
    int cursize = BUDDYMAXSIZE;
    for (i = 0; i < NUMSIZES; i++)
    {
        bl[i].size = cursize;
        bl[i].freeCount  = 0;
        bl[i].allocatedCount = 0;
        cursize /= 2;
    }

    // in practice, we could get this space from a system, or malloc
    bl[0].freeCount = 1;
    bl[0].freeList[0] = 0;

    printf("Done create\n");
}

void printBuddyList(){
    // each list
    int i;
    for (i = 0; i < NUMSIZES; i++)
    {
        printf("block size: %ld, free count: %ld\n\t", bl[i].size, bl[i].freeCount);
        // each block
        int j;
        for(j=0;j<bl[i].freeCount; j++){
            printf("%ld ", bl[i].freeList[j]);
        }
        printf("\n");


        printf("block size: %ld, allocated count: %ld\n\t", bl[i].size, bl[i].allocatedCount);
        // each block
        for(j=0;j<bl[i].allocatedCount; j++){
            printf("%ld ", bl[i].allocatedList[j]);
        }
        printf("\n");
    }
}


void resetBuddyList(){
    // walk through the buddy list and initialize sizes and counts
    // at the top, maxsize, each level down will be half of the size
    int i;
    for (i = 0; i < NUMSIZES; i++)
    {
        bl[i].freeCount = 0;
        bl[i].allocatedCount = 0;
    }

    // in practice, we could get this space from a system, or malloc
    bl[0].freeCount = 1;
    bl[0].freeList[0] = 0;
}


long getSpace(long size){
    // go down the list to find the smallest chunk size that is big enough
    // then return a block of that size
    // if none exists, then take a higher level block and split it
    return splitBlock(rightIndex(size));
}

long rightIndex(long size){
    // find the right size block
    // each list
    int i;
    int rightIndex = NUMSIZES - 1; // index of the smallest available piece
    // find the right index
    for (i = 0; i < NUMSIZES; i++)
    {
        // printf("Comparing %ld and %ld \n", bl[i].size, size);
        if(bl[i].size < size && bl[i-1].size >= size){
            rightIndex = i - 1;
        }
    }
    return rightIndex;
}

long splitBlock (long index) {

    if(index < 0){
        return -1; // There is not enough space to allocate
    }

    if(bl[index].freeCount > 0){
        // take a block off and return it
        bl[index].freeCount -= 1;
        return bl[index].freeList[bl[index].freeCount];
    }
    else {
        long original = splitBlock(index - 1);
        long buddy = original ^ bl[index].size;
        // add buddy to free list
        bl[index].freeList[bl[index].freeCount] = buddy;
        bl[index].freeCount++;

        // add original to allocated list
        bl[index].allocatedList[bl[index].allocatedCount] = original;
        bl[index].allocatedCount++;

        return original;
    }
}

long freeSpace(long addr){
    // idea is to find it on the smallest allocated list we can
    // then check if its buddy is on that free list
    // if not, then put this one on the free list
    // if so, then merge them and return them to the next level up

    
    // idea is to look to see if the new block is a buddy of one already on the list
    // if so, then combine them
    // otherwise, put it on the list

    // to compute the address we compute which 

    int i = NUMSIZES - 1;
    int notdone = 1;

    // checking sizes
    while(notdone == 1 && i > 0){
        i -= 1;
        int j = 0;
        // looking for the element
        for(j=0; j<bl[i].allocatedCount; j++){
            // if on allocated list
            if(bl[i].allocatedList[j] == addr)
            {
                notdone = 0;
                // now we need to take it off the allocated list

                // swap the last element on the list and then decrease list size
                bl[i].allocatedCount-=1;
                bl[i].allocatedList[j] = bl[i].allocatedList[bl[i].allocatedCount];

            }
        }
    }

    // postcondition: i is the smallest (block size) that the address was found allocated
    // address has been removed from the allocated list

    if(notdone == 1){
        // the block that was attempted to be freed was not found as allocated
        printf("ERROR: did not find address %ld\n", addr);
        return -1;
    }

    // look at the free list for the buddy
    long buddy = addr ^ bl[i].size;
    int foundBuddy = 0;
    int j;
    for(j=0; j < bl[i].freeCount; j++){
        // if we find the buddy
        if(foundBuddy == 0 && bl[i].freeList[j] == buddy) {
            // remove it
            bl[i].freeCount-=1;
            bl[i].freeList[j] = bl[i].freeList[bl[i].freeCount];
            foundBuddy = 1;
        }
    }

    if(foundBuddy){
        // see which value is smaller
        if(buddy < addr){
            freeSpace(buddy);
        }
        else {
            freeSpace(addr);
        }
    } else {
        bl[i].freeList[bl[i].freeCount] = addr;
        bl[i].freeCount+=1;
    }

}