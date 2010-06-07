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
	
	int internalFormat=1;
	switch(image->getColorType()) {
		case PNG_COLOR_TYPE_RGB:
			format=GL_RGB;
			internalFormat=GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGBA:
			format=GL_RGBA;
			internalFormat=GL_RGBA;
			break;
		case PNG_COLOR_TYPE_BGRA:
			format=GL_BGRA;
			internalFormat=GL_RGBA;
			//image->flipPixels();
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
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, saveName);

	this->format=format;
	
	this->font=NULL;
}

urTexture::urTexture(const char *str, const char *fontname, unsigned int size, unsigned int width, unsigned int height,
					 UITextAlignment alignment, UILineBreakMode mode, CGSize shadowOffset, GLfloat shadowBlur, GLfloat shadowColor[]) {
	name=0;
	
	char fontsizestr[16];
	sprintf(fontsizestr, " %d", size);
	string key=fontname;
	key += fontsizestr;
	if(fonts.find(key)==fonts.end()) {
		fonts[key]=font=new urFont();
		font->loadFont(fontname,size*7/10,true,true,true);
		font->key=key;
		font->refCount++;
	} else {
		font=fonts[key];
		font->refCount++;
	}
	
	this->width=width;
	this->height=height;
	this->str=str;
	
	this->alignment=alignment;
	this->linebreakmode=mode;
	this->shadowOffset=shadowOffset;
	this->shadowBlur=shadowBlur;
	this->shadowColor[0]=(shadowColor)?shadowColor[0]:0;
	this->shadowColor[1]=(shadowColor)?shadowColor[1]:0;
	this->shadowColor[2]=(shadowColor)?shadowColor[2]:0;
	this->shadowColor[3]=(shadowColor)?shadowColor[3]:0;
	
	
	
	char *copy=new char[this->str.length()+1], *rptr;
	strcpy(copy, str);
	char *line=strtok_r(copy, "\n", &rptr);
	while(line!=NULL) {
		lines.push_back(line);
		widths.push_back(font->getLineWidth(line)+5);
		NSLog(@"str:%s, line : %s, width : %f", copy, line, font->getLineWidth(line));
		line=strtok_r(NULL, "\n", &rptr);
	}
	delete [] copy;
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
		drawString(rect);
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
		drawString(CGRectMake(point.x,point.y,width,height));
	}
}

void urTexture::renderString(CGRect rect) {
	float lineHeight=font->getLineHeight();
	float width=rect.size.width;
	float height=rect.size.height+6;
	int nLines=lines.size();
	for(int i=0;i<nLines;i++) {
		float offsetX, offsetY=(height+lineHeight*(nLines-2*i-2))*0.5;
		if(alignment==UITextAlignmentLeft) 
			offsetX=0;
		if(alignment==UITextAlignmentCenter) 
			offsetX=(width-widths[i])*0.5;
		if(alignment==UITextAlignmentRight) 
			offsetX=width-widths[i];
		font->drawString(lines[i], rect.origin.x+offsetX, rect.origin.y+offsetY);
	}
}

void urTexture::drawString(CGRect rect) {
	if(shadowBlur>0.0f) {
		float color[4];
		glGetFloatv(GL_CURRENT_COLOR, color);
		int repeat=ceil(shadowBlur);
		GLfloat alpha=0.1f/(repeat*2+1)/(repeat*2+1);
		
		for(int i=-repeat;i<=repeat;i++) {
			for(int j=-repeat;j<=repeat;j++) {
				glPushMatrix();
				glTranslatef(i, j, 0);
				glColor4f(shadowColor[0], shadowColor[1], shadowColor[2], shadowColor[3]*alpha);
				renderString(rect);
				glPopMatrix();
			}
		}
		
		
		glColor4f(color[0],color[1],color[2],color[3]);
	}
	renderString(rect);
}

