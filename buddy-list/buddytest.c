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

    long addr1,addr2,addr3;
    addr1 = getSpace(1000);
    printf("Test 1: got %ld\n", addr1);

    // addr2 = getSpace(500);
    printf("Test 2: got %ld\n", addr2);

    // addr3 = getSpace(1500);
    printf("Test 3: got %ld\n", addr3);
    printBuddyList();


    // printf("Test 4: resetting allocator\n");
    // resetBuddyList();
    // printBuddyList();

    printf("Test 5: freeSpace() all");
    freeSpace(addr3);
    freeSpace(addr2);
    freeSpace(addr1);
    printBuddyList();

    printf("Test 6: freeSpace() some");
    addr1 = getSpace(1000);
    addr2 = getSpace(500);
    addr3 = getSpace(1500); 
    freeSpace(addr3);
    freeSpace(addr1);
    printBuddyList();

}