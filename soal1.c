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

//akses file
static int xmp_access(const char *path, int mask){
int res;
res = access(path,mask);
if(res == -1) return -errno;
return 0;
}

//bikin direktori
static int xmp_mkdir(const char *path, mode_t mode){
int res;
res = mkdir(path,mode);
if(res == -1) return -errno;
return 0;
}

//rename file
static int xmp_rename(const char *from, const char *to){
int res;
res = rename(from,to);
if(res == -1) return -errno;
return 0;
}


//open file
static int xmp_open(const char *path, struct fuse_file_info *fi){
int res;
res = open(path, fi->flags);
if(res == -1) return -errno;
close(res);
return 0;
}

//read file(read doang)
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
int fd, res;
(void) fi;
fd = open(path, O_RDONLY);
if(fd == -1) return -errno;
res = pread(fd, buf, size, offset);
if(res == -1) res = -errno;

close(fd);
return res;
}

//write file
static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
int fd, res;

(void) fi;
fd = open(path, O_WRONLY);
if(fd == -1) return -errno;
res = pwrite(fd, buf, size, offset);
if(res == -1) res = -errno;

close(fd);
return res;
}
