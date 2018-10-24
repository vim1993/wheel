#include <stdarg.h>
#include <stdio.h>

#include "type.h"
#include "lxlog.h"

static LOG_LEVEL_E gPrintLevel = LOG_LEVEL_DEBUG;

void Log_Set_PrintLevel(LOG_LEVEL_E level)
{
    gPrintLevel = level;

    return;
}

LOG_LEVEL_E Log_Get_PrintLevel(void)
{
    return gPrintLevel;
}

void Log_Print(LOG_LEVEL_E level, const char * format, ...)
{
    va_list ap;

    if(level < gPrintLevel)
    {
        return;
    }

    va_start(ap, format);

    vfprintf(stderr, format, ap);
    va_end(ap);

    return;
}
