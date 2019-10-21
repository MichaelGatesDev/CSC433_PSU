#define MAXLIST 20
#define NUMSIZES 10
#define BUDDYMAXSIZE 8192

struct buddy_node {
    long size;                  // block size
    long freeList[MAXLIST];    // free addresses
    long freeCount;                 // how many blocks are free
    long allocatedList[MAXLIST];    // blocks of this size that are used
    long allocatedCount; // how many blocks are allocated
};

void createBuddyList();

void printBuddyList();

void resetBuddyList();

long getSpace(long size);

long freeSpace(long addr);