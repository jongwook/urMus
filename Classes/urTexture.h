#ifndef __URTEXTURE_H__
#define __URTEXTURE_H__

#include "config.h"
#include "urImage.h"
#include "urFont.h"


#ifndef __APPLE_CC__

typedef enum {
	kTexture2DPixelFormat_Automatic = 0,
	kTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8,
} Texture2DPixelFormat;

typedef enum {
   UITextAlignmentLeft,
   UITextAlignmentCenter,
   UITextAlignmentRight,
} UITextAlignment;

typedef struct tagCGPoint {
	float x;
	float y;
} CGPoint;

typedef struct tagCGSize {
	float width;
	float height;
} CGSize;

typedef struct tagCGRect {
   CGPoint origin;
   CGSize size;
} CGRect;


typedef enum {
   UILineBreakModeWordWrap = 0,
   UILineBreakModeCharacterWrap,
   UILineBreakModeClip,
   UILineBreakModeHeadTruncation,
   UILineBreakModeTailTruncation,
   UILineBreakModeMiddleTruncation,
} UILineBreakMode;

#else
	#include <UIKit/UIKit.h>
#endif



class urTexture
{
protected:
	GLuint name;
	unsigned int width, height;
	unsigned int texWidth, texHeight;
	GLenum format;	// GL_RGB or GL_RGBA
	urFont *font;
	string str;
	GLfloat _maxS, _maxT;	
	bool ownfont;
	vector<string> lines;
	vector<float> widths;
	UITextAlignment alignment;
	UILineBreakMode linebreakmode;
	CGSize shadowOffset;
	GLfloat shadowBlur;
	GLfloat shadowColor[4];
	
	void renderString(CGRect rect);
	void drawString(CGRect rect);
public:
	urTexture(const void *data, GLenum format, unsigned int width, unsigned int height);
	urTexture(urImage *image);
	urTexture(const char *str, const char *fontname, unsigned int size, unsigned int width, unsigned int height,
			  UITextAlignment alignment=UITextAlignmentCenter, UILineBreakMode mode=UILineBreakModeWordWrap,
			  CGSize shadowOffset=CGSizeMake(0,0), GLfloat shadowBlur=0.0f, GLfloat shadowColor[]=NULL);
	~urTexture(void);

	GLuint getName() { return name; }
	unsigned int getWidth() { return width; }
	unsigned int getHeight() { return height; }
	unsigned int getLineHeight() { return (font && font->bLoadedOk)?font->getLineHeight():0; }
	void bind() { glBindTexture(GL_TEXTURE_2D, name); }
	void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
	GLfloat maxS() { return _maxS; }
	GLfloat maxT() { return _maxT; }
	
	void drawInRect(CGRect rect);
	void drawAtPoint(CGPoint point, bool tile);
};

#endif // __URTEXTURE_H__