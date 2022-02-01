#include "/usr/include/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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


int get_v(FILE *fp, struct v_lines ** v_array, char c, char *x, char *y, char *z, int idx, int scnt, int xidx, int yidx, int zidx){
    char c_last = c;
    c = getc(fp);
    
    
    if(c == '\n' || c == EOF){
        if(scnt != 2){
	    printf("Missing v values at v%d\n", idx); //if a specified data type (v, vn or vt) is missing a value, it will kill the program
	    exit(0);
        }
	v_array[idx]->x = atof(x);
	v_array[idx]->y = atof(y);
 	v_array[idx]->z = atof(z);
	v_array[idx]->index = idx;
	return -1;
    }

    else{
	if(c == ' '){
	    if(c_last != ' ' && c_last != '\n'){ //prevents dupe spaces from being counted
	        scnt++;
	    }
	}
	else if(c <= 57 || c >= 48 || c == '-'){
	    if(scnt == 0){   //unlike vn and vt this is starting at 0 because the first space in the line
		x[xidx] = c; //was used to identify this to be a vertice and not a normal or texture
	        xidx++;
	    }
	    else if(scnt == 1){  //checks spaces to determine whether x y or z is to be filled
		y[yidx] = c;
	        yidx++;
            }
	    else if(scnt == 2){
		z[zidx] = c;
	        zidx++;
	    }
		
	}
	return get_v(fp, v_array, c, x, y, z, idx, scnt, xidx, yidx, zidx);
    }
}




int get_vn(FILE *fp, struct vn_lines ** vn_array, char c, char *x, char *y, char *z, int idx, int scnt, int xidx, int yidx, int zidx){
    char c_last = c;
    c = getc(fp);
    
    if(c == '\n' || c == EOF){
	if(scnt != 3){
	    printf("Missing vn values at v%d\n", idx);
	    exit(0);
        }
	vn_array[idx]->x = atof(x);
	vn_array[idx]->y = atof(y);
 	vn_array[idx]->z = atof(z);
	vn_array[idx]->index = idx;
	return -1;
    }

    else{
	if(c == ' '){
	    if(c_last != ' ' && c_last != '\n'){ //prevents dupe spaces from being counted
	        scnt++;
	    }
	}
	else if(c <= 57 || c >= 48 || c == '-'){
	    //printf("found vn with spaces %d\n", scnt);
	    if(scnt == 1){
		x[xidx] = c;
	        xidx++;
	    }
	    else if(scnt == 2){
		y[yidx] = c;
	        yidx++;
            }
	    else if(scnt == 3){
		z[zidx] = c;
	        zidx++;
	    }
		
	}
	return get_vn(fp, vn_array, c, x, y, z, idx, scnt, xidx, yidx, zidx);
    }
}



int get_vt(FILE *fp, struct vt_lines ** vt_array, char c, char *x, char *y, int idx, int scnt, int xidx, int yidx){
    char c_last = c;
    c = getc(fp);
    
    
    if(c == '\n' || c == EOF){
	if(scnt != 2){
	    printf("Missing vt values at vt%d\n", idx);
	    exit(0);
        }
	vt_array[idx]->x = atof(x);
	vt_array[idx]->y = atof(y);
        if(vt_array[idx]->x > 1 || vt_array[idx]->x < 0 || vt_array[idx]->y > 1 || vt_array[idx]->y < 0){
	    printf("Error at vt%d (%f %f), vt values must be >=0 or <= 1 as per assignment description\n", idx, vt_array[idx]->x, vt_array[idx]->y);
	    exit(0);
	}
	vt_array[idx]->index = idx;
	return -1;
    }

    else{
	if(c == ' '){
	    if(c_last != ' ' && c_last != '\n'){ //prevents dupe spaces from being counted
	        scnt++;
	    }
	}
	else if(c <= 57 || c >= 48 || c == '-'){
	    //printf("found vt with spaces %d\n", scnt);
	    if(scnt == 1){
		x[xidx] = c;
	        xidx++;
	    }
	    else if(scnt == 2){
		y[yidx] = c;
	        yidx++;
            }
		
	}
	return get_vt(fp, vt_array, c, x, y, idx, scnt, xidx, yidx);
    }
}


void set_f(struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, int idx, int scnt, int slcnt, char * temp){

    //printf("temp is %s\n", temp);  determines which index to place the value by number of spaces and slashes
    int i = atoi(temp);
    if(slcnt == 0){
	if(scnt == 0){ //would be at index 1 2 of F
     	    f_array[idx]->vertices1 = i; 
	}

        else if(scnt == 1){ //would be at index 2 2 of F
	f_array[idx]->vertices2 = i; 
        }

	else if(scnt == 2){ //would be at index 3 2 of F
	    f_array[idx]->vertices3 = i; 
	}
	else if(scnt == 3){ //would be at index 4 2 of F
	    f_array[idx]->vertices4 = i; 
	}
    }


    else if(slcnt == 1){
        if(scnt == 0){ //would be at index 1 2 of F
            f_array[idx]->texture1 = i; 
        }

        else if(scnt == 1){ //would be at index 2 2 of F
            f_array[idx]->texture2 = i; 
        }

        else if(scnt == 2){ //would be at index 3 2 of F
	    f_array[idx]->texture3 = i; 
        }
        else if(scnt == 3){ //would be at index 4 2 of F
            f_array[idx]->texture4 = i; 
        }
    }



    else if(slcnt == 2){
        if(scnt == 0){ //would be at index 1 2 of F
            f_array[idx]->normal1 = i; 
        }

        else if(scnt == 1){ //would be at index 2 2 of F
            f_array[idx]->normal2 = i; 
        }

        else if(scnt == 2){ //would be at index 3 2 of F
	    f_array[idx]->normal3 = i; 
        }
        else if(scnt == 3){ //would be at index 4 2 of F
            f_array[idx]->normal4 = i; 
        } 
    }
}

int get_f(FILE *fp, struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, char c, char *temp, int idx, int scnt, int slcnt, int tokidx){
    char c_last = c;
    c = getc(fp);  
    
    if(c == '\n' || c == EOF){
        set_f(f_array, v_array, vn_array, vt_array, idx, scnt, slcnt, temp);
	return -1;
    }

    else{
	if(c == ' '){
	    if(c_last != ' ' && c_last != '\n'){ //prevents dupe spaces from being counted
		set_f(f_array, v_array, vn_array, vt_array, idx, scnt, slcnt, temp);
	        scnt++;
	    }
	    slcnt = 0;
	    tokidx = 0;
	    memset(temp, 0, strlen(temp));

	}
	else if(c <= 57 && c >= 48){
	    temp[tokidx] = c;
	    tokidx++;
	}
        else if(c == '/'){
	    if(c_last != '/'){
                set_f(f_array, v_array, vn_array, vt_array, idx, scnt, slcnt, temp);
	    }
	    tokidx = 0;
	    memset(temp, 0, strlen(temp));
	    slcnt++;
	    if(slcnt > 2){
		printf("Invalid line: check f%d\n", idx);
		exit(0);
	    }
	}
	return get_f(fp, f_array, v_array, vn_array, vt_array, c, temp, idx, scnt, slcnt, tokidx);
    }
}

struct f_line * initF(int idx){
    struct f_line * F1;
    F1 = malloc(sizeof(F1) * 25);
    F1->index = idx;
    F1->vertices1 = 0;
    F1->texture1 = 0;
    F1->normal1 = 0;

    F1->vertices2 = 0;
    F1->texture2 = 0;
    F1->normal2 = 0;
   
    F1->vertices3 = 0;
    F1->texture3 = 0;
    F1->normal3 = 0;
 
    F1->vertices4 = 0;
    F1->texture4 = 0;
    F1->normal4 = 0;

    return F1;
    
}



