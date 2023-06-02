/*
 * pkgfs.c
 * Copyright (C) 2002 Florin Malita <mali@go.ro>
 *
 * This file is part of LUFS, a free userspace filesystem implementation.
 * See http://lufs.sourceforge.net/ for updates.
 *
 * LUFS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LUFS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */



/* Sample (simple) file system.
 * When mounting a lufs filesystem xxx, the lufsd daemon will try
 * to open a shared library called liblufs-xxx.so. This is what your
 * filesystem library should be called (in this case liblufs-pkgfs.so).
 *
 * Upon successful opening, it will try to resolve the following symbols:
 *   xxx_init(struct list_head*, struct dir_cache*, struct credentials*, void**),
 *   xxx_free(void*),
 *   xxx_mount(void*),
 *   xxx_umount(void),
 *   xxx_readdir(void*, char*, struct directory*),
 *   xxx_stat(void*, char*, struct lufs_fattr*),
 *   xxx_mkdir(void*, char*, int),
 *   xxx_rmdir(void*, char*),
 *   xxx_create(void*, char*, int),
 *   xxx_unlink(void*, char*),
 *   xxx_rename(void*, char*, char*),
 *   xxx_open(void*, char*, unsigned),
 *   xxx_release(void*, char*),
 *   xxx_read(void*, char*, long long, unsigned long, char*),
 *   xxx_write(void*, char*, long long, unsigned long, char*),
 *   xxx_readlink(void*, char*, char*, int),
 *   xxx_link(void*, char*, char*),
 *   xxx_symlink(void*, char*, char*),
 *   xxx_setattr(void*, char*, struct lufs_fattr*)
 *
 * Each of this (except xxx_init) receives as the first prameter a pointer
 * to a structure allocated by xxx_init to hold state information.
 * Most file system operations return 0 on success and -1 on failure. xxx_init
 * will return a pointer the allocated structure, xxx_mount will return 1 
 * on success and 0 upon failure, 
 *
 * If you wish to have an object-oriented version of the filesystem, start
 * with one of the other filesystems (such as sshfs) as your base.
 *
 * To create a new filesystem, you first need to update configure.in - 
 * model your changes to it after an already existant filesystem's
 * configuration.  Remember, you need both to add either a an AC_ARG_ENABLE
 * or a AC_DEFINE line, and add the path to your new Makefile in the AC_OUTPUT
 * statement at the bottom.  To create your directory, pick a filesystem whose
 * -configuration steps- are most similar to the one that you want to use,
 * and copy it recursively to your new directory.  Be sure to edit its
 * Makefile.in and Makefile.am, and replace all special references to the
 * old filesystem with references to your new filesystem.  Update the list
 * of source and header files as well as you change them.  Finally, above all,
 * don't forget to put the libraries that you want to link with in.  You can
 * either have the* names of the library calculated in configure and then
 * passed in a variable (as in the case of GVFS), or simply hard-code them in
 * - either way, you'll need to be modifying your lufs_<your_fs>_la_LIBADD
 * variable if you have any libraries to link with at all.
 *
 * A warning concerning libraries: Link errors very well may not show up until
 * runtime.  Be forewarned, and test thoroughly.  :)
 *
 * To upgrade your code to the latest version of LUFS (as LUFS has been in
 * fairly rapid development and you may well encounter significantly changed
 * new releases) as well as to submit new code, context diffs can be very
 * useful.  Do a 'make distclean' on your source code, and unpack an
 * unmodified, unbuilt LUFS of the same version that you have been working on
 * in another directory.  Type 'diff --new-file -r -C 3 <normal version's
 * directory> <your directory> > <patchname>'.  To patch SRBFS to handle
 * your changes, including patching a newer version, if the patch is in the
 * same directory as the freshly untarred LUFS, type 'patch -p1 < <patchname>'.
 * Accept the default answer on any questions that it asks.  You can adjust
 * the fuzz factor (how tolerant it is to changes) in the patch command
 * with the -F option.
 */


#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <utime.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <lufs/proto.h>
#include <lufs/fs.h>

