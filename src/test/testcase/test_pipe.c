/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
** ��   ��   ��: test_pipe.c
** ��   ��   ��: �ܽ���
** ����޸�����: 2018/01/09 16:29:55
** ��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2017/10/22 16:29:55
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: ��Ԫ���������Ĳ���ģ��
** ���ļ���C����Դ�ļ�ģ���������ɡ�------------��纣����Ʒ��������Ʒ��------------
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


/*********************************************ͷ�ļ�����***********************************************/
#include "cut.h"
#include "wind_pipe.h"
#include "wind_string.h"
/********************************************�ڲ���������**********************************************/

w_uint8_t pipebuf[128];


/********************************************�ڲ���������*********************************************/



/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/


CASE_SETUP(pipeinfo)
{

}

CASE_TEARDOWN(pipeinfo)
{

}

CASE_FUNC(pipeinfo)
{
    w_err_t err;
    pipe_s *pipe;
    pipe = wind_pipe_create("test",pipebuf,sizeof(pipebuf));
    EXPECT_NE(pipe,NULL);
    EXPECT_EQ(pipe->magic,WIND_PIPE_MAGIC);
    EXPECT_STR_EQ(pipe->name,"test");
    EXPECT_EQ(pipe->used,B_TRUE);
    EXPECT_EQ(pipe->buff,pipebuf);
    EXPECT_EQ(pipe->buflen,sizeof(pipebuf));
    err = wind_pipe_free(pipe);
    EXPECT_EQ(ERR_OK,err);

}

CASE_SETUP(pipefunc)
{

}

CASE_TEARDOWN(pipefunc)
{

}

CASE_FUNC(pipefunc)
{
    w_err_t err;
    w_int32_t i;
    w_int32_t res;
    pipe_s *pipe;
    w_int8_t buf[14];
    pipe = wind_pipe_create("test",pipebuf,sizeof(pipebuf));
    EXPECT_NE(pipe,NULL);
    for(i = 0;i < 10;i ++)
    {
        res = wind_pipe_write(pipe,"test123456789",13);
        EXPECT_EQ(res,13);
        res = wind_pipe_read(pipe,buf,14);
        EXPECT_EQ(res,13);
        res = wind_memcmp(buf,"test123456789",13);
    }
    err = wind_pipe_free(pipe);
    EXPECT_EQ(ERR_OK,err);

}





SUITE_SETUP(test_pipe)
{

}

SUITE_TEARDOWN(test_pipe)
{

}


TEST_CASES_START(test_pipe)
TEST_CASE(pipeinfo)
TEST_CASE(pipefunc)
TEST_CASES_END
TEST_SUITE(test_pipe)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus