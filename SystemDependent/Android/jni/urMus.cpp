// OpenGL ES 1.0 code

#include <jni.h>
#include <android/log.h>
#define LOG_TAG "urMus"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#include <GLES/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "../../../src/httpServer.h"
#include "../../../src/urAPI.h"
#include "../../../src/MachTimer.h"
#include "../../../src/urGraphics.h"
#include "../../../src/urSTK.h"

extern "C" {
#include "lfs.h"
}

GLuint texture;
GLfloat background;
string storagePath;
urTexture *t;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern string g_fontPath;

static bool urMus_ready=false;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void init_scene(int width, int height)
{
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
	
    glDisable(GL_DITHER);
    glEnable(GL_CULL_FACE);
	
    float ratio = width / height;
    glViewport(0, 0, width, height);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
	
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	SCREEN_WIDTH=width;
	SCREEN_HEIGHT=height;
	
	LOGI("init_scene(%d,%d)\n",width,height);
}

void create_texture()
{
	string imagePath=storagePath+"/cloud_sequencer.png";
	urImage image(imagePath.c_str());
	LOGI("Loaded image size : %d by %d",image.getWidth(),image.getHeight());
//	t=new urTexture("Hello urMus!","/data/data/edu.umich.urMus/files/arial.ttf",20,320,533);
	t=new urTexture(&image);
	texture=t->getName();
	t->autoTexCoord();
	LOGI("Texture ID : %d",texture);
}

