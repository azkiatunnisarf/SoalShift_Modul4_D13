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
#include<sys/wait.h>
#ifdef HAVE_SETXATTR
#include<sys/xattr.h>
#endif

static const char *dirnyaa = "/home/azkianisa/Downloads";

//get atribut file
static int xmp_getattr(const char *path, struct stat *stbuf){
int res;
char dpath[200];

sprintf(dpath,"%s%s",dirnyaa,path);
res = lstat(dpath,stbuf);
if(res == 1) return -errno;
return 0;
}

//read direktori downlads
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
DIR *dp;
struct dirent *de;
char dpath[200];
sprintf(dpath,"%s%s",dirnyaa,path);

(void) offset;
(void) fi;

dp = opendir(dpath);
if(dp == NULL) return -errno;

	while((de = readdir(dp))!=NULL){
	struct stat st;
	memset(&st,0,sizeof(st));
	st.st_ino = de->d_ino;
	st.st_mode = de->d_type << 12;
	if(filler(buf, de->d_name, &st, 0)) break;
	}
closedir(dp);
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

res = mkdir(dpath,mode);
if(res == -1) return -errno;
return 0;
}

//open file, mkdir simpanan pas ngebuka
static int xmp_open(const char *path, struct fuse_file_info *fi){
int res;
char dpath[200];
sprintf(dpath,"%s%s",dirnyaa,path);
time_t old;
time_t new;
stat("/home/azkianisa/Downloads/*.txt",&check);
old=check.st_mtime;
int status;
while(1){
	stat("/home/azkianisa/Downlads/*.txt",&check);
	new=check.st_mtime;
	if(old!=new) xmp_mkdir(); //mkdir simpanan
	else{while(wait(&status)>0);}
	}
res = open(dpath, fi->flags);
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

//main program
int main(int argc, char *argv[]){
umask(0);
return fuse_main(argc, argv, &xmp_oper, NULL);
}
