#define wnioC
/**********************************************************************


wn_ioerrfpush(pfunc)     (*pfunc)(operation,file,failure)
wn_ioerrfpop()


wn_search_path_for_file(&result,path,filename)


wn_file wn_fopen(file_name,mode)
wn_fclose(stream)
int wn_fgets(s,n,stream)
wn_fputs(s,stream)
bool wn_feof(f)
int wn_getchar()
int wn_getc(stream)
wn_ungetc(c,stream)
int wn_putc(c,stream)
wn_fflush(stream)


wn_file wn_popen(command,mode)
wn_pclose(stream)


int wn_open(path,flags,mode)
wn_close(d)
wn_write(d,buf,nbytes)
wn_read(d,buf,nbytes)


wn_mv(from,to)
wn_rm(path)
wn_rmf(path)
wn_mkdir(dir_name,mode)
wn_rmdir(dir_name)
wn_cd(dir_name)
wn_pushd(dir_name)
wn_popd()
wn_pwd(dir_name)


bool wn_file_exists(path)
bool wn_is_directory(path)


**********************************************************************/
/****************************************************************************


COPYRIGHT NOTICE:


  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.


AUTHOR:


  Will Naylor, Bill Chapman


****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "wnlib.h"

#ifdef WN_WINDOWS
# include <io.h>
# include <direct.h>
#else
# include <unistd.h>
#endif

#ifdef WN_VMS
# include <stat.h>
#else
# include <sys/types.h>
# include <sys/stat.h>
#endif

#include "wnasrt.h"
#include "wnmem.h"
#include "wnsll.h"
#include "wneq.h"
#include "wnstr.h"

#define WN_DONT_GUARD_WNIO
#include "wnio.h"

#define DEFAULT_OPEN_MODE    00644
#define DEFAULT_MKDIR_MODE   00755


local wn_sll io_stack = NULL,
             directory_stack = NULL;

local void lo_check_buffer_overflow(char *buf, char *routine_name) {
  int len = strlen(buf);

  if (len >= WN_MAX_FILE_NAME_PATH_LEN) {
    fprintf(stderr,
    /**/  "File name buffer overflow in <%s>.\n"
    /**/  "  File name \"%s\" is > %d bytes long\n",
    /**/  routine_name, buf, len);
    wn_abort();
  }
}

local void default_error_function(char operation[],char file[],char failure[])
{
  (void)fprintf(wn_stderr,"io failure: op=<%s>,file=<%s>,failure=<%s>\n",
                operation,file,failure);
  exit(-1);
}



local error_func_type pcurrent_error_func = (default_error_function);



void wn_ioerrfpush(error_func_type pfunc)
{
  wn_gppush(wn_defaultgp());


  wn_sllins(&io_stack,(ptr)pcurrent_error_func);
  pcurrent_error_func = pfunc;


  wn_gppop();
}



void wn_ioerrfpop(void)
{
  wn_gppush(wn_defaultgp());


  pcurrent_error_func = (error_func_type)(io_stack->contents);
  wn_assert(io_stack != NULL);
  wn_slledel(&io_stack);


  wn_gppop();
}



local void translate_errno(char *pfailure[])
{
# if 1
    *pfailure = strerror(errno);
# else
    extern char *sys_errlist[];

    *pfailure = sys_errlist[errno];
# endif
}



void wn_search_path_for_file(char **presult,char *path,char *filename)
{
  int i;
  static char filepath[WN_MAX_FILE_NAME_PATH_LEN];


  *presult = NULL;


  while(*path != '\0')
  {
    while((*path == ' ')||(*path == '\t'))  /* skip blanks */
    {
      ++path;
    }


    if(*path == '\0')
    {
      break;
    }


    i = 0;
    while(!((*path == ' ')||(*path == '\t')||(*path == '\0')))
    {
      filepath[i] = *path;
      ++path; ++i;
    }
    filepath[i] = '\0';
    strcat(filepath,WN_OSSLASH);
    strcat(filepath,filename);
    lo_check_buffer_overflow(filepath, "wn_search_path_for_file");


    if(wn_file_exists(filepath))
    {
      *presult = filepath;


      break;
    }
  }
}



wn_file wn_fopen(char file_name[],char type[])
{
  char *failure;
  wn_file ret;


  errno = 0;


  ret = fopen(file_name,type);


  if(ret == NULL)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_fopen",file_name,failure);
  }


  return(ret);
}



int wn_fclose(wn_file f)
{
  int ret;
  char *failure;


  errno = 0;


  ret = fclose(f);


  if(ret == EOF)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_fclose","?",failure);
  }


  return(ret);
}



int wn_fgets(char *s,int n,wn_file stream)
{
  int ret;
  char *failure;

  errno = 0;

  ret = (fgets(s,n,stream) != NULL);

  if((ret == 0)&&(errno != 0))
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_fgets","?",failure);
  }

  return(ret);
}


int wn_fputs(char *s,wn_file stream)
{
  int ret;
  char *failure;


  errno = 0;


  ret = fputs(s,stream);


  if((ret == EOF)&&(!(wn_streq(s,""))))
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_fputs","?",failure);
  }


  return(ret);
}
   


int wn_getchar(void)
{
  return(wn_getc(wn_stdin));
}



int wn_getc(wn_file stream)
{
  int ret;
  char *failure;


  errno = 0;


  ret = getc(stream);


  if((ret == EOF)&&(errno != 0))
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_getc","?",failure);
  }


  return(ret);
}
   


int wn_ungetc(int c,wn_file stream)
{
  int ret;
  char *failure;


  errno = 0;


  ret = ungetc(c,stream);


  if(ret == EOF)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_ungetc","?",failure);
  }


  return(ret);
}
   


int wn_putc(char c,wn_file stream)
{
  int ret;
  char *failure;


  errno = 0;


  ret = putc(c,stream);


  if((ret == EOF)&&(errno != 0))
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_getc","?",failure);
  }


  return(ret);
}



bool wn_feof(wn_file f)
{
  return(feof(f));
}



int wn_fflush(wn_file stream)
{
  int ret;
  char *failure;


  ret = fflush(stream);


  if(ret != 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_fflush","?",failure);
  }


  return(ret);
}


#ifndef WN_WINDOWS
/*     I tried running this with MSVC++ 4 on Windows ME, and it
** just doesn't work.  Blue screen of death. */
wn_file wn_popen(char command[],char type[])
{
  char *failure;
  wn_file ret;
# ifndef __cplusplus
    extern FILE *popen(const char *, const char *);
    /* 					maybe redundant, sometime not */
# endif

  errno = 0;


#ifdef WN_WINDOWS
  ret = _popen(command,type);
#else
  ret =  popen(command,type);
#endif


  if(ret == NULL)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_popen",command,failure);
  }


  return(ret);
}



int wn_pclose(wn_file f)
{
  int ret;
  char *failure;
# ifndef __cplusplus
    extern int pclose (FILE *__stream);	/* sometimes redundant, sometimes not */
# endif


  errno = 0;


# ifdef WN_WINDOWS
    ret = _pclose(f);
# else
    ret =  pclose(f);
# endif


  /*     the man page clearly says pclose returns -1 on error (not
  ** necessarily EOF) */
  if(ret == -1)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_pclose","?",failure);
  }


  return(ret);
}
#endif /* WN_WINDOWS */



int wn_open(char *path,int flags,int mode)
{
  int ret;
  char *failure;


  errno = 0;


# if defined(WN_WINDOWS)  ||  defined(__CYGWIN__)
    if (!(O_TEXT & flags))
    /**/		   flags |= O_BINARY;

    wn_assert(!(O_TEXT & flags) != !(O_BINARY & flags));
# endif

  ret = open(path,flags,mode);

  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_open",path,failure);
  }


  return(ret);
}
   


int wn_close(int d)
{
  int ret;
  char *failure;


  errno = 0;


  ret = close(d);


  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_close","?",failure);
  }


  return(ret);
}
   


int wn_write(int d,char *buf,int nbytes)
{
  int ret;
  char *failure;


  errno = 0;


  ret = write(d,buf,nbytes);


  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_write","?",failure);
  }


  return(ret);
}
   


int wn_read(int d,char *buf,int nbytes)
{
  int ret;
  char *failure;


  errno = 0;


  ret = read(d,buf,nbytes);


  if(ret <= 0)
  {
    if (0 == ret  &&  !errno)
    /**/		      return 0;

    translate_errno(&failure);
    (*pcurrent_error_func)("wn_read","?",failure);
  }


  return(ret);
}
   


