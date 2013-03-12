#include <linux/string.h>
#include <linux/errno.h>	/* error codes */
#include <linux/module.h>
#include "debug.h"

#define add_proc_file(name)						\
									\
struct proc_file name##_file;						\
									\
static int open_proc_##name(struct inode *inode, struct file *file)  	\
{ 									\
	return seq_open(file, name##_file.seq_ops); 			\
} 									\
									\
static void *proc_seq_start_##name(struct seq_file *s, loff_t *pos)	\
{									\
	if (*pos >= 1)							\
		return NULL;						\
									\
	return &name##_file;						\
}									\
									\
static struct seq_operations seq_ops_##name = {				\
	.start = proc_seq_start_##name,					\
	.next  = proc_seq_next,						\
	.stop  = proc_seq_stop,						\
	.show  = proc_seq_show						\
};									\
									\
static struct file_operations fops_##name = {				\
	.owner   = THIS_MODULE,						\
	.open    = open_proc_##name,					\
	.read    = seq_read,						\
	.llseek  = seq_lseek,						\
	.release = seq_release						\
};									\
									\
int create_proc_file_##name(struct proc_dir_entry *proc_dir, int size) 	\
{									\
									\
	memset(&name##_file, 0, sizeof(struct proc_file));		\
									\
	if (!proc_dir) {						\
		return -EINVAL;						\
	}								\
									\
	if (create_proc_file(&name##_file, proc_dir, #name, size, &seq_ops_##name, &fops_##name) < 0) {	\
		return -EFAULT;						\
	}								\
									\
	return 0;							\
}									\

int PERROR_counter =1;
int PDEBUG_counter =1;
// common
static void *proc_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	if (*pos >= 1)
		return NULL;
	return v;
}

static void proc_seq_stop(struct seq_file *s, void *v)
{
	/* Nothing to do here */
}

static int proc_seq_show(struct seq_file *s, void *v)
{
	int i = 0;
	int len1, len2;
	struct proc_file *file = (struct proc_file*) v;

	if (file != NULL) {
		if (file->len > 0) {
			if (file->len < file->size) {
				for (i = 0; i < file->len; i++) {
					seq_putc(s, file->buffer[i]);
				}				
			}
			else {
				len1 = (file->end - file->in + 1);
				len2 = file->len - len1;

				for (i = 0; i < len1; i++) {
					seq_putc(s, file->in[i]);
				}				

				for (i = 0; i < len2; i++) {
					seq_putc(s, file->buffer[i]);
				}				
			}
		}	
	}
	else {
		return -EINVAL;
	}	

	return 0;
}

int proc_buffer_init(struct proc_file *file, int size)
{
	file->buffer = kmalloc(size, GFP_KERNEL);

	if (!file->buffer) {
		return -ENOMEM;
	}

	file->size = size;
	file->in = file->buffer;
	file->end = file->buffer + (size - 1);
	file->len = 0;

	return 0;
}

int proc_buffer_free(struct proc_file *file)
{
	kfree(file->buffer);
	file->buffer = NULL;

	return 0;
}

int write_proc_buffer(struct proc_file *file, const char *buffer, unsigned int count)
{
	int ret = 0;
	int len;
	int len1, len2;

	if (file != NULL) {
		if (count > 0) {
			if (count > file->size) {
				len = file->size;
			}
			else {
				len = count;
			}
	
			if (len < (file->end - file->in + 1)) {
				memcpy(file->in, buffer, len);
				file->in += len;
			}
			else if (len == (file->end - file->in + 1)) {
				memcpy(file->in, buffer, len);
				file->in = file->buffer;
			}
			else {
				len1 = (file->end - file->in + 1);
				len2 = len - len1;
	
				memcpy(file->in, buffer, len1);
				file->in = file->buffer;
				memcpy(file->in, (buffer + len1), len2);
				file->in += len2;
			}
	
			if (file->len < file->size) {
				file->len += len;

				if (file->len > file->size) {
					file->len = file->size;
				}
			}

			ret = len;
		}
		else {
			return -EINVAL;
		}
	}
	else {
		return -EINVAL;
	}

	return ret;
}

int create_proc_file(struct proc_file *file, struct proc_dir_entry *parent, const char* entry_name, int size, struct seq_operations *seq_ops, struct file_operations *fops)
{
	int ret = 0;

	if (file != NULL) {
		memset(file, 0, sizeof(struct proc_file));

		if (size <= MAX_PROC_BUFFER_SIZE) {
			ret = proc_buffer_init(file, size);

			if (ret < 0) {
				return ret; 
			}
		}
		else {
			return -EINVAL;
		}
	
		if (strlen(entry_name) < MAX_PROC_FILE_NAME_SIZE) {
			strcpy(file->entry_name, entry_name);
		}
		else {
			return -EINVAL;
		}	

		file->dir = parent;
		file->entry = create_proc_entry(entry_name, 0, file->dir);

		if (file->entry)
			file->seq_ops = seq_ops;
			file->fops = fops;

			file->entry->proc_fops = file->fops;
		}
		else {
			ret = -EINVAL;	
		}

	return ret;
}

int remove_proc_file(struct proc_file *file)
{
	int ret = 0;

	if (file != NULL) {
		remove_proc_entry(file->entry_name, file->dir);
		file->entry = NULL;

		if (proc_buffer_free(file) < 0) {
			printk(KERN_ERR "error : remove_proc_file - trying to remove unallocated buffer\r\n");
		}
	}
	else {
		return -EINVAL;
	}

	return ret;
}

/* Add your proc file here */
add_proc_file(debug)	/* eg - /proc/<dir>/debug */
add_proc_file(error)
add_proc_file(packet)
		