extern "C" {
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_init(JNIEnv * env, jobject obj,  jint width, jint height);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_changeBackground(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_startServer(JNIEnv * env, jobject obj, jstring path);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_setupAPI(JNIEnv *env, jobject obj);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    init_scene(width, height);
    create_texture();
}

extern void drawView();

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_step(JNIEnv * env, jobject obj)
{
	static float grey;
    grey += 0.01f;
    if (grey > 1.0f) grey = 0.0f;
    
	glClearColor(background, grey, grey, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	t->drawInRect(CGRectMake(0,0,320,533));
	drawView();
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_changeBackground(JNIEnv * env, jobject obj)
{
    background = 1.0f - background;
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_startServer(JNIEnv * env, jobject obj, jstring path)
{
	const char *pathstr=env->GetStringUTFChars(path, 0);
	chdir(pathstr);
	LOGI("Internal storage path : %s\n",pathstr);
	LOGI("Starting Server....\n");
	http_start(pathstr,pathstr);
	storagePath=pathstr;
	env->ReleaseStringUTFChars(path, pathstr);
}

extern MachTimer *mytimer;

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_setupAPI(JNIEnv *env, jobject obj)
{
	lua = lua_open();
	luaL_openlibs(lua);
	luaopen_lfs (lua); // Added external luafilesystem, runs under lua's open license
	l_setupAPI(lua);

	// path to the default font 
	g_fontPath=storagePath+"/arial.ttf";

	string urMusPath=storagePath+"/urMus.lua";
	if(luaL_dofile(lua, urMusPath.c_str())!=0)
	{
		const char* error = lua_tostring(lua, -1);
		LOGI("Lua Error : %s",error);
	}
	
	mytimer=new MachTimer();
	mytimer->start();
}




//////////////////////
/// Touch Handling ///
//////////////////////

extern "C" {
	JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_FindDoubleDragTouch(JNIEnv *env, jobject obj, jint t1, jint t2);
	JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_FindSingleDragTouch(JNIEnv *env, jobject obj, jint t);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchDownParse(JNIEnv *env, jobject obj, jint t, jint numTaps, jfloat posx, jfloat posy);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchArgInit(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchMoveUpdate(JNIEnv *env, jobject obj, jint t, jint t2, jfloat oposx, jfloat oposy, jfloat posx, jfloat posy);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchEnds(JNIEnv *env, jobject obj, jint numTaps, jfloat oposx, jfloat oposy, jfloat posx, jfloat posy);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchDoubleDragUpdate(JNIEnv *env, jobject obj, jint t, jint dragidx, jfloat pos1x, jfloat pos1y, jfloat pos2x, jfloat pos2y);
	JNIEXPORT jboolean JNICALL Java_edu_umich_urMus_urMusView_testDoubleDragStart(JNIEnv *env, jobject obj, jint t1, jint t2);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_doTouchDoubleDragStart(JNIEnv *env, jobject obj, jint t1,jint t2,jint touch1, jint touch2);
	JNIEXPORT jboolean JNICALL Java_edu_umich_urMus_urMusView_testSingleDragStart(JNIEnv *env, jobject obj, jint t);
	JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_getSingleDoubleTouchConversionID(JNIEnv *env, jobject obj, jint t);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_doTouchSingleDragStart(JNIEnv *env, jobject obj, jint t, jint touch1, jfloat pos1x, jfloat pos1y, jfloat pos2x, jfloat pos2y);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchSingleDragUpdate(JNIEnv *env, jobject obj, jint t, jint dragidx);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchScrollUpdate(JNIEnv *env, jobject obj, jint t);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchDragEnd(JNIEnv *env, jobject obj, jint t,jint touch, jfloat posx, jfloat posy);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callAllOnEnterLeaveRegions(JNIEnv *env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callAllTouchSources(JNIEnv *env, jobject obj, jdouble x, jdouble y, jint index);
	JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_getArg(JNIEnv *env, jobject obj);
	JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_getArgMoved(JNIEnv *env, jobject obj, jint i);	
}

int FindDoubleDragTouch(int t1, int t2);
int FindSingleDragTouch(int t);
void onTouchDownParse(int t, int numTaps, float posx, float posy);
void onTouchArgInit();
void onTouchMoveUpdate(int t, int t2, float oposx, float oposy, float posx, float posy);
void onTouchEnds(int numTaps, float oposx, float oposy, float posx, float posy);
void onTouchDoubleDragUpdate(int t, int dragidx, float pos1x, float pos1y, float pos2x, float pos2y);
bool testDoubleDragStart(int t1, int t2);
void doTouchDoubleDragStart(int t1,int t2,int touch1, int touch2);
bool testSingleDragStart(int t);
bool getSingleDoubleTouchConversionID(int t);
void doTouchSingleDragStart(int t, int touch1, float pos1x, float pos1y, float pos2x, float pos2y);
void onTouchSingleDragUpdate(int t, int dragidx);
void onTouchScrollUpdate(int t);
void onTouchDragEnd(int t,int touch, float posx, float posy);

extern int arg;
extern int argmoved[MAX_FINGERS];
extern float argcoordx[MAX_FINGERS];
extern float argcoordy[MAX_FINGERS];
extern float arg2coordx[MAX_FINGERS];
extern float arg2coordy[MAX_FINGERS];

JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_FindDoubleDragTouch(JNIEnv *env, jobject obj, jint t1, jint t2) {
	return FindDoubleDragTouch(t1, t2);
}

JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_FindSingleDragTouch(JNIEnv *env, jobject obj, jint t) {
	return FindSingleDragTouch(t);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchDownParse(JNIEnv *env, jobject obj, jint t, jint numTaps, jfloat posx, jfloat posy) {
	onTouchDownParse(t, numTaps, posx, posy);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchArgInit(JNIEnv *env, jobject obj) {
	onTouchArgInit();
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchMoveUpdate(JNIEnv *env, jobject obj, jint t, jint t2, jfloat oposx, jfloat oposy, jfloat posx, jfloat posy) {
	onTouchMoveUpdate(t, t2, oposx, oposy, posx, posy);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchEnds(JNIEnv *env, jobject obj, jint numTaps, jfloat oposx, jfloat oposy, jfloat posx, jfloat posy) {
	onTouchEnds(numTaps, oposx, oposy, posx, posy);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchDoubleDragUpdate(JNIEnv *env, jobject obj, jint t, jint dragidx, jfloat pos1x, jfloat pos1y, jfloat pos2x, jfloat pos2y) {
	onTouchDoubleDragUpdate(t, dragidx, pos1x, pos1y, pos2x, pos2y);
}

JNIEXPORT jboolean JNICALL Java_edu_umich_urMus_urMusView_testDoubleDragStart(JNIEnv *env, jobject obj, jint t1, jint t2) {
	return testDoubleDragStart(t1, t2);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_doTouchDoubleDragStart(JNIEnv *env, jobject obj, jint t1,jint t2,jint touch1, jint touch2) {
	doTouchDoubleDragStart(t1, t2, touch1, touch2);
}

JNIEXPORT jboolean JNICALL Java_edu_umich_urMus_urMusView_testSingleDragStart(JNIEnv *env, jobject obj, jint t) {
	return testSingleDragStart(t);
}

JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_getSingleDoubleTouchConversionID(JNIEnv *env, jobject obj, jint t) {
	return getSingleDoubleTouchConversionID(t);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_doTouchSingleDragStart(JNIEnv *env, jobject obj, jint t, jint touch1, jfloat pos1x, jfloat pos1y, jfloat pos2x, jfloat pos2y) {
	doTouchSingleDragStart(t, touch1, pos1x, pos1y, pos2x, pos2y);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchSingleDragUpdate(JNIEnv *env, jobject obj, jint t, jint dragidx) {
	onTouchSingleDragUpdate(t, dragidx);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchScrollUpdate(JNIEnv *env, jobject obj, jint t) {
	onTouchScrollUpdate(t);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_onTouchDragEnd(JNIEnv *env, jobject obj, jint t,jint touch, jfloat posx, jfloat posy) {
	onTouchDragEnd(t, touch, posx, posy);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callAllOnEnterLeaveRegions(JNIEnv *env, jobject obj) {
	callAllOnEnterLeaveRegions(arg, argcoordx, argcoordy,arg2coordx,arg2coordy);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callAllTouchSources(JNIEnv *env, jobject obj, jdouble x, jdouble y, jint index) {
	callAllTouchSources(x, y, index);	
}

JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_getArg(JNIEnv *env, jobject obj) {
	return arg;
}

JNIEXPORT jint JNICALL Java_edu_umich_urMus_urMusView_getArgMoved(JNIEnv *env, jobject obj, jint i) {
	return argmoved[i];
}

/////////////////////
/// Accelerometer ///
/////////////////////

extern "C" {
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_didAccelerate(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z);
}

bool callAllOnAccelerate(float x, float y, float z);
void callAllAccelerateSources(double tilt_x, double tilt_y, double tilt_z);

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_didAccelerate(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z) {
	callAllOnAccelerate(x, y, z);
	callAllAccelerateSources(x, y, z);
}

////////////////////////////
/// Microphone & Speaker ///
////////////////////////////

extern "C" {
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callOnMicrophone(JNIEnv *env, jobject obj, jshortArray buffer, jint length);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callOnSpeaker(JNIEnv *env, jobject obj, jshortArray buffer, jint length);
}

double urs_PullActiveDacSingleTickSinks();

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callOnMicrophone(JNIEnv *env, jobject obj, jshortArray buffer, jint length) {
	jboolean isCopy;
	jshort *pBuffer=env->GetShortArrayElements(buffer, &isCopy);
	callAllMicSources(pBuffer, length);
	for(int i=0;i<length;i++) {
		callAllMicSingleTickSources(pBuffer[i]);
	}
	
	env->ReleaseShortArrayElements(buffer,pBuffer,JNI_ABORT);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMusView_callOnSpeaker(JNIEnv *env, jobject obj, jshortArray buffer, jint length) {
	jshort *pBuffer=new jshort[length];
	for(int i=0;i<length;i++) {
		pBuffer[i]=(jshort)(32768*urs_PullActiveDacSingleTickSinks());
	}
//	LOGI("First few : %04x %04x %04x %04x",pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3]);
	env->SetShortArrayRegion(buffer,0,length,pBuffer);
	delete [] pBuffer;
}


void* LoadAudioFileData(const char *filename, UInt32 *outDataSize, UInt32*	outSampleRate)
{
	FileWvIn input;

	// Try to load the soundfile.
	
	input.openFile( storagePath+"/"+filename );
	

	double rate = 1.0;
	rate = input.getFileRate() / Stk::sampleRate();
	input.setRate( rate );
	LOGI("wave file %s rate - %f",filename,rate);
	input.ignoreSampleRateChange();

	// Find out how many channels we have.
	int channels = input.channelsOut();
	*outDataSize=input.getSize();
	*outSampleRate=input.getFileRate();
	SInt16 *data=new SInt16[input.getSize()];
	for(unsigned int i=0;i<input.getSize();i++) {
		data[i]=(int)(input.tick()*32768.0);
	}

	input.closeFile();


	return data;
}

