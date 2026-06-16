// FON.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GBK16.h"
#include "string.h"

#define PING "苹"

/* data defines */
static char name_buffer[20][200];
static unsigned char read_buffer[10*1024*1024];
/* hz */
static unsigned char hz_buf[10000][32];
static unsigned short hz_index[10000];
static unsigned char hz_original[5000];
static unsigned int hz_original_cnt = 0;
static unsigned int hz_cnt = 0;
/* find hz */
int find_all_hz(unsigned char * hzb,unsigned int len)
{
	char * str_pos = (char *)hzb;
	/* 找到关键字段 */
	while(1)
	{
		/* 找到关键位置 */
		char * pos = strstr(str_pos,(const char *)"GBK2312_ASSETS_HZ16");
		/* 获取数据 */
		if( pos != NULL )
		{
			/* 关键字长度 */
			int slen = strlen("GBK2312_ASSETS_HZ16");
			/* 获取位置 */
			unsigned char * spos = (unsigned char *)pos + slen;
			/* 判断格式 */
			if( spos[0] != '(' || spos[1] != '"' )
			{
				printf("format error \r\n");
				/* 直接返回 */
				return (-1);
			}
			/* 定位到数据位置 */
			spos += 2;
			/* copy data */
			int j = 0;
			/* tes */
			for( j = 0 ; spos[j] != '"' ; j ++ )
			{
				hz_original[hz_original_cnt] = spos[j];
				hz_original_cnt++;
			}
			/* 判断长度，如果都是汉字应该是偶数字节数*/
			if( hz_original_cnt % 2 )
			{
				printf("len error \r\n");
				/* 直接返回 */
				return (-1);
			}
			/* str_pos_i */
			str_pos = pos + slen;
		}
		else
		{
			/* return break */
			return hz_original_cnt;
		}
	}
}
/* get offset from gbk2312 */
int get_gbk_offset(char * hz_code)
{
	/* hz_code */
	unsigned char high = hz_code[0];
	unsigned char low = hz_code[1];
#if 0
	printf("high:%x, low:%x\n", high, low);
#endif	
	if (low < 0x80)
	{
		return ((high-0x81)*190 + (low-0x40))*32;
	}
	else
	{
		return ((high-0x81)*190 + (low-0x41))*32;
	}
}
/* Tchar to char */
int Tchar_to_char(_TCHAR * tchar,char * buffer)
{
    int i = 0;
	/* set buffer to zero */
	memset(buffer,0,sizeof(buffer));
	/* offset */
	while( *tchar != '\0' )
	{
       buffer[i] = (char)(*tchar);
	   tchar ++;
	   i++;
	}
	/* return  */
	return i;
}
/*------------------*/
int _tmain(int argc, _TCHAR* argv[])
{
	/* 转换TCHAR to char * */
  	for(int i = 1 ; i< argc ; i++ )
	{
       Tchar_to_char(argv[i],name_buffer[i-1]);
	}
#if 0
	memcpy(name_buffer[0],"hz_lib.c",8);
	memcpy(name_buffer[1],"hz_out.h",8);
#endif
	/* 打开第一条位置的文件 */
	FILE * dir_file;
	/* */
	fopen_s(&dir_file,name_buffer[0],"rb");
	/* 打开文件是否成功 */
    if( dir_file == NULL )
	{
		/* 打开失败直接中断，打印信息 */
		printf("Can not open this file: %s,please check it up\r\n");
		/* break */
		return 0;
	}
	/* 将文件全部读取进来 ，前提是不能大于10M */
	int len = fread(read_buffer,1,sizeof(read_buffer),dir_file);
	/* 文件关闭 */
	fclose(dir_file);
	/* 判断文件是否大于10MB ，如果大于则不进行处理 */
	if( len >= sizeof(read_buffer) )
	{
		printf("file is over 10MB ,can not supply \r\n");
		/* 中断 */
		return 0;
	}
	/* 从输入文件中找到所有需要搜索的汉字 */
	int ret = find_all_hz(read_buffer,len);
	/* 转换到中间缓存 */
	for( int i = 0 ; i < ret  ; i += 2 )
	{
		int offset = get_gbk_offset((char *)&hz_original[i]);
		/* 获取字模数据 */
		for( int j = 0 ; j < 32 ; j ++ )
		{
			hz_buf[hz_cnt][j] = _acGBK16[ j + offset ];
		}
		/* 获取index */
		hz_index[hz_cnt] = *((unsigned short *)&hz_original[i]);
		/* hz ++ */
		hz_cnt ++;
	}
	/* create out file */
	FILE * file_create;
	/* 创建文件 */
	fopen_s(&file_create,name_buffer[1],"wb+");
    /* 如果文件创建失败则直接退出 */
	if( file_create == NULL )
	{
		printf("This file create failed:%s\r\n",name_buffer[1]);
		/* 直接返回 */
		return (-1);
	}
	/* 文件创建成功 */
	char s_buf[100];
    #define HEADER "/* \r\n	This file created by software automatic , do not change \r\n*/"
    #define INDEX_HEADER "\r\nconst unsigned short HZ_INDEX[] = \r\n{\r\n" 
    #define INDEX_TAIL "\r\n};\r\n"
    #define ENTER_CTRL_T  "\r\n"
	/* 写索引文件 */
	fwrite(HEADER,1,strlen(HEADER),file_create);
	/* 写索引头文件 */
	fwrite(INDEX_HEADER,1,strlen(INDEX_HEADER),file_create);
	/* static unsisgned */
	static unsigned int enter_ctrl = 0;
	/* 循环写索引 */
	for( unsigned int i = 0 ; i < hz_cnt ; i ++ )
	{
		/* 生成写入的数据 */
		sprintf_s(s_buf,"0x%04x,",hz_index[i]);
		/* 写入文件 */
		fwrite(s_buf,1,strlen(s_buf),file_create);
        /* 写入回车数据的时机 */
		enter_ctrl ++ ;
		/* 每8个数据写入一个数据 */
		if( enter_ctrl >= 8 )
		{
		   enter_ctrl = 0;
		   fwrite(ENTER_CTRL_T,1,strlen(ENTER_CTRL_T),file_create);
		}
	}
	/* 写入文件结尾 */
	fwrite(INDEX_TAIL,1,strlen(INDEX_TAIL),file_create);
	/* write dtata */
	#define DATA_HEADER "const unsigned char HZ_DATAX[][32] = \r\n{\r\n"
	#define DATA_TAIL "};\r\n"
    #define DATA_GROUP "{\r\n"
    #define DATA_GROUPEND "},\r\n"
    #define DATA_COMMON "\r\n/* HZ data */ \r\n"
	/* 清零文件 */
	enter_ctrl = 0;
	/* 写入注释文件 */
	fwrite(DATA_COMMON,1,strlen(DATA_COMMON),file_create);
    /* 写数据头文件 */
	fwrite(DATA_HEADER,1,strlen(DATA_HEADER),file_create);
	/* 循环写入字模数据 */
	for( unsigned int i = 0 ; i < hz_cnt ; i ++ )
	{
		/* 写入分组数据 */
		fwrite(DATA_GROUP,1,strlen(DATA_GROUP),file_create);
		/* ------------ */
		/* 生成写入的数据 */
		for( int j = 0 ; j < 32 ; j ++ )
		{
		   sprintf_s(s_buf,"0x%02x,",hz_buf[i][j]);
		   /* 写入文件 */
		   fwrite(s_buf,1,strlen(s_buf),file_create);
		   /* 写入回车数据的时机 */
		   enter_ctrl ++ ;
		   /* 每8个数据写入一个数据 */
		   if( enter_ctrl >= 8 )
		   {
			   enter_ctrl = 0;
			   fwrite(ENTER_CTRL_T,1,strlen(ENTER_CTRL_T),file_create);
		   }
		}
		/* 写入分组数据 */
		fwrite(DATA_GROUPEND,1,strlen(DATA_GROUPEND),file_create);
		/* ------------ */
	}
	/* 写入结尾数据 */
	fwrite(DATA_TAIL,1,strlen(DATA_TAIL),file_create);
	
	fclose(file_create);

	printf("OK HZ are %d\r\n",hz_cnt);
    /*int oname_buffer[1]ffset = get_gbk_offset(PING);

	printf("%d,%s",argc,argv[0]);
	unsigned char hz_dd[32];

	for(int i = 0; i < 32; i++)
	{
		hz_dd[i] = _acGBK16[offset+i];
	}

	FILE * ddt = fopen("hz_lib.c","rb");

	if( ddt == 0 )
	{
		printf("error\r\n");
		return 0;
	}

	int len = fread(buffer_h,1,sizeof(buffer_h),ddt);
	char hz_b[100];

	for( int i = 0 ; i < len ; i ++ )
	{
		if ( sscanf(&buffer_h[i],"GBK2312_HZ16(\"%s\");",hz_b) )
		{
			printf("%d\r\n",i);
		}
	}*/

	return 0;
}

