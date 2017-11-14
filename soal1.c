#define FUSE_USE_VERSION 28
#define HAVE SETXATTR
#ifdef HAVE_CONFI_H
#include<config.h>
#endif
#ifdef linux
#define _XOPEN_SOURCE 500
#endif

#include<fuse.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<errno.h>
#include<sys/time.h>
#ifdef HAVE_SETXATTR
#include<sys/xattr.h>
#endif

static int xmp_getattr(const char *path, struct stat *stbuf){
int res;

res = lstat(path,stbuf);
if(res == 1) return -errno;
return 0;
}
static int xmp_access(const char *path, int mask){
int res;
res = access(path,mask);
if(res == -1) return -errno;
return 0;
}