#define LOCALFS 1

/* Localfs instances shared structure.
 * Just an example...
 */
struct pkgfs_global{
    unsigned count;
};

/* Localfs state info structure.
 * We don't need any state info for pkgfs, but just for kicks...
 */
struct pkgfs_context{
    struct pkgfs_global *global;
	FILE* fp;
	char last_op[32];
	char last_file[2048];
};

static void traceop(void* ctx, const char* op, const char* file)
{
	FILE* fp;
	struct pkgfs_context *c = ctx;

	if(strcmp(op, c->last_op) == 0 && strcmp(file, c->last_file)==0) {
		return;
	}
	fp = fopen("/tmp/lufs-pkgfs.lst", "a");
	if(fp) {
		fprintf(fp, "%s %s\n", op, file);
		fclose(fp);
	}
	strcpy(c->last_op, op);
	strcpy(c->last_file, file);
}

/* Initialization
 * Here we allocate a structure to hold all the file system local info 
 * (pkgfs_local). This structure will then be passed as a parameter to 
 * the other functions.
 * global_ctx holds info about another structure that can be shared between all
 * instances of the filesystem. If the pointer is NULL, then this is the
 * first instance and the structure should be allocated.
 * ! You must implement  locking/usage-count/deallocation logic when using
 *   a global context. (locking is omited here)
 * ! Most filesystems don't need a global context so you can safely ignore the
 *   global_ctx parameter.  
 */
void*
pkgfs_init(struct list_head *cfg, struct dir_cache *cache, struct credentials *cred, void **global_ctx){
    struct pkgfs_context *ctx;
    
    TRACE("initializing");

    /* Initialize global context */
    if(!(*global_ctx)){
	TRACE("initializing global context");
	if(!(*global_ctx = malloc(sizeof(struct pkgfs_global))))
	    return NULL;
	((struct pkgfs_global*)*global_ctx)->count = 0;
    }else{
	TRACE("we already have a global context");
    }

    /* Increment global context usage count */
    ((struct pkgfs_global*)*global_ctx)->count++;
    
    /* Inilialize local context */
    if(!(ctx = malloc(sizeof(struct pkgfs_context)))){
	if(!(--((struct pkgfs_global*)*global_ctx)->count)){
	    TRACE("freeing global context");
	    free(*global_ctx);
	}
	return NULL;
    }
    /* Save a reference to the global context */	
    ctx->global = *global_ctx;
	ctx->fp = NULL;
	*ctx->last_op = 0;
	*ctx->last_file = 0;

    
    return ctx;
}

/* Cleanup
 * Check the global context count and free it if necessary.
 * Deallocate memory and free all resources.
 */
void
pkgfs_free(void *c){
    struct pkgfs_context *ctx = c;
    TRACE("freeing file system data");
    
    if(!(--(ctx->global->count))){
	TRACE("freeing global context");
	free(ctx->global);
    }
    
    free(ctx);
}

/* Mount the file system.
 * Called when a mount operation is performed.
 * Initialize specific connections, login, etc.
 *
 * Notes:
 *     By default, LUFS may attempt multiple connections at once.  If your
 * filesystem doesn't support this, you need to specificy -c 1 on the
 * lufsmount command line or connections=1 in the mount options.
 *     See ftpfs for an example of how to read configuration options
 * from a configuration file if you want to, for example, be able to set
 * default values.
 */
int 	
pkgfs_mount(void *ctx){
    struct pkgfs_context *c = ctx;
    TRACE("mounting");
    return 1;
}

/* Unmount the  file system
 * Called when the file system is unmounted.
 */
void 	
pkgfs_umount(void *ctx){
    TRACE("unmounting");

}

/* Read a file/dir's attributes
 * Fill all relevant data into the fattr structure.
 * The uid/gid fields are just ownership hints hints:
 *    != 0 => we own the file
 *    == 0 => we don't own it
 * The credentials structure (if applicable and saved from _init)
 * can help determine ownership based on remote uids/gids.
 *
 * Notes:
 *     If your filesysem doesn't natively support '.' or '..',
 * don't forget to special-case them here.
 *     It is best to assume that name is a relative path, not an
 * absolute one.  Thus, you need to either be keeping track of the
 * last accessed directory in readdir, or, as this code does, changing
 * to the current directory there.
 */
