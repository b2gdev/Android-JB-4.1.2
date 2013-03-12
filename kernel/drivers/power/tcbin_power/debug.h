#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MAX_PROC_BUFFER_SIZE		(PAGE_SIZE * 4)
#define MAX_PROC_FILE_NAME_SIZE		32
/* #define TCBIN_POWER_DEBUG		1 */	/* uncomment this line to direct debug messages to console */

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

extern int proc_enable;
extern struct proc_file debug_file;
extern struct proc_file error_file;
extern struct proc_file packet_file;
extern int PERROR_counter;	
extern int PDEBUG_counter;

int create_proc_file(struct proc_file *file, struct proc_dir_entry *parent, const char* entry_name, int size, struct seq_operations *seq_ops, struct file_operations *fops);
int remove_proc_file(struct proc_file *file);
int write_proc_buffer(struct proc_file *file, const char *buffer, unsigned int count);
int create_proc_file_debug(struct proc_dir_entry *proc_dir, int size);
int create_proc_file_error(struct proc_dir_entry *proc_dir, int size);
int create_proc_file_packet(struct proc_dir_entry *proc_dir, int size);

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef TCBIN_POWER_DEBUG
	#define PDEBUG(fmt, args...) printk("tcbin_power: " fmt, ## args)
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
