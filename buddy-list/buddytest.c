#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"

// split blocks into halves, and halves differ 1 bit
// if take an address, and we XOR the size of the block, we will get the buddy address

int main(void){

    long x = 17;
    long y = 5;

    printf("Hello %ld\n", x^y);

    createBuddyList();
    printBuddyList();
	
    long addr1;
    addr1 = getSpace(1000);
    printf("\n\n========================================================\n\n");
    printf("Test 1: got %ld\n", addr1);
    printBuddyList();
    printf("\n\n========================================================\n\n");

    long addr2;
    addr2 = getSpace(500);
    printf("\n\n========================================================\n\n");
    printf("Test 2: got %ld\n", addr2);
    printBuddyList();
    printf("\n\n========================================================\n\n");

    long addr3;
    addr3 = getSpace(1500);
    printf("\n\n========================================================\n\n");
    printf("Test 3: got %ld\n", addr3);
    printBuddyList();
    printf("\n\n========================================================\n\n");


    // printf("Test 4: resetting allocator\n");
    // resetBuddyList();
    // printBuddyList();

    printf("\n\n========================================================\n\n");
    printf("Test 5: freeSpace() all\n");
    freeSpace(addr3);
    freeSpace(addr2);
    freeSpace(addr1);
    printBuddyList();
    printf("\n\n========================================================\n\n");
    
    
    printf("\n\n========================================================\n\n");
    printf("Test 6: freeSpace() some");
    addr1 = getSpace(1000);
    addr2 = getSpace(500);
    addr3 = getSpace(1500); 
    freeSpace(addr3);
    freeSpace(addr1);
    printBuddyList();
    printf("\n\n========================================================\n\n");

}
