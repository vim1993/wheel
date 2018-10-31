#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lxOs.h"
#include "lxMsgQue.h"
#include "lxQue.h"
#include "lxStack.h"
#include "type.h"
#include "lxlog.h"
#include "lxMemory.h"

void init(LOG_LEVEL_E level)
{
    Log_Set_PrintLevel(level);
}

int main(int argc, char *argv[])
{
    init(LOG_LEVEL_DEBUG);

    lx_memery_Obj * this = NEW(lx_memery_Obj);
    if(!this)
    {
        LOG_ERROR_PRINT("create lx_memery_Obj failed\n");
        return -1;
    }

    int * data = MALLOC(this, sizeof(int));
    int * data2 = MALLOC(this, sizeof(int));
    int * data3 = MALLOC(this, sizeof(int));
    FREE(this, data);
    int * data4 = MALLOC(this, sizeof(int));

    FREE(this, data2);
    FREE(this, data3);
    FREE(this, data4);
    DELETE(lx_memery_Obj, this);

    return 0;
}
