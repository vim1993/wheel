#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <openssl/des.h>
#include <openssl/md5.h>

#include "lxlog.h"

#define DES_KEY_BASE_LEN (8)

struct des_manager {
    unsigned int m_r;

    long long m_t;

    unsigned int m_cr;
    unsigned int m_isbig;

    unsigned char * out;
    unsigned int outlen;

    char * cstime;
    char * cetime;
};

#define BASE64_C_LEN (3)
#define BASE64_E_LEN (4)

#define TIME_STR_MAX_LEN (16)

static const unsigned char * Base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const int base64_dec[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

int cal_char2Base64(unsigned char * in, unsigned int inlen, unsigned char * out, unsigned int outlen)
{
    unsigned int i = 0, value = 0;
    unsigned int cnt = 0, re = 0;

    if(in == NULL || out == NULL || inlen <= 0 || outlen <= 0)
    {
        LOG_ERROR_PRINT("[%s] param error\n", __func__);
        return -1;
    }

    re = inlen%BASE64_C_LEN;
    cnt = inlen/BASE64_C_LEN + (re == 0 ? 0 : 1);

    if(cnt * BASE64_E_LEN > outlen)
    {
        LOG_ERROR_PRINT("more len: %d, %d\n", cnt, outlen);
        return 0;
    }

    memset(out, 0x0, outlen);
    for(i = 0; i < cnt; i++)
    {
        if(re != 0 && i == cnt - 1)
        {
            if(re == 1)
            {
                value = in[i * BASE64_C_LEN] << 16;
            }
            else
            {
                value = in[i * BASE64_C_LEN] << 16 | in[ (i * BASE64_C_LEN) + 1] << 8;
            }
        }
        else
        {
            value = in[i * BASE64_C_LEN] << 16 | in[ (i * BASE64_C_LEN) + 1] << 8 | in[ (i * BASE64_C_LEN) + 2] << 0;
        }

        out[i*BASE64_E_LEN] = Base64[(value >> 18) & 0x3F];
        out[(i*BASE64_E_LEN) + 1] = Base64[(value >> 12) & 0x3F];
        out[(i*BASE64_E_LEN) + 2] = ((((value >> 6) & 0x3F) == 0) & (i == cnt - 1)) ? '=' : Base64[(value >> 6) & 0x3F]; //如果是最后一组数�?,并且此字节为0,则代表补�?,因此使用'='替换
        out[(i*BASE64_E_LEN) + 3] = (((value & 0x3F) == 0) & (i == cnt - 1)) ? '=' : Base64[value & 0x3F];//如果是最后一组数�?, 并且此字节为0,则代表补�?,因此使用'='替换
    }

   return i * BASE64_E_LEN;
}

int cal_Base642Char(unsigned char * in, unsigned int inlen, unsigned char * out)
{
    if(in == NULL || inlen <= 0 || out == NULL)
    {
        return -1;
    }

    unsigned int cnt = 0, i = 0, value = 0;

    cnt = inlen/BASE64_E_LEN;

    for(i = 0; i < cnt; i++)
    {
        value = (base64_dec[in[i * BASE64_E_LEN ]] << 26 | base64_dec[in[(i * BASE64_E_LEN) + 1]] << 20 | base64_dec[in[(i * BASE64_E_LEN) + 2]] << 14 | base64_dec[in[(i * BASE64_E_LEN) + 3]] << 8) >> 8;

        out[i * BASE64_C_LEN] = value >> 16;
        out[(i * BASE64_C_LEN)+1] = value >> 8 & 0xFF;
        out[(i * BASE64_C_LEN)+2] = value & 0xFF;
    }

    return cnt * BASE64_C_LEN;
}

int isBig(void)
{
    union w {
        int paramA;
        char paramB;
    }check;

    check.paramA = 1;

    return check.paramB == 1 ? 0 : 1;
}

void cal_l2B(long long * pValue)
{
    long long tmpValue = 0;
    unsigned char * pTmp = (unsigned char *)&tmpValue;
    unsigned char * pRValue = (unsigned char *)pValue;

    unsigned int len = sizeof(long long);
    for(int i = 0; i < len; i++) {
        pTmp[i] = pRValue[len-i-1];
    }

    *pValue = tmpValue;
}

/*
*   param
*       csTime:描述时间字符串采用YMDTHMS格式, 20190619T092147
*   return
*       时间�?
*/
time_t cal_date2timestamp(const unsigned char * csTime)
{
    unsigned int offset = 0;
    unsigned char tmpBuf[5] = {0};
    struct tm t;

    memset(&t, 0x0, sizeof(t));
    memcpy(tmpBuf, csTime, 4);
    tmpBuf[4] = 0;
    offset += 4;
    t.tm_year = atoi(tmpBuf) - 1900;

    memcpy(tmpBuf, csTime+offset, 2);
    tmpBuf[2] = 0;
    offset += 2;
    t.tm_mon = atoi(tmpBuf) - 1;

    memcpy(tmpBuf, csTime+offset, 2);
    tmpBuf[2] = 0;
    offset += 3;
    t.tm_mday = atoi(tmpBuf);

    memcpy(tmpBuf, csTime+offset, 2);
    tmpBuf[2] = 0;
    offset += 2;
    t.tm_hour = atoi(tmpBuf);

    memcpy(tmpBuf, csTime+offset, 2);
    tmpBuf[2] = 0;
    offset += 2;
    t.tm_min = atoi(tmpBuf);

    memcpy(tmpBuf, csTime+offset, 2);
    tmpBuf[2] = 0;
    offset += 2;
    t.tm_sec = atoi(tmpBuf);

    return mktime(&t);
}

static int cal_ecb_decryption(unsigned char * calkeyBuffer, struct des_manager * pDes)
{
    if(calkeyBuffer == NULL || pDes == NULL)
    {
        LOG_ERROR_PRINT("[%s] param error\n", __func__);
        return -1;
    }

    int offset = 0, cnt = 0, delen = 0, i = 0;
    unsigned char key[9] = {0};
    unsigned char * pTmp = NULL;
    unsigned char * out = (unsigned char *)malloc(pDes->outlen - 24);
    if(out == NULL)
    {
        LOG_ERROR_PRINT("[%s] malloc out buffer error\n", __func__);
        return -1;
    }
    pTmp = out;
    pTmp++;

    unsigned char * pDeData = (unsigned char *)malloc(pDes->outlen - 24);
    if(pDeData == NULL)
    {
        LOG_ERROR_PRINT("[%s] malloc out buffer error\n", __func__);
        free(out);
        return -1;
    }
    memset(pDeData, 0x0 , pDes->outlen - 24);

    DES_key_schedule ks,ks2,ks3;

    offset = 0;
    memset(key, 0x0, sizeof(key));
    memcpy(key, calkeyBuffer + offset, DES_KEY_BASE_LEN);
    key[DES_KEY_BASE_LEN] = 0;
    DES_set_key_unchecked((const_DES_cblock*)key, &ks);
    offset += DES_KEY_BASE_LEN;

    memset(key, 0x0, sizeof(key));
    memcpy(key, calkeyBuffer + offset, DES_KEY_BASE_LEN);
    key[DES_KEY_BASE_LEN] = 0;
    DES_set_key_unchecked((const_DES_cblock*)key, &ks2);
    offset += DES_KEY_BASE_LEN;

    memset(key, 0x0, sizeof(key));
    memcpy(key, calkeyBuffer + offset, DES_KEY_BASE_LEN);
    key[DES_KEY_BASE_LEN] = 0;
    DES_set_key_unchecked((const_DES_cblock*)key, &ks3);
    cnt = cal_Base642Char(pDes->out + 24, pDes->outlen - 24, out);
    delen = out[0];
    cnt = delen/DES_KEY_BASE_LEN;
    for(i = 0; i < cnt; i++)
    {
        memset(key, 0x0, sizeof(key));
        memcpy(key, pTmp + i * DES_KEY_BASE_LEN, DES_KEY_BASE_LEN);
        key[DES_KEY_BASE_LEN] = 0;

        DES_ecb3_encrypt((const_DES_cblock*)key, (DES_cblock*)(pDeData + DES_KEY_BASE_LEN * i), &ks, &ks2, &ks3, DES_DECRYPT);
    }

    LOG_INFO_PRINT("deout:%s\n", pDeData);
    offset = 0;
    memcpy(pDes->cstime, pDeData, TIME_STR_MAX_LEN - 1);
    offset += TIME_STR_MAX_LEN - 1;
    memcpy(pDes->cetime, pDeData + offset, TIME_STR_MAX_LEN - 1);

    free(pDeData);
    free(out);

    return 0;
}


static int cal_ecb_encryption(unsigned char * calkeyBuffer, struct des_manager * pDes)
{
    if(calkeyBuffer == NULL || pDes == NULL)
    {
        LOG_ERROR_PRINT("[%s] param error\n", __func__);
        return -1;
    }

    FILE * fp = NULL;
    int offset = 0, cnt = 0, enclen = 0;
    unsigned char key[9] = {0};

    unsigned int totalenclen = strlen(pDes->cstime) + strlen(pDes->cetime);
    unsigned int cEncLen = DES_KEY_BASE_LEN - (totalenclen%DES_KEY_BASE_LEN);
    totalenclen += cEncLen;

    enclen = totalenclen * 2 + 30;
    char * pTmp = NULL;
    char * pBase64Data = NULL;
    char * pEnData = (char *) malloc(totalenclen + 1);
    if(pEnData == NULL)
    {
        LOG_ERROR_PRINT("malloc endata buffer failed\n");
        return -1;
    }
    offset += strlen(pDes->cstime);
    memcpy(pEnData, pDes->cstime, offset);
    memcpy(pEnData + offset, pDes->cetime, strlen(pDes->cetime));
    offset += strlen(pDes->cetime);
    memset(pEnData + offset, cEncLen + '0', cEncLen);

    pEnData[totalenclen] = 0;

    pDes->outlen = totalenclen;
    pDes->out = (unsigned char *) malloc(pDes->outlen + 2);
    if(pDes->out == NULL)
    {
        free(pEnData);
        LOG_ERROR_PRINT("malloc enc buffer failed\n");
        return -1;
    }

    memset(pDes->out, 0x0, pDes->outlen + 2);
    pTmp = pDes->out;
    pTmp[0] = totalenclen;
    pDes->out++;
    DES_key_schedule ks,ks2,ks3;

    offset = 0;
    memset(key, 0x0, sizeof(key));
    memcpy(key, calkeyBuffer + offset, DES_KEY_BASE_LEN);
    key[DES_KEY_BASE_LEN] = 0;
    DES_set_key_unchecked((const_DES_cblock*)key, &ks);
    offset += DES_KEY_BASE_LEN;

    memset(key, 0x0, sizeof(key));
    memcpy(key, calkeyBuffer + offset, DES_KEY_BASE_LEN);
    key[DES_KEY_BASE_LEN] = 0;
    DES_set_key_unchecked((const_DES_cblock*)key, &ks2);
    offset += DES_KEY_BASE_LEN;

    memset(key, 0x0, sizeof(key));
    memcpy(key, calkeyBuffer + offset, DES_KEY_BASE_LEN);
    key[DES_KEY_BASE_LEN] = 0;
    DES_set_key_unchecked((const_DES_cblock*)key, &ks3);

    cnt = totalenclen/DES_KEY_BASE_LEN;
    for(int i =0; i < cnt; i++)
    {
        memset(key, 0x0, sizeof(key));
        memcpy(key, pEnData + i * DES_KEY_BASE_LEN, DES_KEY_BASE_LEN);
        key[DES_KEY_BASE_LEN] = 0;
        DES_ecb3_encrypt((const_DES_cblock*)key, (DES_cblock*)(pDes->out + DES_KEY_BASE_LEN * i), &ks, &ks2, &ks3, DES_ENCRYPT);
    }

    LOG_INFO_PRINT("en:%s\n", pEnData);

    if(pEnData)
    {
        free(pEnData);
    }

    if(pDes->out)
    {
        do
        {
            memset(calkeyBuffer + 16, 0x0, 9);
            calkeyBuffer[16] = pDes->m_cr;
            pBase64Data = (char *) malloc(enclen);
            if(pBase64Data == NULL)
            {
                LOG_ERROR_PRINT("malloc pBase64Data buffer failed\n");
                break;
            }

            cnt = cal_char2Base64(calkeyBuffer, 17, pBase64Data, enclen);
            if(cnt <= 0)
            {
                break;
            }

            cnt += cal_char2Base64(pTmp, pDes->outlen + 1, pBase64Data + cnt, enclen - cnt);

            fp = fopen("licence.ini", "w+");
            if(fp != NULL)
            {
                fwrite(pBase64Data, cnt, 1, fp);
                fclose(fp);
            }
        }while(0);

        if(pBase64Data)
        {
            free(pBase64Data);
        }

        free(pTmp);
        pDes->out = NULL;
        pDes->outlen = 0;
    }

    return 0;
}

static int do_ecb_encryption(struct des_manager * pDes)
{
    if(pDes == NULL)
    {
        LOG_ERROR_PRINT("[%s] param error\n", __func__);
        return -1;
    }

    unsigned char keyBuffer[25] = {0};
    unsigned int i = 0, offset = 0;

    long long curgmt = time(NULL);
    pDes->m_isbig = isBig();

    if(!pDes->m_isbig)
    {
        cal_l2B(&curgmt);
    }

    pDes->m_t = curgmt;
    srand((unsigned int)time(NULL));
    pDes->m_r = rand() % 1000;
    snprintf(keyBuffer, sizeof(keyBuffer), "%x", pDes->m_r);
    pDes->m_cr = strlen(keyBuffer);

    memset(keyBuffer + pDes->m_cr, DES_KEY_BASE_LEN - pDes->m_cr + '0', DES_KEY_BASE_LEN - pDes->m_cr);
    offset += 8;
    memcpy(keyBuffer + DES_KEY_BASE_LEN, &pDes->m_t, DES_KEY_BASE_LEN);
    offset += 8;
    for(i = 0; i < DES_KEY_BASE_LEN; i++)
    {
        if(i < DES_KEY_BASE_LEN - pDes->m_cr)
        {
            keyBuffer[offset + i] = DES_KEY_BASE_LEN - pDes->m_cr + '0';
        }
        else
        {
            keyBuffer[offset + i] = pDes->m_cr + '0';
        }
    }

    return cal_ecb_encryption(keyBuffer, pDes);
}

static int do_ecb_decryption(struct des_manager * pDes)
{
    if(pDes == NULL)
    {
        LOG_ERROR_PRINT("[%s] param error\n", __func__);
        return -1;
    }

    unsigned int offset = 0, i = 0;
    unsigned char keybuffer[25] = {0};

    cal_Base642Char(pDes->out, 24, keybuffer);
    pDes->m_cr = keybuffer[16];
    memset(keybuffer + 16, 0x0, 9);
    offset += 16;
    for(i = 0; i < DES_KEY_BASE_LEN; i++)
    {
        if(i < DES_KEY_BASE_LEN - pDes->m_cr)
        {
            keybuffer[offset + i] = DES_KEY_BASE_LEN - pDes->m_cr + '0';
        }
        else
        {
            keybuffer[offset + i] = pDes->m_cr + '0';
        }
    }

    return cal_ecb_decryption(keybuffer, pDes);
}

int create_licence_config(char * cstime, char * endtime)
{
    int i32Ret = -1;
    struct des_manager desfiter;

    memset(&desfiter, 0x0, sizeof(desfiter));

    desfiter.cstime = (char *) malloc(TIME_STR_MAX_LEN);
    if(desfiter.cstime == NULL)
    {
        LOG_ERROR_PRINT("mac cstime buffer failed\n");
        return -1;
    }

    desfiter.cetime = (char *) malloc(TIME_STR_MAX_LEN);
    if(desfiter.cetime == NULL)
    {
        LOG_ERROR_PRINT("mac cetime buffer failed\n");
        free(desfiter.cstime);
        return -1;
    }

    snprintf(desfiter.cstime, TIME_STR_MAX_LEN, "%sT000000", cstime);
    snprintf(desfiter.cetime, TIME_STR_MAX_LEN, "%sT000000", endtime);

    LOG_INFO_PRINT("cstime:%s, cetime:%s\n", desfiter.cstime, desfiter.cetime);

    i32Ret = do_ecb_encryption(&desfiter);
    free(desfiter.cstime);
    free(desfiter.cetime);

    return i32Ret;
}

int check_licence_config(void)
{
    struct des_manager dfilter;
    unsigned int ret = 0;
    char * licbuf = (char *)malloc(1024);
    if(licbuf == NULL)
    {
        LOG_ERROR_PRINT("malloc databuf failed\n");
        return -1;
    }

    memset(licbuf, 0x0, 1024);

    FILE * fp = fopen("licence.ini", "r");
    if(fp == NULL)
    {
        LOG_ERROR_PRINT("open licence failed\n");
        return -1;
    }

    ret = fread((void *)licbuf, 1024, 1, fp);
    fclose(fp);
    if(ret < 0)
    {
        LOG_ERROR_PRINT("read licence failed\n");
        return -1;
    }

    dfilter.out = licbuf;
    dfilter.outlen = ret == 0 ? strlen(licbuf) : ret;

    dfilter.cstime = (char *) malloc(TIME_STR_MAX_LEN);
    if(dfilter.cstime == NULL)
    {
        LOG_ERROR_PRINT("mac cstime buffer failed\n");
        return -1;
    }

    dfilter.cetime = (char *) malloc(TIME_STR_MAX_LEN);
    if(dfilter.cetime == NULL)
    {
        LOG_ERROR_PRINT("mac cetime buffer failed\n");
        free(licbuf);
        free(dfilter.cstime);
        return -1;
    }

    if(do_ecb_decryption(&dfilter) != 0)
    {
        return 0;
    }

    time_t curt = time(NULL);
    if(curt >= cal_date2timestamp(dfilter.cstime) && curt <= cal_date2timestamp(dfilter.cetime))
    {
        ret = 1;
    }

    free(licbuf);
    free(dfilter.cstime);
    free(dfilter.cetime);

    LOG_INFO_PRINT("[%s] ret=%d\n", __func__, ret);

    return ret;
}

void utils_base64_unit_test(int argc, char * argv [ ])
{
    const char * value = "qazw";
    unsigned char out[1024] = {0};
    unsigned char outdec[1024] = {0};

    int retlen = cal_char2Base64((unsigned char *)value, 4, out, sizeof(out));
    LOG_INFO_PRINT("%d, enc:%s\n", retlen, out);

    retlen = cal_Base642Char(out, retlen, outdec);
    LOG_INFO_PRINT("%d, outdec:%s\n", retlen, outdec);

    return;
}

int utils_ecb_unit_test(int argc, char * argv[])
{
    if(argc == 3)
    {
        create_licence_config(argv[1], argv[2]);
    }
    else
    {
        create_licence_config("20190621", "20190622");
    }
    check_licence_config();

    return 0;
}