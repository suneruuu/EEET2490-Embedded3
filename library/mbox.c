// -----------------------------------mbox.c -------------------------------------
#include "includes/library/mbox.h"
#include "includes/library/gpio.h"
#include "includes/peripheral/uart1.h"
#include "includes/peripheral/uart0.h"
#include "includes/utils/color.h"

void uart_hex(unsigned int num);

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

static void write_hex_byte_upper(unsigned int value, char *out)
{
    const char *hex = "0123456789ABCDEF";
    out[0] = hex[(value >> 4) & 0xF];
    out[1] = hex[value & 0xF];
}

static void write_u32_hex8_upper(unsigned int value, char *out)
{
    for (int pos = 28, i = 0; pos >= 0; pos -= 4, ++i)
    {
        unsigned int digit = (value >> pos) & 0xF;
        out[i] = (digit > 9) ? (char)('A' + (digit - 10)) : (char)('0' + digit);
    }
}

/**
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = MBOX0_READ;
    } while ((res & 0xF) != channel);

    return res;
}

/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
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
    // Print Buffer Address here once per mailbox call
    print_color("[INFO] ", CYAN);
    uart_puts("Buffer Address: ");
    char addr_hex[11];
    addr_hex[0] = '0';
    addr_hex[1] = 'x';
    write_u32_hex8_upper(buffer_addr, &addr_hex[2]);
    addr_hex[10] = '\0';
    print_color(addr_hex, HI_BLACK);
    uart_puts("\n");

    // Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);

    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            uart_puts("Got successful response\n");

        return (mBuf[1] == MBOX_RESPONSE);
    }

    return 0;
}

/**
 * Get Mac Adress Information
 */

void get_mac_address()
{
    mBuf[0] = 8 * 4;        // Length of buffer
    mBuf[1] = MBOX_REQUEST; // Request code
    mBuf[2] = 0x00010003;   // Tag: MAC address
    mBuf[3] = 6;            // Buffer size
    mBuf[4] = 0;            // Request code
    mBuf[5] = 0;            // Value buffer
    mBuf[6] = 0;
    mBuf[7] = MBOX_TAG_LAST; // End tags

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        unsigned int hi = mBuf[5], lo = mBuf[6]; // firmware writes the MAC into 2 slots of mBuf

        char mac[32];
        int idx = 0;
        unsigned int bytes[6] = {
            (hi) & 0xFF,
            (hi >> 8) & 0xFF,
            (hi >> 16) & 0xFF,
            (hi >> 24) & 0xFF,
            (lo) & 0xFF,
            (lo >> 8) & 0xFF
        };
        for (int i = 0; i < 6; i++) {
            write_hex_byte_upper(bytes[i], &mac[idx]);
            idx += 2;
            if (i < 5) mac[idx++] = ':';
        }
        mac[idx] = '\0';

        print_color("[INFO] ", CYAN);
        uart_puts("Board MAC Address: ");
        print_color(mac, GREEN);
        uart_puts("\n\n");
    }
    else
    {
        print_color("[FAIL] ", RED);
        uart_puts("Unable to fetch MAC address. Check your network\n\n");
    }
}

/**
 * Get Board Revision
 */

void get_board_revision()
{
    mBuf[0] = 7 * 4;        // Length of buffer
    mBuf[1] = MBOX_REQUEST; // Request code

    mBuf[2] = 0x00010002;    // Tag: Board revision
    mBuf[3] = 4;             // Buffer size
    mBuf[4] = 0;             // Request code
    mBuf[5] = 0;             // Value buffer
    mBuf[6] = MBOX_TAG_LAST; // End tags

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        char rev[16];
        rev[0] = '0';
        rev[1] = 'x';
        write_u32_hex8_upper(mBuf[5], &rev[2]);
        rev[10] = '\0';

        print_color("[INFO] ", CYAN);
        uart_puts("Board Revision: ");
        print_color(rev, YELLOW);
        uart_puts("\n\n");
    }
    else
    {
        print_color("[FAIL] ", RED);
        uart_puts("Unable to fetch Board Revision\n");
    }
}