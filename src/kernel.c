#include "..\includes\uart0.h"
#include "..\includes\uart1.h"
#include "..\includes\mbox.h"
#include "..\includes\framebf.h"
#include "..\includes\color.h"
#include "../assets/images/hello.c"
#include "../assets/images/cinema.c"


#define Welcome_msg \
"\n"\
":::::::::: :::::::::: :::::::::: ::::::::::: ::::::::   :::     ::::::::   :::::::\n"\
":+:        :+:        :+:            :+:    :+:    :+: :+:     :+:    :+: :+:   :+:\n"\
"+:+        +:+        +:+            +:+          +:+ +:+ +:+  +:+    +:+ +:+  :+:+\n"\
"+#++:++#   +#++:++#   +#++:++#       +#+        +#+  +#+  +:+   +#++:++#+ +#+ + +:+\n"\
"+#+        +#+        +#+            +#+      +#+   +#+#+#+#+#+       +#+ +#+#  +#+\n"\
"#+#        #+#        #+#            #+#     #+#          #+#  #+#    #+# #+#   #+#\n"\
"########## ########## ##########     ###    ##########    ###   ########   #######\n"\
"\n"\
" ::::::::  :::    :::     :::      ::::::::   ::::::::\n"\
":+:    :+: :+:    :+:   :+: :+:   :+:    :+: :+:    :+:\n"\
"+:+        +:+    +:+  +:+   +:+  +:+    +:+ +:+\n"\
"+#+        +#++:++#++ +#++:++#++: +#+    +:+ +#++:++#++\n"\
"+#+        +#+    +#+ +#+     +#+ +#+    +#+        +#+\n"\
"#+#    #+# #+#    #+# #+#     #+# #+#    #+# #+#    #+#\n"\
" ########  ###    ### ###     ###  ########   ########\n"\
"\n"\
"Developed by Ngo Minh Hieu - s3940891"\
"\n"\
"             Nguyen Ngoc Huan - s3979896"\
"\n"\
"             Nguyen Le Thuc Quynh - s3924993"\
"\n"\
"             Ngo Duc Minh Quan - s3938194\n"\

#define name_msg1 \
"Ngo Minh Hieu - s3940891"\

#define name_msg2 \
"Nguyen Ngoc Huan - s3979896"\

#define name_msg3 \
"Nguyen Le Thuc Quynh - s3924993"\

#define name_msg4 \
"Ngo Duc Minh Quan - s3938194"\

void main()
{
 // set up serial console
uart_init();
// say hello

uart_puts(Welcome_msg);                                                             
// Initialize frame buffer
framebf_init();
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
//drawString(50, 613, Welcome_msg, 0x00AA0000, 2);
drawImage(hello, 0, 0, 800, 600);
wait_msec(1000);
ClearScreen();
drawImage(cinema, 0, 0, 800, 600);
drawString(300, 100, "Team 4", 0x0000BB00, 2);
drawString(150, 130, name_msg1, 0x00AA0000, 2);
drawString(150, 160, name_msg2, 0x000000CC, 2);
drawString(150, 190, name_msg3, 0x00FF8C00, 2);
drawString(150, 220, name_msg4, 0x00F7A189, 2);
wait_msec(3000);
ClearScreen();

// echo everything back
while(1) {
    //read each char
    char c = uart_getc();
    //send back
    uart_sendc(c);
    }
}