int parseBody(FILE *fp, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, struct f_line ** f_array, struct MEMORY * mem, char c){
    char c_last = c;
    char x[10] = "";
    char y[10] = "";
    char z[10] = ""; //this parser is only responsible for getting v, vt, vn or f values
    c = getc(fp);
    if(c == EOF){    //parser gets comparisons and then uses required function.
	return 0;
    }
    else{
        if(c_last == 'v' && c == ' '){
	    v_array[mem->vidx] = malloc(sizeof(v_array) * 25);
	    get_v(fp, v_array, c, x, y, z, mem->vidx, 0, 0, 0, 0);
	    //printf("V at index  %d with x = %f    y = %f     z = %f\n", v_array[mem->vidx]->index, v_array[mem->vidx]->x, v_array[mem->vidx]->y, v_array[mem->vidx]->z);
     	    mem->vidx++;
	    //if(!checkMem(mem, 1));
	    if (mem->vidx == mem->VMEM){
 	        mem->VMEM = mem->VMEM * 2;
		v_array = realloc(v_array, sizeof(struct v_lines*) * mem->VMEM);
	    }
	    
	    
	    
	}
        else if(c_last == 'v' && c == 'n'){
	    vn_array[mem->vnidx] = malloc(sizeof(vn_array) * 25);
	    get_vn(fp, vn_array, c, x, y, z, mem->vnidx, 0, 0, 0, 0);
     	    mem->vnidx++;
	    //if(!checkMem(mem, 2));
	    if (mem->vnidx == mem->VNMEM){
 	        mem->VNMEM = mem->VNMEM * 2;
		vn_array = realloc(vn_array, sizeof(struct vn_lines*) * mem->VNMEM);
	    }

	}
        else if(c_last == 'v' && c == 't'){
	    vt_array[mem->vtidx] = malloc(sizeof(vt_array) * 25);
	    get_vt(fp, vt_array, c, x, y, mem->vtidx, 0, 0, 0);
     	    mem->vtidx++;
	    //if(!checkMem(mem, 3));
	    if (mem->vtidx == mem->VTMEM){
 	        mem->VTMEM = mem->VTMEM * 2;
		 vt_array = realloc(vt_array, sizeof(struct vt_lines*) * mem->VTMEM);
	    }
	    
	}
        else if(c_last == 'f' && c == ' '){
	    //printf("%d\n", mem->fidx);
            f_array[mem->fidx] = initF(mem->fidx);
	    get_f(fp, f_array, v_array, vn_array, vt_array, c, x, mem->fidx, 0, 0, 0);
     	    mem->fidx++;
	    //if(!checkMem(mem, 4));
	    if (mem->fidx == mem->FMEM){
 	        mem->FMEM = mem->FMEM * 2;
		f_array = realloc(f_array, sizeof(struct f_line*) * mem->FMEM);
	    }

	    
	}
            
        return parseBody(fp, v_array, vn_array, vt_array, f_array, mem, c);
    }
    	printf("\nexitting\n");
	
}

void setMemVar(struct MEMORY * mem){
    mem->VMEM = 30000;
    mem->VNMEM = 30000;
    mem->VTMEM = 30000;
    mem->FMEM = 30000;
    mem->vidx = 1;
    mem->vnidx = 1;
    mem->vtidx = 1;
    mem->fidx = 1;
}


int getObject(FILE *fp, char * object, int idx){
    char c;
    c = getc(fp);
    if (c == '\n' || c == EOF){
	return 0;
    }
    else{
	object[idx] = c;
	idx++;
	return getObject(fp, object, idx);
    }
}

int getGroup(FILE *fp, char * group, int idx){
    char c;
    c = getc(fp);
    if (c == '\n' || c == EOF){
	return 0;
    }
    else{
	group[idx] = c;
	idx++;
	return getGroup(fp, group, idx);
    }
}

int parseHeader(FILE *fp, char * object, char * group, char c){
    char c_last = c;
    c = getc(fp);      //this parser is only responble the header (getting comments and object/group names
    if(c == ' ' && c_last == 'v'){
	ungetc(c, fp);
	ungetc(c_last, fp); //brings back the first v input for the other function to start reading from (will ignore the first v if not done)
	return 0;
    }else if(c == ' ' && (c_last == 'o' || c_last == ' ') ){
        getObject(fp, object, 0);
	return parseHeader(fp, object, group, c);
    }else if(c == ' ' && (c_last == 'g' || c_last == ' ') ){
	getGroup(fp, group, 0);
        return parseHeader(fp, object, group, c);
    }
    else{
	return parseHeader(fp, object, group, c);
    }
}

void freeAll(struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, struct MEMORY * mem, char * object, char * group, FILE * fp){
    int i = 0;
    while(f_array[i] != NULL){
	free(f_array[i]);
        i++;
    }

    i = 1;
    while(v_array[i] != NULL){
	free(v_array[i]);
        i++;
    }
   
    i = 1;
    while(vn_array[i] != NULL){
	free(vn_array[i]);
        i++;
    }

    i = 1;
    while(vt_array[i] != NULL){
	free(vt_array[i]);
        i++;
    }
   
    free(mem);
    free(v_array);
    free(vn_array);
    free(vt_array);
    free(f_array);
    free(object);
    free(group);
    fclose(fp);
}

void checkIntegrity(struct f_line ** f_array, int i){
    if(f_array[i]->vertices1 == 0 && (f_array[i]->texture1 != 0 || f_array[i]->normal1 != 0) ){  //if in the F data type the v is missing, it will kill the program
	printf("missing v in f in the first set%d\n", i);
	exit(0);
    }

    if(f_array[i]->vertices2 == 0 && (f_array[i]->texture2 != 0 || f_array[i]->normal2 != 0) ){
	printf("missing v in f in the second set%d\n", i);
	exit(0);
    }

    if(f_array[i]->vertices3 == 0 && (f_array[i]->texture3 != 0 || f_array[i]->normal3 != 0) ){
	printf("missing v in f in the third set%d\n", i);
	exit(0);
    }

    if(f_array[i]->vertices4 == 0 && (f_array[i]->texture4 != 0 || f_array[i]->normal4 != 0) ){
	printf("missing v in f in the fourth set%d\n", i);
	exit(0);
    }
    

}

    

