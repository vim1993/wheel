#ifndef __LX_LOG_H__
#define __LX_LOG_H__

typedef enum LOG_LEVEL_E {
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
}LOG_LEVEL_E;

void Log_Set_PrintLevel(LOG_LEVEL_E level);
LOG_LEVEL_E Log_Get_PrintLevel(void);
void Log_Print(LOG_LEVEL_E level, const char * format, ...);

#define LOG_ERROR_PRINT(format, printlist...) \
        Log_Print(LOG_LEVEL_ERROR, "[%s][%s][%d]:", __FILE__, __func__, __LINE__); \
        Log_Print(LOG_LEVEL_ERROR, format, ##printlist);

#define LOG_DEBUG_PRINT(format, printlist...) \
        Log_Print(LOG_LEVEL_DEBUG, "[LOG_LEVEL_DEBUG]"format, ##printlist);

#define LOG_WARN_PRINT(format, printlist...) \
        Log_Print(LOG_LEVEL_WARN, "[LOG_LEVEL_WARN]"format, ##printlist);

#define LOG_INFO_PRINT(format, printlist...) \
        Log_Print(LOG_LEVEL_INFO, "[LOG_LEVEL_INFO]"format, ##printlist);

#endif