int wn_mv(char from[],char to[])
{
  int ret;
  char *failure;
  char dest[WN_MAX_FILE_NAME_PATH_LEN];
  char *pc;

  errno = 0;

  if (wn_is_directory(to)) {
    /* we are to copy 'from' into the directory 'to' */

    strcpy(dest, to);
    pc = dest + strlen(dest);
    while (pc > dest  &&  pc[-1] == '/') {
      -- pc;
      *pc = 0;
    }
    strcat(dest, WN_OSSLASH);
    /* get basename of 'from' into pc */
    pc = from + strlen(from);
    while (pc > from  &&  pc[-1] == '/') {
      -- pc;
      *pc = 0;
    }
    while (pc > from  &&  pc[-1] != '/') {
      -- pc;
    }
    strcat(dest, pc);
    lo_check_buffer_overflow(dest, "wn_mv");

    to = dest;
  }

  ret = rename(from,to);
  if (ret < 0  &&  EXDEV == errno)
  {
    /* we tried to rename across filesystems, which often doesn't work */

    errno = 0;
    ret = wn_cp(from, to);
    if (0 > ret)
    {
      return ret;
    }
    ret = wn_rm(from);
    if (0 > ret)
    {
      return ret;
    }
  }

  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_mv","?",failure);
  }


  return(ret);
}


/* return 0 on success, -1 on failure */
int wn_cp(char *from, char *to) {
  struct stat stat_buf;
  int mode;
  char *failure;
  char dest[WN_MAX_FILE_NAME_PATH_LEN];
  char iobuf[64 << 10];		/* 64K - note MSVC++ 4.0 has a 1MB stack */
  char *pc;
  int fd_from, fd_to;
  int chars_read, chars_written, chars_in_buf_written;
  int sts;

  errno = 0;

  /* get mode of file to copy to new file */
  sts = stat(from, &stat_buf);
  if (sts) {
    if (!errno)
    /**/	errno = EIO;
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_cp","?",failure);
    return -1;
  }
  mode = stat_buf.st_mode & 0777;

  fd_from = wn_open(from, O_RDONLY, mode);
  if (0 > fd_from) {
    if (!errno)
    /**/	errno = EIO;
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_cp","?",failure);
    return -1;
  }

  if (wn_is_directory(to)) {
    /* we are to copy 'from' into the directory 'to' */

    strcpy(dest, to);
    pc = dest + strlen(dest);
    while (pc > dest  &&  pc[-1] == '/') {
      -- pc;
      *pc = 0;
    }
    strcat(dest, WN_OSSLASH);
    /* get basename of 'from' into pc */
    pc = from + strlen(from);
    while (pc > from  &&  pc[-1] == '/') {
      -- pc;
      *pc = 0;
    }
    while (pc > from  &&  pc[-1] != '/') {
      -- pc;
    }
    strcat(dest, pc);
    lo_check_buffer_overflow(dest, "wn_cp");

    to = dest;
  }

  wn_rmf(to);
  wn_assert(!wn_file_exists(to));

  fd_to = wn_open(to, O_CREAT|O_WRONLY, mode);
  if (0 > fd_to) {
    if (!errno)
    /**/	errno = EIO;
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_cp","?",failure);
    return -1;
  }

  do {
    chars_read = wn_read(fd_from, iobuf, sizeof(iobuf));
    if (chars_read > 0) {
      for (chars_in_buf_written = 0;  chars_in_buf_written < chars_read;
      /**/			      chars_in_buf_written += chars_written) {
        chars_written = wn_write(fd_to, iobuf + chars_in_buf_written,
	/**/				chars_read - chars_in_buf_written);
	if (0 >= chars_written) {
	  if (!errno)
	  /**/	      errno = EIO;
	  translate_errno(&failure);
	  (*pcurrent_error_func)("wn_cp","?",failure);
	  return -1;
	}
	wn_assert(chars_written > 0);
      }
    } else if (0 > chars_read) {
      wn_assert(-1 == chars_read);
      if (!errno)
      /**/        errno = EIO;
      translate_errno(&failure);
      (*pcurrent_error_func)("wn_cp","?",failure);
      return -1;
    }
  } while (chars_read);

  sts = wn_close(fd_from);
  if (sts) {    
    if (!errno)
    /**/	errno = EIO;
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_cp","?",failure);
    return -1;
  }
  sts = wn_close(fd_to);
  if (sts) {    
    if (!errno)
    /**/	errno = EIO;
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_cp","?",failure);
    return -1;
  }

  return 0;
} /* wn_cp */
  

