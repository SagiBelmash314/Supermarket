#ifndef MYMACROS_H
#define MYMACROS_H

#define CHECK_RETURN_0(pointer) if(!(pointer)) {return 0;}
#define CHECK_MSG_RETURN_0(pointer,msg) if(!(pointer)) {printf(msg); return 0;}
#define FREE_CLOSE_FILE_RETURN_0(pointer, filePointer) fclose(filePointer); free(filePointer); return 0;
#define CLOSE_RETURN_0(filePointer) fclose(filePointer); return 0;

#endif //MYMACROS_H
