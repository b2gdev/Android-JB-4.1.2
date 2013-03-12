#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MAX_PROC_BUFFER_SIZE		(PAGE_SIZE * 4)
#define MAX_PROC_FILE_NAME_SIZE		32

/* driver specific definition, define driver name here, that's all folks */
#define DRIVER_NAME        w2sg0006

/* uncomment this line to direct debug messages to console */
//#define DEBUG_TO_CONSOLE			1

/* some useful macros for dynamic code generation */
#define STR_EXPAND(a)      #a
#define STR(a)             STR_EXPAND(a)
#define CONC_EXPAND(a,b)   a ## b
#define CONC(a,b)          CONC_EXPAND(a,b)

/* message type definitions, will be used in debug.c */
#define DEBUG     CONC(DRIVER_NAME, _debug)
#define ERROR     CONC(DRIVER_NAME, _error)
#define PACKET    CONC(DRIVER_NAME, _packet)

struct proc_file {
	char			dir_name[MAX_PROC_FILE_NAME_SIZE];
	char			entry_name[MAX_PROC_FILE_NAME_SIZE];
	struct proc_dir_entry	*entry, *dir;
	unsigned char 		*buffer;
	unsigned int 		size;
	unsigned char 		*in;
	unsigned char		*end;
	unsigned int		len;
	spinlock_t		lock;
	struct seq_operations	*seq_ops;
	struct file_operations	*fops;
};

/* file definitions */
extern struct proc_file CONC(DEBUG, _file);
extern struct proc_file CONC(ERROR, _file);
extern struct proc_file CONC(PACKET, _file);
#define debug_file   CONC(DEBUG, _file)
#define error_file   CONC(ERROR, _file)
#define packet_file  CONC(PACKET, _file)

/* other variable definitions */
extern int CONC(DEBUG, _counter);
extern int CONC(ERROR, _counter);
extern int CONC(DRIVER_NAME, _proc_enable);
#define PDEBUG_counter  CONC(DEBUG, _counter)
#define PERROR_counter  CONC(ERROR, _counter)
#define proc_enable     CONC(DRIVER_NAME, _proc_enable)

/* function prototypes */
int CONC(remove_proc_file_, DRIVER_NAME)(struct proc_file *file);
int CONC(write_proc_buffer_, DRIVER_NAME)(struct proc_file *file, const char *buffer, unsigned int count);
int CONC(create_proc_file_, DEBUG)(struct proc_dir_entry *proc_dir, int size);
int CONC(create_proc_file_, ERROR)(struct proc_dir_entry *proc_dir, int size);
int CONC(create_proc_file_, PACKET)(struct proc_dir_entry *proc_dir, int size);
#define remove_proc_file(a)            CONC(remove_proc_file_, DRIVER_NAME)(a)
#define write_proc_buffer(a,b,c)       CONC(write_proc_buffer_, DRIVER_NAME)(a,b,c)
#define create_proc_file_debug(a,b)    CONC(create_proc_file_, DEBUG)(a,b)
#define create_proc_file_error(a,b)    CONC(create_proc_file_, ERROR)(a,b)
#define create_proc_file_packet(a,b)   CONC(create_proc_file_, PACKET)(a,b)

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef DEBUG_TO_CONSOLE
	#define PDEBUG(fmt, args...) printk(STR(CONC(DRIVER_NAME,_:)) fmt, ## args)
#else
	#define PDEBUG(fmt,args...) { \
		if(proc_enable){unsigned char proc_buf[256]; \
			int proc_len = 0; \
			proc_len += scnprintf((proc_buf + proc_len), sizeof(proc_buf), "(%d)  [%u] ", PDEBUG_counter, (unsigned int)jiffies); \
			proc_len += scnprintf((proc_buf + proc_len),sizeof(proc_buf),fmt,## args); \
			write_proc_buffer(&debug_file, proc_buf, proc_len); \
			PDEBUG_counter++;\
		}\
	}
#endif

#undef PERROR             /* undef it, just in case */
#define PERROR(fmt,args...) { \
	if(proc_enable){unsigned char proc_buf[256]; \
		int proc_len = 0; \
		proc_len += scnprintf(proc_buf + proc_len, sizeof(proc_buf), "(%d)  [%u] ", PERROR_counter, (unsigned int)jiffies); \
		proc_len += scnprintf(proc_buf + proc_len, sizeof(proc_buf), fmt,## args); \
		write_proc_buffer(&error_file, proc_buf, proc_len); \
		PERROR_counter++;\
	}\
}

#undef PDEBUG_PACKET
#define PDEBUG_PACKET(fmt,args...) { \
	if(proc_enable){unsigned char proc_buf[256]; \
		int proc_len; \
		proc_len = sprintf(proc_buf,fmt,## args); \
		write_proc_buffer(&packet_file, proc_buf, proc_len); \
	} \
}

#endif	/* _DEBUG_H_ */
