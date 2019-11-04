#ifndef __TYPE_H__
#define __TYPE_H__

#define RET_FAILED (-1)
#define RET_OK     (0)

#ifdef _MSC_VER
#define COMPILER_IS_VC
#else
#define COMPILER_IS_GCC
#endif

typedef enum status_e {
    STATUS_E_FALSE,
    STATUS_E_TRUE,
    STATUS_E_MALLOC_FALSE
}status_e;

typedef enum BOOLTYPE {
    BOOL_FALSE,
    BOOL_TRUE
}BOOLTYPE;

typedef unsigned char u8int;
typedef char i8int;

typedef unsigned short u16int;
typedef short i16int;

typedef unsigned int u32int;
typedef int i32int;

typedef void * VOIDPTR;

#define CAL_STRUCT_OFFSET(type, member) ( (size_t)&(((type *)0)->member) )

#ifdef COMPILER_IS_GCC
#define CAL_STRUCT_HEAD_PTR(type, ptr, member) ({ \
    const typeof(((type *)0)->member) * _ptr = ptr; \
    (type *)((const char *)_ptr - CAL_STRUCT_OFFSET(type, member)); \
})
#else
#define CAL_STRUCT_HEAD_PTR(type, ptr, member) (type *)((char *)_ptr - CAL_STRUCT_OFFSET(type, member))
#endif

#define GET_STRUCT_HEAD_PTR(type, ptr, member) (ptr == NULL ? NULL : CAL_STRUCT_HEAD_PTR(type, ptr, member))

#define NEW(TYPE)  (TYPE##_new())
#define New(type, ...) (type##_new(__VA_ARGS__))
#define DELETE(TYPE, PTR) TYPE##_delete(PTR)

#endif