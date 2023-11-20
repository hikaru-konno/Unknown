#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dynamic_libs/gx2_functions.h>
#include "shaders/ColorShader.h"
#include "shaders/FXAAShader.h"
#include "shaders/Shader3D.h"
#include "shaders/ShaderFractalColor.h"
#include "shaders/Texture2DShader.h"
#include "Overlay.h"
#include <map>
//#include "../gui/Gui.h"
#include "../resources/Resources.h"
#include "../gui/GuiImageData.h"

Overlay *Overlay::instance = NULL;

Overlay::Overlay()
{
    init_colorVtxs_img();
    init_colorVtxs_rect();
}

Overlay::~Overlay()
{
    //! destroy shaders
    ColorShader::destroyInstance();
    FXAAShader::destroyInstance();
    Shader3D::destroyInstance();
    ShaderFractalColor::destroyInstance();
    Texture2DShader::destroyInstance();
    if(this->colorVtxs_img){
        free(this->colorVtxs_img);
        this->colorVtxs_img = NULL;
    }
    if(this->colorVtxs){
        free(this->colorVtxs);
        this->colorVtxs = NULL;
    }
}

void Overlay::init_colorVtxs_img(){
    if(!this->colorVtxs_img){
        this->colorVtxs_img = (f32*)memalign(0x40, sizeof(f32) * 16);
        if(this->colorVtxs_img == NULL) return;

    }
    memset(this->colorVtxs_img,0xFF,16*sizeof(f32));

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, this->colorVtxs_img, 16 * sizeof(f32));
}

void Overlay::init_colorVtxs_rect(){
    if(!this->colorVtxs){
        this->colorVtxs = (u8*)memalign(0x40, sizeof(u8) * 16);
        if(this->colorVtxs == NULL) return;

    }
    memset(this->colorVtxs,0xFF,16*sizeof(u8));

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, this->colorVtxs, 16 * sizeof(u8));
}


void Overlay::draw_Image_private(f32 x,f32 y, float alpha, s32 width, const char *filename)
{
    if(this->colorVtxs_img == NULL){
        init_colorVtxs_img();
        return;
    }

    f32 widthScaleFactor = 1.0f / (f32)1280;
    f32 heightScaleFactor = 1.0f / (f32)720;

    glm::vec3 positionOffsets = glm::vec3(0.0f);

    positionOffsets[0] = (x-((1280)/2)+(width/2)) * widthScaleFactor * 2.0f;
    positionOffsets[1] = -(y-((720)/2)+(width/2)) * heightScaleFactor * 2.0f;

    glm::vec3 scale(width*widthScaleFactor,width*heightScaleFactor,1.0f);

    GuiImageData* imgData = Resources::GetImageData(filename);
    Texture2DShader::instance()->setShaders();
    Texture2DShader::instance()->setAttributeBuffer(this->colorVtxs_img, NULL, 4);
    Texture2DShader::instance()->setAngle(0);
    Texture2DShader::instance()->setOffset(positionOffsets);
    Texture2DShader::instance()->setScale(scale);
    Texture2DShader::instance()->setColorIntensity(glm::vec4(1.0f, 1.0f, 1.0f, alpha/255.0f));
    Texture2DShader::instance()->setBlurring(glm::vec3(0.0f));
    Texture2DShader::instance()->setTextureAndSampler(imgData->getTexture(), imgData->getSampler());
    Texture2DShader::instance()->draw(GX2_PRIMITIVE_QUADS, 4);
}


void Overlay::draw_rect_private(f32 x,f32 y, f32 width, f32 height)
{
    if(this->colorVtxs == NULL){
        init_colorVtxs_rect();
        return;
    }

    f32 widthScaleFactor = 1.0f / (f32)1280;
    f32 heightScaleFactor = 1.0f / (f32)720;

    //s32 width = 40;

    glm::vec3 positionOffsets = glm::vec3(0.0f);

    positionOffsets[0] = (x-((1280)/2)+(width/2)) * widthScaleFactor * 2.0f;
    positionOffsets[1] = -(y-((720)/2)+(height/2)) * heightScaleFactor * 2.0f;

    glm::vec3 scale(width*widthScaleFactor,height*heightScaleFactor,1.0f);

    ColorShader::instance()->setShaders();
    ColorShader::instance()->setAttributeBuffer(this->colorVtxs, NULL, 4);
    ColorShader::instance()->setAngle(0);
    ColorShader::instance()->setOffset(positionOffsets);
    ColorShader::instance()->setScale(scale);
    ColorShader::instance()->setColorIntensity(glm::vec4(130, 79, 255, 1.0f));
    ColorShader::instance()->draw(GX2_PRIMITIVE_QUADS, 4);
}
