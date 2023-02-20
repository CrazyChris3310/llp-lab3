#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include "page.h"

struct FileHeader {
    struct PossibleValue tableOfTables;
    struct PossibleValue tableOfColumns;
    struct PossibleValue freePages;
    size_t next_table_id;
};

struct FileManager {
    FILE* file;
    char* filename;
    size_t blockSize;
    bool isNew;
    struct FileHeader header;
};

void writeFileHeader(struct FileManager* fm);

struct FileManager* createFileManager(char* filename, size_t blockSize);

void destoryFileManager(struct FileManager* manager);

int8_t readPage(struct FileManager* fm, size_t blockId, struct Page* page);
int8_t writePage(struct FileManager* fm, size_t blockId, struct Page* page);

size_t addNewBlock(struct FileManager* fm, struct PageHeader* header);
size_t writeNewPage(struct FileManager *fm, struct Page* page);

size_t getFileLength(struct FileManager* fm);

void clearFile(struct FileManager* fm);
#endif