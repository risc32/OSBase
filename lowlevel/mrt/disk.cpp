#pragma once

#include "asm.cpp"
#include "basic.cpp"

struct ATADriver {
    void read(uint64_t lba, uint32_t count, char* buffer) const {
        wait_bsy();

        outb(base_port + ATA_SECTOR_COUNT, count & 0xFF);
        outb(base_port + ATA_LBA_LOW, lba & 0xFF);
        outb(base_port + ATA_LBA_MID, (lba >> 8) & 0xFF);
        outb(base_port + ATA_LBA_HIGH, (lba >> 16) & 0xFF);
        outb(base_port + ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
        outb(base_port + ATA_COMMAND, 0x20);

        for (uint32_t sector = 0; sector < count; sector++) {
            wait_bsy();
            wait_drq();

            uint16_t temp_buffer[256];
            for (int i = 0; i < 256; i++) {
                temp_buffer[i] = inw(base_port + ATA_DATA);
            }
            memcpy(buffer + (sector * 512), temp_buffer, 512);
        }
    }

    bool write(uint64_t lba, const char* data) const {
        uint32_t count = 1;
        const char* buffer = data;

        wait_bsy();
        outb(base_port + ATA_SECTOR_COUNT, count & 0xFF);
        outb(base_port + ATA_LBA_LOW, lba & 0xFF);
        outb(base_port + ATA_LBA_MID, (lba >> 8) & 0xFF);
        outb(base_port + ATA_LBA_HIGH, (lba >> 16) & 0xFF);
        outb(base_port + ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
        outb(base_port + ATA_COMMAND, 0x30);

        for (uint32_t sector = 0; sector < count; sector++) {
            wait_bsy();
            wait_drq();

            const uint16_t* sector_buffer = (const uint16_t*)(buffer + (sector * 512));
            for (int i = 0; i < 256; i++) {
                outw(base_port + ATA_DATA, sector_buffer[i]);
            }
        }
        return true;
    }

    void wait_bsy() const {
        while (inb(base_port + ATA_STATUS) & 0x80) {}
    }

    void wait_drq() const {
        while (!(inb(base_port + ATA_STATUS) & 0x08)) {}
    }

private:
    uint16_t base_port = 0x1F0;

    enum {
        ATA_DATA = 0,
        ATA_ERROR = 1,
        ATA_FEATURES = 1,
        ATA_SECTOR_COUNT = 2,
        ATA_LBA_LOW = 3,
        ATA_LBA_MID = 4,
        ATA_LBA_HIGH = 5,
        ATA_DRIVE_HEAD = 6,
        ATA_STATUS = 7,
        ATA_COMMAND = 7
    };
};
