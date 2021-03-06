/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
** 文   件   名: test_mpool.c
** 创   建   人: 周江村
** 最后修改日期: 2017/10/22 16:29:55
** 描        述: 
**  
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2017/10/22 16:29:55
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 单元测试用例的测试模板
** 本文件由C语言源文件模板软件生成。------------清风海岸出品，必属精品！------------
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "cut.h"
#include "wind_core_inc.h"
#include "wind_mpool.h"
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#define TNUM 5
#define TSIZE 13


WIND_MPOOL(test_pool,TNUM,TSIZE);
void *testblk[TNUM+1];


CASE_SETUP(pool_info)
{
    wind_pool_create("test_pool",test_pool,sizeof(test_pool),TSIZE);
}

CASE_TEARDOWN(pool_info)
{
    
}

CASE_FUNC(pool_info)
{
    pool_s *pool = (pool_s *)test_pool;
    EXPECT_EQ(pool->magic,WIND_MPOOL_MAGIC);
    EXPECT_STR_EQ(pool->name,"test_pool");
    EXPECT_EQ(pool->size,sizeof(test_pool)-sizeof(pool_s));
    EXPECT_EQ(pool->itemnum,TNUM);
    EXPECT_EQ(pool->itemsize,(((TSIZE+3)>>2)<<2)+sizeof(pool_item_s));
    EXPECT_EQ(pool->used,0);
}


CASE_SETUP(pool_alloc)
{
    int i;
    wind_pool_create("test_pool",test_pool,
                     sizeof(test_pool),TSIZE);
    for(i = 0;i < TNUM+1;i ++)
        testblk[i] = 0;
}


CASE_TEARDOWN(pool_alloc)
{
    int i;
    for(i = 0;i < TNUM+1;i ++)
        testblk[i] = 0;
}

CASE_FUNC(pool_alloc)
{
    int i;
    w_err_t err;
    pool_item_s* itm;
    pool_s *pool = (pool_s *)test_pool;
    for(i = 0;i < TNUM;i ++)
    {
        testblk[i] = wind_pool_alloc(test_pool);
        EXPECT_NE(testblk[i],NULL);
        itm = (pool_item_s*)((w_uint32_t)testblk[i] - sizeof(pool_item_s*));
        EXPECT_EQ(itm->flag,POOL_BLK_USED);
        EXPECT_EQ(itm->next,NULL);
        EXPECT_EQ(pool->used,i+1);
    }
    testblk[TNUM] = wind_pool_alloc(test_pool);
    EXPECT_EQ(testblk[TNUM],NULL);
    
    for(i = 0;i < TNUM;i ++)
    {
        wind_pool_free(test_pool,testblk[i]);
        itm = (pool_item_s*)((w_uint32_t)testblk[i] - sizeof(pool_item_s*));
        EXPECT_EQ(itm->flag,POOL_BLK_FREE);
        EXPECT_EQ(itm->next,NULL);
        EXPECT_EQ(pool->used,TNUM - i-1);
    }
    err = wind_pool_free(test_pool,testblk[TNUM]);
    EXPECT_EQ(err,ERR_NULL_POINTER);
    err = wind_pool_free(test_pool,(void*)0xffffffff);
    EXPECT_EQ(err,ERR_INVALID_PARAM);
    err = wind_pool_free(test_pool,testblk[0]);
    EXPECT_EQ(err,ERR_INVALID_PARAM);
}


SUITE_SETUP(test_mpool){}
SUITE_TEARDOWN(test_mpool){}



TEST_CASES_START(test_mpool)
TEST_CASE(pool_info)
TEST_CASE(pool_alloc)
TEST_CASES_END
TEST_SUITE(test_mpool)


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
