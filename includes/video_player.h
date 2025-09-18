#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

/* No std headers */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/* You supply (from framebf.c): fb, width, height, pitch, framebf_init() */

#ifdef __cplusplus
extern "C" {
#endif

/* Ensure framebuffer is initialized (calls framebf_init() if needed). */
void video_play_init(void);

/* Draw one frame i at (x,y) without scaling. */
void video_blit_frame_index(int frame_index, int x, int y);

/* Nearest-neighbor scale: draw frame i into rectangle (x,y, dst_w, dst_h). */
void video_blit_scaled_frame_index(int frame_index, int x, int y, int dst_w, int dst_h);

/* Play the whole video once into the given rectangle (scaled to fit). */
void video_play_at_rect(int x, int y, int dst_w, int dst_h);

/* Convenience: play once scaled to bottom-right quadrant (width/2 x height/2). */
void video_play_fill_bottom_right_quad(void);

/* New function to center the video and scale it by 3x. */
void video_play_fill_center_quad(void);

#ifdef __cplusplus
}
#endif
#endif /* VIDEO_PLAYER_H */
