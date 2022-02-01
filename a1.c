#include "/usr/include/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "a1-func.h"
#define PY_SSIZE_T_CLEAN
#include "/usr/include/python3.7/Python.h"
#define memsize 9000000


int main(int argc, char **argv){
    char cwd[200];
    setenv("PYTHONPATH", getcwd(cwd,sizeof(cwd)), 1 );

    FILE *fp;
    fp = fopen("preloaddata.js", "w");
    if(!fp){
        printf("error opening preloaddata.js to clear\n");
        exit(0);
    }
    fclose(fp);


/////////////////A1 code stuff///////////////////////

    char fileName[50];
    if(argv[1] == NULL){
	printf("Must select a .obj file prior to viewing\n");
	exit(0);
    }
    strcpy(fileName, argv[1]);
    strcat(fileName, ".obj");
    printf("%s\n", argv[1]);
    fp = fopen(fileName, "r");
    if(fp == NULL){
	    printf("Invalid .obj File. Please check to make sure the correct file was used.\n");
	exit(0);
    }
    char * vData = malloc(sizeof(char) * memsize);
    char * nData = malloc(sizeof(char) * memsize);
    char * tData = malloc(sizeof(char) * memsize);
    int fLen = 0;
    parserInit(fp, vData, nData, tData, &fLen);

/////////////////////////////////////////////////////

    int width = 0;
    int rows = 0;
    int depth = 0;
    int i;
    int j;
    int * PPMdata;
    int PPMlen = 0;
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pRet, *retVal;   //initialization of python variables
    Py_Initialize();  //initialize python
    pName = PyUnicode_DecodeFSDefault("readppm");
    if(pName == NULL){
	perror("Python program not found");
 	exit(1);
    }
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (pModule != NULL){
	pFunc = PyObject_GetAttrString(pModule, "main");
        if(pFunc && PyCallable_Check(pFunc)){

	    pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(argv[1]));
	    pRet = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if(pRet == NULL){
                perror("Must select a .PPM file prior to viewing\n");
                exit(0);
            }
            if(pRet != NULL){
                retVal = PyList_GetItem(pRet, 0);
                width = PyLong_AsLong(retVal);

                retVal = PyList_GetItem(pRet, 1);
                rows = PyLong_AsLong(retVal);

                retVal = PyList_GetItem(pRet, 2);
                depth = PyLong_AsLong(retVal);
                //printf("%d %d %d\n", width, rows, depth);
                //r = malloc(width * rows * sizeof(int));
                //g = malloc(width * rows * sizeof(int));
                //b = malloc(width * rows * sizeof(int));
                //int count = 0;
                j = 0;
                PPMdata = malloc(width * rows * 4 * sizeof(int));
                for(i = 0; i < width * rows * 3; i++){
                    retVal = PyList_GetItem(pRet, i + 3);
                    PPMdata[j] = PyLong_AsLong(retVal);
                    j++;
                    if(i % 3 == 2){
                        PPMdata[j] = depth;
                        j++;
                    }
                    /*if(i % 3 == 0){
                        r[count] = PyLong_AsLong(retVal);
                    }else if(i % 3 == 1){
                        g[count] = PyLong_AsLong(retVal);
                    }else if(i % 3 == 2){
                        b[count] = PyLong_AsLong(retVal);
                        count++;
                    }*/
                }
                PPMlen = j;
                //int wCnt = 0;
                /*for(i = 0; i < rows * width; i++){       
                    printf("%d %d %d  ", r[i], g[i], b[i]);
                    wCnt++;
                    if(wCnt == width){
                        wCnt = 0;
                        printf("\n");
                    }
           
                }*/
                //printf("\n\n%d %d %d\n", width, rows, depth);

                
            }
            
        }
        else{
            perror("Function is not callable\n");
            exit(0);
        }
	
    }
    else{
        perror("Module not found - make sure to check location of PYTHONPATH\n");
        exit(0);
    }
    if (PyErr_Occurred()){
        PyErr_PrintEx(0);
        PyErr_Clear(); 
    }
    Py_FinalizeEx();
    
    
    fp = fopen("preloaddata.js", "a");
    if(!fp){
        printf("error opening preloaddata.js to append\n");
        exit(0);
    }
    char * jsFile = "// original code taken from MDN Web Docs\nfunction getdistance() {\n   return -4.0;\n}\n\n	// position array\n	function loadvertexcount() {\n        }\n// vertex positions\n	// v data\nfunction loadvertices() {\n        ];\n\n}\n\n\nfunction loadnormals() {\n        ];\n}\n\n\nfunction loadtextcoords() {\n        ];\n}\n\n\nfunction loadvertexindices() {\n         ];\n\n}\n\n\n	// texture array size and data\n	// width and height of the ppm file\n\nfunction loadwidth() {\n}\n\nfunction loadheight() {\n}\n\nfunction loadtexture() {\n}\n";

    char * indices = malloc(sizeof(char) * 500000);
    char lineData[256];
    j = 0;
    int k;
    int len = strlen(jsFile);
    char buf[100];

    for(i = 0; i < len; i++){
        lineData[j] = jsFile[i];
        j++;
        if(jsFile[i] == '\n'){
            fputs(lineData, fp);

            if( strcmp(lineData, "	function loadvertexcount() {\n") == 0){
                sprintf(buf, "           return %d;\n", fLen * 3);
                fputs(buf, fp);
            }

            else if (strcmp(lineData, "function loadvertices() {\n") == 0 ) {
                fputs(vData, fp);
                //strcat(fileData, vData);
                free(vData);
            }

            else if (strcmp(lineData, "function loadnormals() {\n") == 0 ) {
                fputs(nData, fp);
                //strcat(fileData, nData);
                free(nData);
            }

            else if (strcmp(lineData, "function loadtextcoords() {\n") == 0 ) {
                fputs(tData, fp);
                //strcat(fileData, tData);
                free(tData);
            }



            else if (strcmp(lineData, "function loadwidth() {\n") == 0 ) {
                sprintf(buf, "    return %d;\n", width);
                fputs(buf, fp);
                //strcat(fileData, buf);
            }

            else if (strcmp(lineData, "function loadheight() {\n") == 0 ) {
                sprintf(buf, "    return %d;\n", rows);
                fputs(buf, fp);
                //strcat(fileData, buf);
            }

            else if(strcmp(lineData, "function loadvertexindices() {\n") == 0 ){
                strcat(indices, "    return [\n");
                printf("fLen = %d\n", fLen);
                for(k = 0; k < fLen * 3; k = k + 3){
                    sprintf(buf, "        %d, %d, %d,\n", k, k + 1, k + 2);
                    strcat(indices, buf);
                }
                //indices[strlen(indices) - 2] = ' ';
                fputs(indices, fp);
                free(indices);
            }


            else if(strcmp(lineData, "function loadtexture() {\n") == 0 ){
                sprintf(buf, "    return( new Uint8Array([\n        ");
                fputs(buf, fp);
                for(k = 0; k < PPMlen; k++){
                    if(k%4 == 0 && k != 0){
                        sprintf(buf, "\n        ");
                        fputs(buf, fp);
                    }
                    if(k != PPMlen -1){
                        sprintf(buf, "%d, ", PPMdata[k]);
                        fputs(buf, fp);
                    }else if(k == PPMlen - 1){
                        sprintf(buf, "%d\n                          ]) );\n", PPMdata[k]);
                        fputs(buf, fp);
                    }
                }
            }







             memset(lineData, 0, strlen(lineData));
             j = 0;
        }
        
    }
    fclose(fp);
    return -1;
    
}

	