int 	
pkgfs_stat(void *ctx, char *name, struct lufs_fattr *fattr)
{
#if 1
    struct stat stat;

	TRACE("pkgfs_stat %s", name);
    if(lstat(name, &stat) < 0)
	return -1;

    fattr->f_mode = stat.st_mode;
    fattr->f_nlink = stat.st_nlink;
    fattr->f_uid = (getuid() == stat.st_uid) ? 1 : 0;
    fattr->f_gid = (getgid() == stat.st_gid) ? 1 : 0;
    fattr->f_size = stat.st_size;
    fattr->f_atime = stat.st_atime;
    fattr->f_mtime = stat.st_mtime;
    fattr->f_ctime = stat.st_ctime;
#endif

    return 0;
}

/* Read a directory's content
 * For each directory entry, call 
 *    lu_cache_add2dir(struct directory *dir, char *name, char *link, struct lufs_fattr *fattr)
 * to add its information.
 * The link is optional(NULL) and applicable only if the entry is a 
 * (sym)link and we have the target info at hand.
 *
 * Notes:
 *     dir_name is an absolute path.  However, it is generally a good idea
 * to either change to that directory here or at least keep track of what
 * directory was being called on, for function calls that might not get
 * absolute paths.
 *     If your filesystem doesn't natively support '.' or '..', don't forget
 * to add them to the cache first-thing here.
 */
int 	
pkgfs_readdir(void *ctx, char *dir_name, struct directory *ddir)
{
	TRACE("pkgfs_readdir");
#if LOCALFS	
    DIR *dir;
    struct lufs_fattr fattr;
    struct dirent *dent;
    int res;

    TRACE("readdir %s", dir_name);

    if(chdir(dir_name) < 0){
		WARN("chdir %s failed: %s", dir_name, strerror(errno));
		return -1;
    }

    if(!(dir = opendir(dir_name))){
		WARN("could not open directory!");
		return -1;
    }

    while((dent = readdir(dir))){
		TRACE("adding direntry %s", dent->d_name);
	
		if((res = pkgfs_stat(ctx, dent->d_name, &fattr)) < 0){
		    WARN("could not stat file!");
		    closedir(dir);
		    return -1;
		}
	
		lu_cache_add2dir(ddir, dent->d_name, NULL, &fattr);
    }

    closedir(dir);
#endif
    return 0;
}

/* Create a directory
 */
int 	
pkgfs_mkdir(void *ctx, char *dir, int mode)
{
	TRACE("pkgfs_mkdir");
#if LOCALFS
	traceop(ctx, "mkdir", dir);
    return mkdir(dir, mode);
#else
    return 0;
#endif
}

/* Delete a directory
 */
int 	
pkgfs_rmdir(void *ctx, char *dir)
{
	TRACE("pkgfs_rmdir");
#if LOCALFS
    return rmdir(dir);
#else
    return 0;
#endif
}

/* Create a file
 */
int 	
pkgfs_create(void *ctx, char *file, int mode)
{
	TRACE("pkgfs_create");
#if LOCALFS
	traceop(ctx, "create", file);
	return mknod(file, mode, 0);
#else
	return 0;
#endif
}

/* Delete a file
 */
int 	
pkgfs_unlink(void *ctx, char *file)
{
	TRACE("pkgfs_unlink");
#if LOCALFS
    return unlink(file);
#else
	return 0;
#endif
}

/* Rename a file/dir
 */
int 	
pkgfs_rename(void *ctx, char *old_name, char *new_name)
{
	TRACE("pkgfs_rename");
#if LOCALFS
	traceop(ctx, "rename", new_name);
    return rename(old_name, new_name);
#else
	return 0;
#endif
}

