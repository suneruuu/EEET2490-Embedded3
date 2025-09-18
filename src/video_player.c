#include "..\includes\video_player.h"

/* No std headers */
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

/* Provided by your framebf.c */
extern unsigned char *fb;
extern unsigned int   width, height, pitch;
void framebf_init(void);

/* ---- Timer (RPi2/3) ---- */
#define PERIPH_BASE  0x3F000000u
#define ST_BASE     (PERIPH_BASE + 0x00003000u)
#define ST_CLO      (*(volatile u32*)(ST_BASE + 0x04))
static inline void delay_us(u32 us){ u32 s=ST_CLO; while((u32)(ST_CLO - s) < us){} }

/* ---- Select video source ---- */
#ifdef USE_CHUNKS
  #include "video_chunks.h"   /* VIDEO_WIDTH/HEIGHT/FPS, VIDEO_TOTAL_FRAMES, video_get_frame() */
  #define FRAMES_TOTAL (VIDEO_TOTAL_FRAMES)
  static inline const u16* get_frame_ptr(int idx){ return video_get_frame((u32)idx); }
#else
  #include "..\includes\video_data.h"     /* VIDEO_WIDTH/HEIGHT/FPS, VIDEO_FRAME_COUNT, video_frames[] */
  #define FRAMES_TOTAL (VIDEO_FRAME_COUNT)
  static inline const u16* get_frame_ptr(int idx){ return &video_frames[(u32)idx * (VIDEO_WIDTH*VIDEO_HEIGHT)]; }
#endif

/* ---- RGB565 -> ARGB8888 ---- */
static inline u32 ex5(u32 v){ return (v<<3)|(v>>2); }
static inline u32 ex6(u32 v){ return (v<<2)|(v>>4); }
static inline u32 rgb565_to_argb32(u16 p){
  u32 r=ex5((p>>11)&0x1F), g=ex6((p>>5)&0x3F), b=ex5(p&0x1F);
  return 0xFF000000u | (r<<16) | (g<<8) | b;
}

/* ---- Unscaled blit ---- */
void video_blit_frame_index(int idx, int x, int y){
  if(idx<0 || idx>=FRAMES_TOTAL) return;
  if(x<0||y<0) return;
  if((u32)(x+VIDEO_WIDTH)>width || (u32)(y+VIDEO_HEIGHT)>height) return;

  const u16* src = get_frame_ptr(idx);
  if(!src) return;

  u32* row=(u32*)(fb + (u32)y*pitch) + (u32)x;
  int stride_px=(int)(pitch/4);
  for(int r=0;r<(int)VIDEO_HEIGHT;++r){
    u32* d=row + r*stride_px;
    const u16* s=src + r*VIDEO_WIDTH;
    for(int c=0;c<(int)VIDEO_WIDTH;++c) d[c]=rgb565_to_argb32(s[c]);
  }
}

/* ---- Scaled blit (nearest) ---- */
void video_blit_scaled_frame_index(int idx, int x, int y, int dst_w, int dst_h){
  if(idx<0 || idx>=FRAMES_TOTAL) return;
  if(dst_w<=0 || dst_h<=0) return;
  if(x<0||y<0) return;
  if((u32)(x+dst_w)>width || (u32)(y+dst_h)>height) return;

  const u16* src = get_frame_ptr(idx);
  if(!src) return;

  u32 step_x = ((u32)VIDEO_WIDTH  << 16) / (u32)dst_w;
  u32 step_y = ((u32)VIDEO_HEIGHT << 16) / (u32)dst_h;

  for(int dy=0; dy<dst_h; ++dy){
    u32 sy_fp = (u32)dy * step_y;
    int sy = (int)(sy_fp >> 16);
    const u16* srow = src + sy * (int)VIDEO_WIDTH;

    u32* drow = (u32*)(fb + (u32)(y+dy)*pitch) + (u32)x;

    u32 sx_fp = 0;
    for(int dx=0; dx<dst_w; ++dx){
      int sx = (int)(sx_fp >> 16);
      drow[dx] = rgb565_to_argb32(srow[sx]);
      sx_fp += step_x;
    }
  }
}

/* ---- API ---- */
void video_play_init(void){
  if(!fb || width==0u || height==0u || pitch==0u) framebf_init();
  
  // Call the new function to play the video in the center of the 800x600 screen
  video_play_fill_center_quad();
}

/* ---- Function to center the video on 800x600 screen ---- */
void video_play_fill_center_quad(void) {
  // Set the destination width and height to the original video resolution
  int dst_w = (int)VIDEO_WIDTH * 3;
  int dst_h = (int)VIDEO_HEIGHT * 3;
  
  // Calculate the position to center the video on the 800x600 screen
  int x = (800 - dst_w) / 2;  // Center horizontally
  int y = (600 - dst_h) / 2;  // Center vertically

  // Ensure the position is within bounds (handle cases where video is larger than screen)
  if (x < 0) x = 0;
  if (y < 0) y = 0;

  // Call the function to blit the frame at the calculated position
  video_play_at_rect(x, y, dst_w, dst_h);
}

/* ---- Function to play the video at specific position and resolution ---- */
void video_play_at_rect(int x, int y, int dst_w, int dst_h){
  u32 us = 1000000u/VIDEO_FPS;
  for(int i=0;i<FRAMES_TOTAL;++i){
    video_blit_scaled_frame_index(i, x, y, dst_w, dst_h);
    delay_us(us);
  }
}
