#ifndef _OF_TTF_FONT_H_
#define _OF_TTF_FONT_H_

#include "config.h"

// core: ---------------------------
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>  //for ostringsream
#include <iomanip>  //for setprecision



using namespace std;



// from ofTypes.h


//----------------------------------------------------------
// urPoint
//----------------------------------------------------------
class urPoint {
  public:
	virtual ~urPoint(){}

    urPoint( float _x=0.0f, float _y=0.0f, float _z=0.0f ) {
        x = _x;
        y = _y;
        z = _z;
    }

    urPoint( const urPoint & pnt) {
        x = pnt.x;
        y = pnt.y;
        z = pnt.z;
    }

    void set(float _x, float _y, float _z = 0){
        x = _x;
        y = _y;
        z = _z;
    }

    urPoint operator+( const urPoint& pnt ) const {
        return urPoint( x+pnt.x, y+pnt.y, z+pnt.z );
    }

	urPoint & operator+=( const urPoint& pnt ) {
        x+=pnt.x;
        y+=pnt.y;
        z+=pnt.z;
        return *this;
    }

    urPoint operator-(const urPoint& pnt) const {
        return urPoint( x-pnt.x, y-pnt.y, z-pnt.z );
    }

    urPoint operator*(const float& val) const {
        return urPoint( x*val, y*val, z*val);
    }

    urPoint & operator*=( const float & val ) {
        x*=val;
        y*=val;
        z*=val;
        return *this;
    }

    urPoint operator/( const float &val ) const {
		if( val != 0){
			return urPoint( x/val, y/val, z/val );
		}
        return urPoint(x, y, z );
    }

    urPoint& operator/=( const float &val ) {
		if( val != 0 ){
			x /= val;
			y /= val;
			z /= val;
		}
		return *this;
    }

	// union allows us to access the coordinates through
    // both an array 'v' and 'x', 'y', 'z' member varibles
    union  {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };
};


// from ofUtils.h
int 	ofNextPow2(int input);
string 	ofToDataPath(string path, bool absolute=false);
void	ofSetDataPathRoot( string root );


//--------------------------------------------------
typedef struct {
	int value;
	int height;
	int width;
	int setWidth;
	int topExtent;
	int leftExtent;
	float tTex;
	float vTex;		//0-1 pct of bitmap...
	float xOff;
	float yOff;
} charProps;

class ofTTFContour{
	public:
		vector <urPoint>pts;
};

class ofTTFCharacter{
	public:
		vector <ofTTFContour> contours;
};

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.

class urFont{
public:
	urFont();
	virtual ~urFont();
	
	void		clone(urFont *copy);
	void 		loadFont(string filename, int fontsize);
	void 		loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours = false);

	bool		bLoadedOk;
	bool 		bAntiAlised;
	bool 		bFullCharacterSet;

	float 		getLineHeight() {return lineHeight;}
	float		getLineWidth(const char *line);
	void 		setLineHeight(float height) { lineHeight = height; }
	
	void 		drawString(string s, float x, float y);
	int 		nCharacters;
	
	ofTTFCharacter getCharacterAsPoints(int character);

	int				refCount;
	string			key;
protected:
	vector <ofTTFCharacter> charOutlines;
	
	
	float 			lineHeight;
	charProps 		* 	cps;			// properties for each character
	GLuint			*	texNames;		// textures for each character
	int				fontSize;
	bool			bMakeContours;

	void 			drawChar(int c, float x, float y);
	
	int 			ofNextPow2(int a);
	int				border, visibleBorder;
};


#endif

