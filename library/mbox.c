// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"
#include "gpio.h"
#include "../peripheral/uart1.h"
#include "../peripheral/uart0.h"
#include "../utils/color.h"

/* Mailbox Data Buffer (each element is 32-bit)*/
/*
* The keyword attribute allows you to specify special attributes
*
* The aligned(N) attribute aligns the current data item on an address
* which is a multiple of N, by inserting padding bytes before the data item
*
* __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
* *
* We must ensure that our our buffer is located at a 16 byte aligned address,
* so only the high 28 bits contain the address
* (last 4 bits is ZERO due to 16 byte alignment)
*
*/
volatile unsigned int __attribute__((aligned(16))) mBuf[36];

/**
* Read from the mailbox
*/
uint32_t mailbox_read(unsigned char channel)
{
    //Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do {
        // Make sure there is mail to receive
        do {
            asm volatile("nop");
        } while (MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = MBOX0_READ;
    } while ( (res & 0xF) != channel);

    return res;
}

/**
* Write to the mailbox
*/
void mailbox_send(uint32_t msg, unsigned char channel)
{
    //Sending message is buffer_addr & channel number
    // Make sure you can send mail
    do {
        asm volatile("nop");
    } while (MBOX1_STATUS & MBOX_FULL);
    // send the message
    MBOX1_WRITE = msg;
}

/**
* Make a mailbox call. Returns 0 on failure, non-zero on success
*/
int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
    //Check Buffer Address
    uart_puts("Buffer Address: ");
    uart_hex(buffer_addr);
    uart_sendc('\n');

    //Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);

    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel)) {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            uart_puts("Got successful response \n");

        return (mBuf[1] == MBOX_RESPONSE);
    }

    return 0;
}

/**
* Get Mac Adress Information
*/

void get_mac_address(){
    mBuf[0] = 8 * 4;        // Length of buffer
    mBuf[1] = MBOX_REQUEST; //Request code
    mBuf[2] = 0x00010003;   // Tag: MAC address
    mBuf[3] = 6;            //Buffer size
    mBuf[4] = 0;            //Request code
    mBuf[5] = 0;            //Value buffer
    mBuf[6] = 0;           
    mBuf[7] = MBOX_TAG_LAST;//End tags 

    if (mbox_call((unsigned int)mBuf, MBOX_CH_PROP)) {
        unsigned int hi = mBuf[5], lo = mBuf[6];    //firmware writes the MAC into 2 slots of mBuf

        char mac[32];   //character array to store MAC
        sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
                (hi)&0xFF, (hi>>8)&0xFF, (hi>>16)&0xFF, (hi>>24)&0xFF,
                (lo)&0xFF, (lo>>8)&0xFF); //write this formatted string to mac

        print_color("[INFO] ", CYAN);
        uart_puts("Board MAC Address → ");
        print_color(mac, GREEN);
        uart_puts("\n");
    } else {
        print_color("[FAIL] ", RED);
        uart_puts("Unable to fetch MAC address\n");
    }
}

/**
* Get Board Revision
*/

void get_board_revision(){
    mBuf[0] = 7 * 4;        //Length of buffer
    mBuf[1] = MBOX_REQUEST; //Request code

    mBuf[2] = 0x00010002;   // Tag: Board revision
    mBuf[3] = 4;            //Buffer size
    mBuf[4] = 0;            //Request code
    mBuf[5] = 0;            //Value buffer
    mBuf[6] = MBOX_TAG_LAST;//End tags

    if (mbox_call((unsigned int)mBuf, MBOX_CH_PROP)) {
        char rev[16];
        sprintf(rev, "0x%08X", mBuf[5]);

        print_color("[INFO] ", CYAN);
        uart_puts("Board Revision → ");
        print_color(rev, YELLOW);
        uart_puts("\n");
    } else {
        print_color("[FAIL] ", RED);
        uart_puts("Unable to fetch Board Revision\n");
    }
}