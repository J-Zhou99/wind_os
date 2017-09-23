/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
** ��   ��   ��: test_mode.h / test_mode.c
** ��   ��   ��: �ܽ���
** ����޸�����: 2015/1/24 16:29:55
** ��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2015/1/24 16:29:55
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

/********************************************�ڲ���������**********************************************/



/********************************************�ڲ���������*********************************************/



/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/


CASE_SETUP(Test1)
{
    test_printf("Test1 setup\r\n");
}

CASE_TEARDOWN(Test1)
{
    test_printf("Test1 teardown\r\n");
}

CASE_FUNC(Test1)
{
    test_printf("test1 test\r\n");
    EXPECT_EQ(0,1);
    EXPECT_GREATER(0,5);
}

CASE_SETUP(Test2)
{
    test_printf("Test2 setup\r\n");
}

CASE_TEARDOWN(Test2)
{
    test_printf("Test2 teardown\r\n");
}

CASE_FUNC(Test2)
{
    int res = 1;
    test_printf("test2 test\r\n");
    EXPECT_EQ(1,res);
    EXPECT_NE(0,res);
}


SUITE_SETUP()
{
    test_printf("test suite setup\r\n");
}

SUITE_TEARDOWN()
{
    test_printf("test suite teardown\r\n");
}


TEST_CASE_START
TEST_CASE(Test1)
TEST_CASE(Test2)
TEST_CASE_END
TEST_SUITE(TestSuite1)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus