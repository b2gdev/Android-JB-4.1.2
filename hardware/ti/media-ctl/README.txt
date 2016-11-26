Acquired from 
	https://github.com/suapapa/media-ctl

Bracnch 
	Master

Commit ID
	0d52ce46b5d30f5fb1e1eaf18a37a4e0a262ab47

Changes
	src/linux/media.h
Remove
	#define MEDIA_IOC_DEVICE_INFO		_IOWR('|', 0x00, struct media_device_info)
	#define MEDIA_IOC_ENUM_ENTITIES		_IOWR('|', 0x01, struct media_entity_desc)
	#define MEDIA_IOC_ENUM_LINKS		_IOWR('|', 0x02, struct media_links_enum)
	#define MEDIA_IOC_SETUP_LINK		_IOWR('|', 0x03, struct media_link_desc)

Add
	#define MEDIA_IOC_DEVICE_INFO		_IOWR('M', 1, struct media_device_info)
	#define MEDIA_IOC_ENUM_ENTITIES		_IOWR('M', 2, struct media_entity_desc)
	#define MEDIA_IOC_ENUM_LINKS		_IOWR('M', 3, struct media_links_enum)
	#define MEDIA_IOC_SETUP_LINK		_IOWR('M', 4, struct media_link_desc)
 

