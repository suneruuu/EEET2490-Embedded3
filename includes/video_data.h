#ifndef VIDEO_DATA_H
#define VIDEO_DATA_H

/* Minimal fixed-width aliases (no standard headers). */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/* Provide uint16_t alias so existing video_data.c compiles without <stdint.h>. */
#ifndef __UINT16_T_DEFINED
typedef unsigned short uint16_t;
#define __UINT16_T_DEFINED
#endif

/* Dimensions and timing must match the generated C frames you have. */
#define VIDEO_WIDTH        160
#define VIDEO_HEIGHT       120
#define VIDEO_FPS          12
#define VIDEO_FRAME_COUNT  116

/* Packed RGB565 frames: frame i starts at i*(W*H). */
extern const uint16_t video_frames[VIDEO_FRAME_COUNT * VIDEO_WIDTH * VIDEO_HEIGHT];

#endif /* VIDEO_DATA_H */
