// OpenGL ES 1.0 code

#include <jni.h>
#include <android/log.h>
#define LOG_TAG "urMus"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#include <GLES/gl.h>

#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include "../../../src/httpServer.h"
#include "../../../src/urAPI.h"
extern "C" {
#include "lfs.h"
}

GLuint texture;
GLfloat background;
string storagePath;
urTexture *t;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

static bool urMus_ready=false;

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void gluLookAt(float eyeX, float eyeY, float eyeZ,
					  float centerX, float centerY, float centerZ, float upX, float upY,
					  float upZ)
{
    // See the OpenGL GLUT documentation for gluLookAt for a description
    // of the algorithm. We implement it in a straightforward way:
	
    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;
	
    // Normalize f
    float rlf = 1.0f / sqrtf(fx*fx + fy*fy + fz*fz);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;
	
    // Normalize up
    float rlup = 1.0f / sqrtf(upX*upX + upY*upY + upZ*upZ);
    upX *= rlup;
    upY *= rlup;
    upZ *= rlup;
	
    // compute s = f x up (x means "cross product")
	
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;
	
    // compute u = s x f
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;
	
    float m[16] ;
    m[0] = sx;
    m[1] = ux;
    m[2] = -fx;
    m[3] = 0.0f;
	
    m[4] = sy;
    m[5] = uy;
    m[6] = -fy;
    m[7] = 0.0f;
	
    m[8] = sz;
    m[9] = uz;
    m[10] = -fz;
    m[11] = 0.0f;
	
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
	
    glMultMatrixf(m);
    glTranslatef(-eyeX, -eyeY, -eyeZ);
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
	t=new urTexture(&image);
	texture=t->getName();
	t->autoTexCoord();
	LOGI("Texture ID : %d",texture);
}

extern "C" {
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_init(JNIEnv * env, jobject obj,  jint width, jint height);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_changeBackground(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_startServer(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_setupAPI(JNIEnv *env, jobject obj);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    init_scene(width, height);
    create_texture();
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_step(JNIEnv * env, jobject obj)
{
	static float grey;
    grey += 0.01f;
    if (grey > 1.0f) grey = 0.0f;
    
	glClearColor(background, grey, grey, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	t->drawInRect(CGRectMake(0,0,SCREEN_WIDTH,SCREEN_HEIGHT));
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_changeBackground(JNIEnv * env, jobject obj)
{
    background = 1.0f - background;
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_startServer(JNIEnv * env, jobject obj)
{
	jclass cls = env->GetObjectClass(obj);
	jmethodID getFilesDir = env->GetMethodID(cls, "getFilesDir", "()Ljava/io/File;");
	jobject dirobj = env->CallObjectMethod(obj,getFilesDir);
	jclass dir = env->GetObjectClass(dirobj);
	jmethodID getStoragePath = env->GetMethodID(dir, "getAbsolutePath", "()Ljava/lang/String;");
	jstring path=(jstring)env->CallObjectMethod(dirobj, getStoragePath);
	const char *pathstr=env->GetStringUTFChars(path, 0);
	LOGI("Internal storage path : %s\n",pathstr);
	LOGI("Starting Server....\n");
	http_start(pathstr,pathstr);
	storagePath=pathstr;
	env->ReleaseStringUTFChars(path, pathstr);
}

JNIEXPORT void JNICALL Java_edu_umich_urMus_urMus_setupAPI(JNIEnv *env, jobject obj)
{
	lua = lua_open();
	luaL_openlibs(lua);
	luaopen_lfs (lua); // Added external luafilesystem, runs under lua's open license
	l_setupAPI(lua);
}

