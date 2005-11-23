/*      $OSSEC, debug_op.c, v0.2, 2004/08/02, Daniel B. Cid$      */

/* Copyright (C) 2004 Daniel B. Cid <dcid@ossec.net>
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

/* v0.2: 2005/10/27: Better handlers 
 * v0.1: 2004/08/02
 */

/* Part of the OSSEC HIDS
 * Available at http://www.ossec.net/hids/
 */

#include <sys/types.h>
#include <sys/stat.h>
     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>

#include "headers/defs.h"
#include "headers/debug_op.h"


int dbg_flag = 0;
int chroot_flag = 0;
int daemon_flag = 0;

/* For internal logs */
#ifndef LOGFILE
#define LOGFILE   "/logs/ossec.log"
#endif


/* _log function */
void _log(const char * msg,va_list args)
{
    time_t tm;
    struct tm *p;

    FILE *fp;
    
    tm = time(NULL);
    p = localtime(&tm);


    /* If under chroot, log directly to /logs/ossec.log */
    if(chroot_flag == 1)
    {
        fp = fopen(LOGFILE, "a");
    }
    else
    {
        char _logfile[256];
        snprintf(_logfile, 256, "%s%s", DEFAULTDIR, LOGFILE);
        fp = fopen(_logfile, "a");
    }

    if(fp)
    {
        (void)fprintf(fp,"%d/%02d/%02d %02d:%02d:%02d ",
                      p->tm_year+1900,p->tm_mon+1, 
                      p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
        (void)vfprintf(fp, msg, args);
        (void)fprintf(fp, "\n");
        fflush(fp);
        fclose(fp);
    }


    /* Only if not in daemon mode */
    if(daemon_flag == 0)
    {
        /* Print to stderr */		
        (void)fprintf(stderr,"%d/%02d/%02d %02d:%02d:%02d ",
                      p->tm_year+1900,p->tm_mon+1 ,p->tm_mday,
                      p->tm_hour,p->tm_min,p->tm_sec);
        (void)vfprintf(stderr, msg, args);
        (void)fprintf(stderr, "\n");
    }
}


void debug1(const char * msg,...)
{
    if(dbg_flag >= 1)
    {
        va_list args;
        va_start(args, msg);

        _log(msg, args);

        va_end(args);
    }
}

void debug2(const char * msg,...)
{
    if(dbg_flag >= 2)
    {
        va_list args;
        va_start(args, msg);
        _log(msg, args);
        va_end(args);
    }
}

void merror(const char * msg,... )
{
    va_list args;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
}

void verbose(const char * msg,... )
{
    va_list args;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
}

void ErrorExit(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);

    exit(1);
}


void nowChroot()
{
    chroot_flag = 1;
}


void nowDaemon()
{
    daemon_flag = 1;
}


void nowDebug()
{
    dbg_flag++;
}

int isChroot()
{
    return(chroot_flag);
}

/* EOF */			
