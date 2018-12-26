/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#define FAT_CHUNK_SIZE                          512
#define JPEG_VALID_FILE_HEADER_MASK             0xe0ffd8ffU

#define JPEG_OUTPUT_FILE_NAME_LENGTH            8
#define JPEG_OUTPUT_FILE_NAME_FORMAT            "%03d.jpg"
#define FILE_LIMIT                              1000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    // open the card image
    FILE* fCard = fopen("card.raw", "rb");

    if (fCard == NULL) {
        printf("Ow-ow, looks like I was unable to open card.raw!\n");
        return 1;
    }

    unsigned char dataBuffer[FAT_CHUNK_SIZE];
    char nameBuffer[JPEG_OUTPUT_FILE_NAME_LENGTH];

    bool isTrackingJpg = false;
    int  foundCount    = 0;

    FILE* currJpgFile  = NULL;

    int blocks = 1;

    while (fread(dataBuffer, FAT_CHUNK_SIZE, blocks, fCard) == blocks || !feof(fCard)) {

        // we want to make sure it passes for check50 and
        // that we eliminate a possible security breach if
        // someone passes card.raw with more than 1000 photos
        // on it.
        //
        // that would in turn have sprintf(...) overwrite past
        // the nameBuffer's boundaries.
        //
        // ideally, we would dynamically allocate more memory
        // for the nameBuffer in case we hit the milestone of
        // 1000 files.
        if (foundCount >= FILE_LIMIT) {
            printf("Maximum number of %i files supported. Exiting...", FILE_LIMIT);
            return 2;
        }

        if (((*(unsigned int*)dataBuffer) & JPEG_VALID_FILE_HEADER_MASK) == JPEG_VALID_FILE_HEADER_MASK)
        {
            isTrackingJpg = true;
            if (currJpgFile != NULL) {
                fclose(currJpgFile);
            }

            sprintf(nameBuffer, JPEG_OUTPUT_FILE_NAME_FORMAT, foundCount);
            currJpgFile = fopen(nameBuffer, "wb");
            if (currJpgFile == NULL) {
                printf("Unable to create file %s", nameBuffer);
                return 3;
            }
            ++foundCount;

        }
        if (isTrackingJpg) {
            fwrite(dataBuffer, FAT_CHUNK_SIZE, blocks, currJpgFile);
        }
    }

    // close the jpg if was open
    if (currJpgFile != NULL) {
        fclose(currJpgFile);
    }

    // close the card image
    fclose(fCard);

    return 0;
}