/* Open a file
 *
 * Notes:
 *     By default, LUFS has no concept of file handles.  To implement file
 * handles, take a look at the atbl class in sshfs - it is easy to cut&paste
 * for use, and can be easily adapted for whatever purpose you need handles
 * for.
 *
 *     Unlike the POSIX open command which has both a "mode" variable and
 * a "flags" variable, this only has a "mode" variable.  To convert to the
 * POSIX version, ->flags=mode^O_ACCMODE and ->mode=mode&O_ACCMODE.
 */
int 	
pkgfs_open(void *ctx, char *file, unsigned mode){
    return 1;
}

int 	
pkgfs_release(void *ctx, char *file){
    return 1;
}

/* Read from a file. Changed to use the (2) routines not for efficiency,
 * but to make it work with 64-bit offsets :-(.
 */
int 	
pkgfs_read(void *ctx, char *file, long long offset, unsigned long count, char *buf)
{
#if LOCALFS
    int f, res;

    TRACE("read %s, offset %Ld, count %ld", file, offset, count);
    
    if((f = open(file, 0)) < 0){
	TRACE("open failed");
	return -1;
    }

    if(lseek(f, offset, SEEK_SET) < 0){
	close(f);
	TRACE("seek failed");
	return -1;
    }

    res = read(f, buf, count);

    TRACE("read: %d", res);

    close(f);

    return res;
#else
	return 0;
#endif
}

/* Write to a file
 */
int	
pkgfs_write(void *ctx, char *file, long long offset, unsigned long count, char *buf)
{
#if LOCALFS
    struct pkgfs_context *c = ctx;
    int fd, res;

    TRACE("write %s, offset %Ld, count %ld", file, offset, count);

    if((fd = open(file, O_RDWR)) < 0){
	TRACE("open failed");
	return -1;
    }

    if(lseek(fd, offset, SEEK_SET) < 0){
	TRACE("lseek failed");
	close(fd);
	return -1;
    }

    res = write(fd, buf, count);
	traceop(ctx, "W", file);

	close(fd);

    return res;
#else
	return 0;
#endif
}

/* Resolve a link (if the info was not added to the dir cache when 
 * reading the parent dir)
 */
int 	
pkgfs_readlink(void *ctx, char *link, char *buf, int buflen)
{
	TRACE("pkgfs_readlink");
#if LOCALFS
    return readlink(link, buf, buflen);
#else
	return 0;
#endif
}

/* Create a link
 */
int 	
pkgfs_link(void *ctx, char *target, char *lnk)
{
#if LOCALFS
	traceop(ctx, "link", lnk);
    return link(target, lnk);
#else
	return 0;
#endif
}

/* Create a symlink
 */
int 	
pkgfs_symlink(void *ctx, char *target, char *link)
{
#if LOCALFS
	TRACE("pkgfs_symlink");
	traceop(ctx, "slink", link);
    return symlink(target, link);
#else
	return 0;
#endif
}

/* Change a file/dir's attributes
 */
int 	
pkgfs_setattr(void *ctx, char *file, struct lufs_fattr *fattr)
{
#if LOCALFS
    struct stat stat;
    int res;

	TRACE("pkgfs_setattr");
    if((res = lstat(file, &stat)) < 0)
	goto out;

    if(stat.st_size > fattr->f_size){
	TRACE("truncating file to %Ld bytes", fattr->f_size);
	if((res = truncate(file, fattr->f_size)) < 0)
	    goto out;
    }

    if(stat.st_mode != fattr->f_mode){
	TRACE("set mode %o, old=%o", (unsigned)fattr->f_mode, (unsigned)stat.st_mode);
	if((res = chmod(file, fattr->f_mode)) < 0)
	    goto out;
    }

    if((stat.st_atime != (time_t)fattr->f_atime) || (stat.st_mtime != (time_t)fattr->f_mtime)){
	struct utimbuf utim = {fattr->f_atime, fattr->f_mtime};

	if((res = utime(file, &utim)) < 0)
	    goto out;
    }

  out:
    return res;    
#else
	return 0;
#endif
}

