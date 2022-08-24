#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct __attribute__((packed)) HWID {
    uint16_t size;
    uint16_t version;
    uint16_t instances[9];
    uint16_t dock_or_PCMCIA;
    uint16_t hashRAM;
    uint16_t hashBIOS;
    uint16_t instanceHashes[];
};

struct __attribute__((packed)) TIMEWEIGHT {
    uint8_t num_weights;
    struct __attribute__((packed)) Weight {
        uint8_t type;
        uint16_t weight;
    } weight[];
};

const char *CLASS_NAMES[] = {
    "CDROM",
    "Hard Disk Controllers",
    "Hard Disk Drives",
    "Displays",
    "SCSI Adapters",
    "PCMCIA",
    "Audio Adapters",
    "Dock",
    "Network Interface Cards",
    "CPUs",
    "Memory",
    "UNUSED",
    "BIOS",
    "UNUSED",
    "Mobile Broadband",
    "Bluetooth Interfaces"
};

const char *HWID_NAMES[] = {
    "CDROM / Mobile Broadband",
    "Hard Disk Controllers",
    "Hard Disk Drives",
    "Displays",
    "SCSI Adapters / Bluetooth Controllers",
    "Audio Adapters",
    "Unused",
    "Network Interface Cards",
    "CPUs"
};

void print_block(uint8_t *hw_block) {
    uint32_t size = *(uint32_t *)hw_block;
    uint16_t threshold = *(uint16_t *)(hw_block + 4); // just an educated guess for now
    unsigned instance_count = 0;

    puts("Block Information:");
    printf("Size      : [%08x]\n", size);
    printf("Threshold : [%08x]\n", threshold);
    puts("");

    struct HWID *hwid = (struct HWID *)(hw_block + 6);
    puts("HWID Information:");
    printf("Size        : [%04x]\n", hwid->size);
    printf("Version     : [%04x]\n", hwid->version);
    for(size_t i = 0; i < 9; i++) {
        instance_count += hwid->instances[i];
        printf("Inst. (%02zx)  : [%04x] -> %s\n", i, hwid->instances[i], HWID_NAMES[i]);
    }
    printf("Dock/PCMCIA : [%04x]\n", hwid->dock_or_PCMCIA);
    printf("RAM hash    : [%02x]\n", hwid->hashRAM);
    printf("SMBIOS hash : [%04x]\n", hwid->hashBIOS);
    for(size_t i = 0; i < instance_count; i++) {
        printf("Hash (%02zx)   : [%04x] %s\n", i, hwid->instanceHashes[i], (hwid->instanceHashes[i] & 1) == 1 ? "[[Non-Removable]]" : "");
    }
    puts("");

    struct TIMEWEIGHT *tw = (struct TIMEWEIGHT *) ((hw_block + 6) + hwid->size);
    puts("Timeweight Information:");
    printf("Weights     : [%02x]\n", tw->num_weights);

    for(size_t i = 0; i < tw->num_weights; i++) {
        printf("Weight (%02x) : [%04x] -> %s\n", tw->weight[i].type, tw->weight[i].weight, CLASS_NAMES[tw->weight[i].type]);
    }

}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        puts("Invalid parameters.\nUsages:\n\thwid file");
        exit(1);
    }
    
    FILE *fd = fopen(argv[1], "r");
    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    rewind(fd);

    uint8_t *hw_block = malloc(fsize);
    fread(hw_block, 1, fsize, fd);

    print_block(hw_block);

    return 0;
}