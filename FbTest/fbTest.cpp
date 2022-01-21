#include <unistd.h>  
#include <stdio.h>  
#include <fcntl.h>  
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <stdlib.h>  
#include <string.h>

#define RED     0x00FF0000
#define GREEN   0x0000FF00
#define BLUE    0x000000FF
#define WHITE   0x00FFFFFF 
#define BLACK   0x00000000
#define SPECIAL 0x0082B6E1

#define ARGB8888_TO_ABGR8888(x) \
    ((((x) &   0xFF) << 16)  |  \
      ((x) & 0xFF00)         |  \
     (((x) & 0xFF0000) >> 16)|  \
      ((x) & 0xFF000000))

struct fb_var_screeninfo vinfo; 
struct fb_fix_screeninfo finfo;

typedef struct self_fb_info{
    int width;
    int height;
    int virtual_width;
    int virtual_height;
    int bitspp;
} self_fb_info;

self_fb_info fb_info;

void fb_display(int color, int fd){
    // Add mapping
    unsigned int *fbp = (unsigned int *) mmap(0, fb_info.virtual_width * fb_info.height * fb_info.bitspp / 8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 

    // Draw color
    for(int i = 0; i < fb_info.virtual_width * fb_info.height; i++){
        *fbp = ARGB8888_TO_ABGR8888(color);
        fbp++;
    }

    // Display frame one
    vinfo.xoffset = 0;
    vinfo.yoffset = 0;

    // Flash 
    ioctl(fd, FBIOPAN_DISPLAY, &vinfo);

    // Remove mapping
    munmap(fbp, fb_info.virtual_width * fb_info.height * fb_info.bitspp / 8);
}

int fb_init(int fd){
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    if(vinfo.bits_per_pixel != 32) {
        printf("Color resolution = %i Bits per Pixel\n", vinfo.bits_per_pixel);
        printf("Please change the color depth to 32 bits per pixel\n");
        return 1;
    }
    fb_info.width  = vinfo.xres;
    fb_info.height = vinfo.yres;
    fb_info.virtual_width = vinfo.xres_virtual;
    fb_info.virtual_height = vinfo.yres_virtual;
    fb_info.bitspp = vinfo.bits_per_pixel;
    printf("visible width: %d, visible height: %d, bitspp: %d\nvirtual width: %d, virtual height: %d\nred_offset: %d, green_offset: %d, blue_offset: %d\n",
        fb_info.width, fb_info.height, fb_info.bitspp, \
        fb_info.virtual_width, fb_info.virtual_height, \
        vinfo.red.offset, vinfo.green.offset, vinfo.blue.offset);
    return 0;
}

int main ()   
{  
    int fd;
    fd = open("/dev/graphics/fb0", O_RDWR);  

    if(fd < 0) {  
        printf("Error : Can not open framebuffer device\n");  
        return -1; 
    }

    if(fb_init(fd)){
        printf("framebuffer initial error\n");
        close (fd);
        return -1;
    }

    fb_display(SPECIAL, fd);
    close (fd);
    return 0;
}