int wn_rm(char path[])
{
  int ret;
  char *failure;


  errno = 0;


#ifdef vms
  ret = delete(path);
#else
  ret = unlink(path);
#endif


  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_rm","?",failure);
  }


  return(ret);
}



void wn_rmf(char *path)
{
#ifdef vms
  (void)delete(path);
#else
  (void)unlink(path);
#endif
}



int wn_mkdir(char dir_name[])
{
  int ret;
  char *failure;


  errno = 0;


# ifdef WN_WINDOWS
    ret = mkdir(dir_name);
# else
    ret = mkdir(dir_name,DEFAULT_MKDIR_MODE);
# endif

  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_mkdir","?",failure);
  }


  return(ret);
}



#if !defined(WN_VMS)
  int wn_rmdir(char dir_name[])
  {
    int ret;
    char *failure;


    errno = 0;


    ret = rmdir(dir_name);


    if(ret < 0)
    {
      translate_errno(&failure);
      (*pcurrent_error_func)("wn_rmdir","?",failure);
    }


    return(ret);
  }
#endif


int wn_cd(char dir_name[])
{
  int ret;
  char *failure;


  errno = 0;


  ret = chdir(dir_name);


  if(ret < 0)
  {
    translate_errno(&failure);
    (*pcurrent_error_func)("wn_cd","?",failure);
  }


  return(ret);
}



void wn_pushd(char dir_name[])
{
  char old_dir[WN_MAX_FILE_NAME_PATH_LEN],*old_dir_copy;


  wn_gppush(wn_defaultgp());


  wn_pwd(old_dir);
  wn_stracpy(&old_dir_copy,old_dir);


  wn_sllins(&directory_stack,old_dir_copy);


  wn_cd(dir_name);


  wn_gppop();
}



void wn_popd(void)
{
  char *old_dir_copy;


  wn_gppush(wn_defaultgp());


  old_dir_copy = (char *)(directory_stack->contents);
  wn_slledel(&directory_stack);


  wn_cd(old_dir_copy);


  wn_free((ptr)old_dir_copy);


  wn_gppop();
}



int wn_pwd(char dir_name[])
{
  int ret;
  char *failure;


/*     put in !! rather than cast from ptr to int, 64 bit platforms were
** getting warnings */
#if defined(WN_VMS) || defined(__cplusplus) || defined(WN_WINDOWS) || \
				       defined(linux) || defined(__linux__)
  ret = !!getcwd(dir_name, WN_MAX_FILE_NAME_PATH_LEN);
#else
  ret = !!getwd(dir_name);
#endif
  lo_check_buffer_overflow(dir_name, "wn_pwd");


  if(ret == 0)
  {
    failure = dir_name;
    (*pcurrent_error_func)("wn_pwd","<none>",failure);
  }


  return(ret);
}


bool wn_file_exists(char path[])
{
#ifdef vms
  char dirpath[WN_MAX_FILE_NAME_PATH_LEN];
#endif
  struct stat statbuf;

  if (stat(path,&statbuf) < 0)
  {
#ifdef vms
    strcpy(dirpath,path);
    strcat(dirpath,".dir;1");
    if (stat(dirpath,&statbuf) < 0)
    {
      return(FALSE);
    }
    else
    {
      return(TRUE);
    }
#else
    return(FALSE);
#endif
  }
  else
  {
    return(TRUE);
  }
}


bool wn_is_directory(char path[])
{
  struct stat statbuf;
#ifdef vms
  char dirpath[WN_MAX_FILE_NAME_PATH_LEN];

  strcpy(dirpath,path);
  strcat(dirpath,".dir;1");
#else
  char *dirpath = path;
#endif

  if (stat(dirpath,&statbuf) < 0)
  {
    return(FALSE);
  }
  else
  {
#   if defined(S_ISDIR)
      if (S_ISDIR(statbuf.st_mode))
#   else
      if (statbuf.st_mode & S_IFDIR)
#   endif
    {
      return(TRUE);
    }
    else
    {
      return(FALSE);
    }
  }
}
