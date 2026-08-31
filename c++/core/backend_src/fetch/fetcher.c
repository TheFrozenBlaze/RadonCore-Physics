#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>


typedef const unsigned char* (*PFNGLGETSTRINGPROC)(unsigned int);


int main() {
    void* gl = dlopen("libGL.so.1", RTLD_NOW);
    if(!gl) {
        printf("OpenGL can't compile on this hardware. Check either the opengl support on this machine or check the driver version");
        printf("Advice: if the machine uses an open source version of a closed source driver, opengl support might not be the best");
        return 1;
    }
    PFNGLGETSTRINGPROC glGetString =
    (PFNGLGETSTRINGPROC)dlsym(gl, "glGetString");
    const char* version =  (const char*)glGetString(0x1F02);
    
    int major = 0;
    int minor = 0;

    sscanf(version, "%d.%d", &major, &minor);
    uint16_t final = major * 100 + minor * 10;
    if(final < 330) {
        printf("Too old of an OpenGL version. Check drivers and make sure that your accelerator is capable of OpenGL 3.3 and above");
        return 1;
    }
    dlclose(gl);
    return 0;
};
