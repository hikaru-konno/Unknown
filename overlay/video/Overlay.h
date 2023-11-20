#ifndef _Overlay_H_
#define _Overlay_H_

#include <string>

#include <stdio.h>

#include <dynamic_libs/gx2_types.h>

class Overlay
{

public:
    static Overlay *getInstance() {
        if(!instance)
            instance = new Overlay();
        return instance;
    }

    static void destroyInstance() {
        if(instance){
            delete instance;
            instance = NULL;
        }
    }

    static void draw_Image(f32 x,f32 y, float alpha, s32 width, const char *filename)
    {
        Overlay * cur_instance = getInstance();
        if(cur_instance ==  NULL) return;
        cur_instance->draw_Image_private(x, y, alpha, width, filename);
    }

    static void draw_rect(f32 x,f32 y, f32 width, f32 height)
    {
        Overlay * cur_instance = getInstance();
        if(cur_instance ==  NULL) return;
        cur_instance->draw_rect_private(x,y, width, height);
    }

private:
    //!Constructor
    Overlay();
    //!Destructor
    ~Overlay();
    static Overlay *instance;
    void draw_Image_private(f32 x,f32 y, float alpha, s32 width, const char *filename);
    void draw_rect_private(f32 x,f32 y, f32 width, f32 height);
    void init_colorVtxs_img();
    void init_colorVtxs_rect();

    f32 * colorVtxs_img = NULL;
    u8 * colorVtxs = NULL;
};

#endif
