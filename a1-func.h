#include "/usr/include/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "/usr/include/python3.7/Python.h"


struct MEMORY{
    int VMEM;
    int VNMEM;
    int VTMEM;
    int FMEM;
    int vidx;
    int vnidx;
    int vtidx;
    int fidx;
};


struct v_lines{    //a struct to store information about each file/directory
    float x;
    float y;
    float z;
    int index;
};

struct vn_lines{    //a struct to store information about each file/directory
    float x;
    float y;
    float z;
    int index;
};

struct vt_lines{    //a struct to store information about each file/directory
    float x;
    float y;
    int index;
};

struct f_line{    //a struct to store information about each file/directory
    int vertices1;
    int texture1;
    int normal1;

    int vertices2;
    int texture2;
    int normal2;
  
    int vertices3;
    int texture3;
    int normal3;

    int vertices4;
    int texture4;
    int normal4;

    int index;
    
    
};


int get_v(FILE *fp, struct v_lines ** v_array, char c, char *x, char *y, char *z, int idx, int scnt, int xidx, int yidx, int zidx);
int get_vn(FILE *fp, struct vn_lines ** vn_array, char c, char *x, char *y, char *z, int idx, int scnt, int xidx, int yidx, int zidx);
int get_vt(FILE *fp, struct vt_lines ** vt_array, char c, char *x, char *y, int idx, int scnt, int xidx, int yidx);
void set_f(struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, int idx, int scnt, int slcnt, char * temp);
int get_f(FILE *fp, struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, char c, char *temp, int idx, int scnt, int slcnt, int tokidx);
struct f_line * initF(int idx);
int parseBody(FILE *fp, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, struct f_line ** f_array, struct MEMORY * mem, char c);
void setMemVar(struct MEMORY * mem);
int getObject(FILE *fp, char * object, int idx);
int getGroup(FILE *fp, char * group, int idx);
int parseHeader(FILE *fp, char * object, char * group, char c);
void freeAll(struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, struct MEMORY * mem, char * object, char * group, FILE * fp);
void collectData(struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, char * vData, char * nData, char * tData, int * fLen);
void parserInit(FILE * fp, char * vData, char * nData, char * tData, int * fLen);
void checkIntegrity(struct f_line ** f_array, int i);










