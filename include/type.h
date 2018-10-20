#ifndef __TYPE_H__
#define __TYPE_H__

typedef enum status_e {
    STATUS_E_FALSE,
    STATUS_E_TRUE
}status_e;

#define CAL_STRUCT_OFFSET(type, member) ( (size_t)&(((type *)0)->member) )

#define CAL_STRUCT_HEAD_PTR(type, ptr, member) ({ \
    const typeof(((type *)0)->member) * _ptr = ptr; \
    (type *)((char *)_ptr - CAL_STRUCT_OFFSET(type, member)); \
})

#define GET_STRUCT_HEAD_PTR(type, ptr, member) (ptr == NULL ? NULL : CAL_STRUCT_HEAD_PTR(type, ptr, member))

#define NEW(TYPE)  (TYPE##_new())
#define DELETE(TYPE, PTR) TYPE##_delete(PTR)

#endif