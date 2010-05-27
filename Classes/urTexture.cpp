#include "urTexture.h"


// static font storage
#include <map>
std::map<string,urFont *> fonts;

inline int pow2roundup (int x)
{
    if (x < 0) return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

urTexture::urTexture(const void *data, GLenum format, unsigned int width, unsigned int height)
{
	GLint saveName;
	
	glGenTextures(1, &name);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
	glBindTexture(GL_TEXTURE_2D, name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, saveName);

	this->width=width;
	this->height=height;
	this->format=format;
	
	this->font=NULL;
}

urTexture::urTexture(urImage *image) 
{
	GLint saveName;
	
	switch(image->getColorType()) {
		case PNG_COLOR_TYPE_RGB:
			format=GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGBA:
			format=GL_RGBA;
			break;
	}

	width=image->getWidth();
	height=image->getHeight();
	texWidth=pow2roundup(width);
	texHeight=pow2roundup(height);
	
	image->resize(texWidth, texHeight);
	const void* data=image->getBuffer();
	
	_maxS=(GLfloat)width/texWidth;
	_maxT=(GLfloat)height/texHeight;
	

	glGenTextures(1, &name);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
	glBindTexture(GL_TEXTURE_2D, name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, saveName);

	this->format=format;
	
	this->font=NULL;
}

urTexture::urTexture(const char *str, const char *fontname, unsigned int size, unsigned int width, unsigned int height) {
	name=0;
	
	char fontsizestr[16];
	sprintf(fontsizestr, " %d", size);
	string key=fontname;
	key += fontsizestr;
	if(fonts.find(key)==fonts.end()) {
		fonts[key]=font=new urFont();
		font->loadFont(fontname,size,true,true,true);
		font->key=key;
		font->refCount++;
	} else {
		font=fonts[key];
		font->refCount++;
	}
	
	this->width=width;
	this->height=height;
	this->str=str;
}

urTexture::urTexture(const char *str, urFont *font, unsigned int width, unsigned int height) {
	name=0;
	ownfont=false;
	this->font=font;
	font->refCount++;
	
	this->width=width;
	this->height=height;
	this->str=str;
}

urTexture::~urTexture(void)
{
	if(name)
		glDeleteTextures(1, &name);
	
	if(font) {
		font->refCount--;
		if(font->refCount==0) {
			fonts.erase(font->key);
			delete font;
		}
	}
}


void urTexture::drawInRect(CGRect rect) {
	if(name) {	// it's an image
		//GLfloat  coordinates[] = { 0,_maxT, _maxS,_maxT, 0,0, _maxS,0};
		//GLfloat coordinates[] = { 0,1, 1,1, 0,0, 1,0 };
		GLfloat vertices[] = {  rect.origin.x, rect.origin.y, 0.0,
			rect.origin.x + rect.size.width, rect.origin.y, 0.0,
			rect.origin.x, rect.origin.y + rect.size.height, 0.0,
		rect.origin.x + rect.size.width, rect.origin.y + rect.size.height, 0.0 };
	
		GLint saveName;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
		glBindTexture(GL_TEXTURE_2D, name);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, saveName);
	}
	if(font && font->bLoadedOk) {	// it's a text
		font->drawString(str,rect.origin.x,rect.origin.y);
	}
}

void urTexture::drawAtPoint(CGPoint point, bool tile) {
	
	if(name) {	// it's an image
		GLfloat	coordinates[] = { 0,_maxT, _maxS,_maxT, 0,0, _maxS,0};
		GLfloat	vertices[] = {  point.x,			point.y,			0.0,
								width + point.x,	point.y,			0.0,
								point.x,			height + point.y,	0.0,
								width + point.x,	height + point.y,	0.0  };
		
		GLint saveName;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
		glBindTexture(GL_TEXTURE_2D, name);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, saveName);
	}
	if(font && font->bLoadedOk) {	// it's a text
		font->drawString(str,point.x,point.y+height*0.5);
	}
}