void collectData(struct f_line ** f_array, struct v_lines ** v_array, struct vn_lines ** vn_array, struct vt_lines ** vt_array, char * vData, char * nData, char * tData, int * fLen){
    int i = 1;
    char buf[50];
    strcat(vData, "    return [\n");
    strcat(nData, "    return [\n");
    strcat(tData, "    return [\n");
    while(f_array[i] != NULL){
	checkIntegrity(f_array, i);
	
        if(f_array[i]->vertices1 != 0 || f_array[i]->vertices2 != 0 || f_array[i]->vertices3 != 0 || f_array[i]->vertices4 != 0){ //check for integrity of V and print accordingly 
	    if(f_array[i]->vertices1 != 0){
		sprintf(buf, "        %f, %f, %f,\n", v_array[f_array[i]->vertices1]->x, v_array[f_array[i]->vertices1]->y, v_array[f_array[i]->vertices1]->z);
                strcat(vData, buf);
	    }

	    if(f_array[i]->vertices2 != 0){
		sprintf(buf, "        %f, %f, %f,\n", v_array[f_array[i]->vertices2]->x, v_array[f_array[i]->vertices2]->y, v_array[f_array[i]->vertices2]->z);
                strcat(vData, buf);
            }
 
	    if(f_array[i]->vertices3 != 0){
		sprintf(buf, "        %f, %f, %f,\n", v_array[f_array[i]->vertices3]->x, v_array[f_array[i]->vertices3]->y, v_array[f_array[i]->vertices3]->z);
                strcat(vData, buf);
	    }

	    if(f_array[i]->vertices4 != 0){
		sprintf(buf, "        %f, %f, %f,\n", v_array[f_array[i]->vertices4]->x, v_array[f_array[i]->vertices4]->y, v_array[f_array[i]->vertices4]->z);
                strcat(vData, buf);
	    }
        }



        if(f_array[i]->texture1 != 0 || f_array[i]->texture2 != 0 || f_array[i]->texture3 != 0 || f_array[i]->texture4 != 0){ //check for integrity of VT and print accordingly 
	    if(f_array[i]->texture1 != 0){
		sprintf(buf, "        %f, %f,\n", vt_array[f_array[i]->texture1]->x, vt_array[f_array[i]->texture1]->y);
                strcat(tData, buf);
	    }

	    if(f_array[i]->texture2!= 0){
		sprintf(buf, "        %f, %f,\n", vt_array[f_array[i]->texture2]->x, vt_array[f_array[i]->texture2]->y);
                strcat(tData, buf);
	    }

	    if(f_array[i]->texture3 != 0){
		sprintf(buf, "        %f, %f,\n", vt_array[f_array[i]->texture3]->x, vt_array[f_array[i]->texture3]->y);
                strcat(tData, buf);
	    }

	    if(f_array[i]->texture4 != 0){
		sprintf(buf, "        %f, %f,\n", vt_array[f_array[i]->texture4]->x, vt_array[f_array[i]->texture4]->y);
                strcat(tData, buf);
	    }

        }



        if(f_array[i]->normal1 != 0 || f_array[i]->normal2 != 0 || f_array[i]->normal3 != 0 || f_array[i]->normal4 != 0){ //check for integrity of VN and print accordingly 
	    if(f_array[i]->normal1 != 0){
		sprintf(buf, "        %f, %f, %f,\n", vn_array[f_array[i]->normal1]->x, vn_array[f_array[i]->normal1]->y, vn_array[f_array[i]->normal1]->z);
                strcat(nData, buf);
	    }

	    if(f_array[i]->normal2!= 0){
		sprintf(buf, "        %f, %f, %f,\n", vn_array[f_array[i]->normal2]->x, vn_array[f_array[i]->normal2]->y, vn_array[f_array[i]->normal2]->z);
                strcat(nData, buf);
	    }

	    if(f_array[i]->normal3 != 0){
		sprintf(buf, "        %f, %f, %f,\n", vn_array[f_array[i]->normal3]->x, vn_array[f_array[i]->normal3]->y, vn_array[f_array[i]->normal3]->z);
                strcat(nData, buf);
	    }

	    if(f_array[i]->normal4 != 0){
		sprintf(buf, "        %f, %f, %f,\n", vn_array[f_array[i]->normal4]->x, vn_array[f_array[i]->normal4]->y, vn_array[f_array[i]->normal4]->z);
                strcat(nData, buf);

	    }

        }

        i++;
    }
    *fLen = i-1;
}
       

void parserInit(FILE * fp, char * vData, char * nData, char * tData, int * fLen){  
    char * object = malloc(sizeof(char) * 50);
    char * group = malloc(sizeof(char) * 50);   //this is my "parser" all in one.

    struct MEMORY * mem;
    mem = malloc(sizeof(struct MEMORY));
    setMemVar(mem);
    struct v_lines ** v_array;
    v_array = malloc(sizeof(struct v_lines*) * mem->VMEM);
    struct vn_lines ** vn_array;
    vn_array = malloc(sizeof(struct vn_lines*) * mem->VNMEM);
    struct vt_lines ** vt_array;
    vt_array = malloc(sizeof(struct vt_lines*) * mem->VTMEM);
    struct f_line ** f_array;
    f_array = malloc(sizeof(struct f_line*) * mem->FMEM);

    char c = getc(fp);
    parseHeader(fp, object, group, c);
    parseBody(fp, v_array, vn_array, vt_array, f_array, mem, c);
    float ** retArr = malloc(sizeof(float*) * (mem->VMEM + mem->VNMEM + mem->VTMEM + mem->FMEM) );
    int i;
    for(i = 0; i < (mem->VMEM + mem->VNMEM + mem->VTMEM + mem->FMEM); i++){
        retArr[i] = malloc(sizeof(float*) * 20);
    }
    collectData(f_array, v_array, vn_array, vt_array, vData, nData, tData, fLen);
    //print_f(F, V, VN, VT);
    //printf("Group: %s\nObject:%s\n", object, group);
    freeAll(f_array, v_array, vn_array, vt_array, mem, object, group, fp);
}



