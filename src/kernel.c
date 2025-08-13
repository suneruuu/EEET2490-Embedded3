#include "..\library\uart0.h"
#include "..\library\uart1.h"
#include "..\library\mbox.h"
#include "..\library\framebf.h"

void main()
{
 // set up serial console
uart_init();
// say hello
uart_puts("\n.########.########.########.########..#######..##.........#######....#####..");
uart_puts("\n.##.......##.......##..........##....##.....##.##....##..##.....##..##...##.");
uart_puts("\n.##.......##.......##..........##...........##.##....##..##.....##.##.....##");
uart_puts("\n.######...######...######......##.....#######..##....##...########.##.....##");
uart_puts("\n.##.......##.......##..........##....##........#########........##.##.....##");
uart_puts("\n.##.......##.......##..........##....##..............##..##.....##..##...##.");
uart_puts("\n.########.########.########....##....#########.......##...#######....#####..");
uart_puts("\n.########.....###....########..########.....#######...######.");               
uart_puts("\n.##.....##...##.##...##.....##.##..........##.....##.##....##");              
uart_puts("\n.##.....##..##...##..##.....##.##..........##.....##.##......");               
uart_puts("\n.########..##.....##.########..######......##.....##..######.");               
uart_puts("\n.##.....##.#########.##...##...##..........##.....##.......##");               
uart_puts("\n.##.....##.##.....##.##....##..##..........##.....##.##....##");               
uart_puts("\n.########..##.....##.##.....##.########.....#######...######.\n");
uart_puts("\n Developed by Ngo Minh Hieu - s3940891\n");
uart_puts("\n              Nguyen Ngoc Huan - s3979896\n");
uart_puts("\n              Nguyen Le Thuc Quynh - s3924993\n");
uart_puts("\n              Ngo Duc Minh Quan - s3938194\n\n");                                                                      
// Initialize frame buffer
//framebf_init();
// Draw something on the screen
// drawRectARGB32(100,100,400,400,0x00AA0000,1); //RED
// drawRectARGB32(150,150,400,400,0x0000BB00,1); //GREEN
// drawRectARGB32(200,200,400,400,0x000000CC,1); //BLUE
// drawRectARGB32(250,250,400,400,0x00FFFF00,1); //YELLOW
// drawRectARGB32(700,450,1000,700,0x008B008B,1); //purple
// drawRectARGB32(750,500,1000,700,0x00FF8C00,1); //orange
// drawRectARGB32(800,550,1000,700,0x00F7A189,1); //light_salmon
// drawLineARGB32(420, 100, 800, 0x00F7A189);
// drawCircleARGB32(300, 500, 100, 0x00AA0000, 1);
// drawPixelARGB32(300, 300, 0x00FF0000); //RED
// echo everything back
while(1) {
    //read each char
    char c = uart_getc();
    //send back
    uart_sendc(c);
    }
}
