#include <ctype.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define BUFFER_SIZE 128

void read_lsb(unsigned char* img, char* buffer, int width, int height, int channels, int8_t bitOrder, int8_t onlyChannel) {
    char thisBit = bitOrder == -1 ? 7 : 0;
    char thisChar = 0;

    int printed = 0;
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            for(int channel = 0; channel < channels; ++channel) {
                if(onlyChannel > 0 && channel != onlyChannel) continue;

                int colorIndex = (((y*width) + x)*channels)+channel;
                unsigned char lsb = img[colorIndex]&1;

                thisChar |= (lsb << thisBit);
                thisBit += bitOrder;

                if((bitOrder < 0 && thisBit < 0) || (bitOrder > 0 && thisBit >= 8)) {
                    if(isprint(thisChar)) {
                        buffer[printed] = thisChar;
                        printed++;
                        if(printed >= BUFFER_SIZE) return;
                    }
                    thisChar = 0;
                    thisBit = bitOrder == -1 ? 7 : 0;
                }
            }
        }
    }
}

void print_truncated(const char* str) {
    char still_going = 1;
    for(int stringIndex = 0; stringIndex < BUFFER_SIZE; ++stringIndex) {
        char thisChar = str[stringIndex];

        if(thisChar == 0) {
            still_going = 0;
        }
        printf("%c", thisChar);
    }
    if(still_going > 0) printf("...");
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: %s [filename]\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int x, y, channels;
    unsigned char* img = stbi_load(filename, &x, &y, &channels, 4);

    if(img == NULL) {
        printf("Couldn't load %s\n", filename);
        return 2;
    }
    printf("Loaded %s: %dx%d w/ %d channels\n", filename, x, y, channels);

    char buffer[BUFFER_SIZE];
    const char* channelNames = "rgba";

    for(int8_t onlyChannel = -1; onlyChannel < channels; onlyChannel++) {
        for(int8_t bitOrder = -1; bitOrder <= 1; bitOrder += 2) {
            memset(buffer, 0, BUFFER_SIZE);
            read_lsb(img, buffer, x, y, channels, bitOrder, onlyChannel);

            // print params
            printf(bitOrder == -1 ? "high->low" : "low->high");
            printf(" channel: ");
            if(onlyChannel == -1) printf("all");
            else printf("%c  ", channelNames[onlyChannel]);

            printf(" - \t\"");
            print_truncated(buffer);
            printf("\"\n");
        }
    }

    stbi_image_free(img);

    return 0;
}
