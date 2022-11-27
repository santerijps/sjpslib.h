#ifndef SJPSLIB_LIST_H

  #define SJPSLIB_LIST_H

  /*
  Define a new dynamic array type.
  */
  #define arraylistdef(name, type)\
    \
    typedef struct name\
    {\
      size_t cap, len;\
      type *items;\
    } name;\
    \
    name name##_create(size_t capacity)\
    {\
      name l;\
      l.cap = capacity;\
      l.len = 0;\
      l.items = (type*) malloc(sizeof(type) * l.cap);\
      assert (l.items != NULL);\
      return l;\
    }\
    \
    void name##_init(name *l, size_t capacity)\
    {\
      l->cap = capacity;\
      l->len = 0;\
      l->items = (type*) malloc(sizeof(type) * l->cap);\
      assert (l->items != NULL);\
    }\
    \
    void name##_free(name *l)\
    {\
      free(l->items);\
      l->len = 0;\
    }\
    \
    void name##_append(name *l, type x)\
    {\
      l->items[l->len++] = x;\
      if (l->len == l->cap)\
      {\
        l->cap *= 2;\
        l->items = (type*) realloc(l->items, l->cap);\
        assert (l->items != NULL);\
      }\
    }\
    \
    void name##_prepend(name *l, type x)\
    {\
      assert (l->len + 1 <= l->cap);\
      for (size_t i = 0, j; i < l->len; i++)\
      {\
        j = l->len - 1 - i;\
        l->items[j + 1] = l->items[j];\
      }\
      l->items[0] = x;\
      l->len += 1;\
      if (l->len == l->cap)\
      {\
        l->cap *= 2;\
        l->items = (type*) realloc(l->items, l->cap);\
        assert (l->items != NULL);\
      }\
    }\
    \
    type name##_get(name *l, size_t index)\
    {\
      assert (index < l->len);\
      return l->items[index];\
    }\
    \
    void name##_set(name *l, size_t index, type x)\
    {\
      assert (index < l->len);\
      l->items[index] = x;\
    }\
    \
    type name##_pop(name *l)\
    {\
      assert (l->len > 0);\
      l->len -= 1;\
      return l->items[l->len];\
    }\
    \
    type name##_shift(name *l)\
    {\
      assert (l->len > 0);\
      type x = l->items[0];\
      for (size_t i = 0; i < l->len; i++)\
      {\
        l->items[i] = l->items[i + 1];\
      }\
      l->len -= 1;\
      return x;\
    }\
    \
    void name##_clear(name *l)\
    {\
      l->len = 0;\
    }\
    \
    void name##_insert(name *l, size_t index, type x)\
    {\
      assert (l->len + 1 <= l->cap);\
      if (index > l->len) index = l->len;\
      for (size_t i = index, j; i < l->len; i++)\
      {\
        j = l->len - 1 - i;\
        l->items[j + 1] = l->items[j];\
      }\
      l->len += 1;\
      l->items[index] = x;\
      if (l->len == l->cap)\
      {\
        l->cap *= 2;\
        l->items = (type*) realloc(l->items, l->cap);\
        assert (l->items != NULL);\
      }\
    }\
    \
    type name##_remove(name *l, size_t index)\
    {\
      assert (index < l->len);\
      type x = l->items[index];\
      for (size_t i = index; i < l->len; i++)\
      {\
        l->items[i] = l->items[i + 1];\
      }\
      l->len -= 1;\
      return x;\
    }\
    \
    void name##_reverse(name *l)\
    {\
      type x, y;\
      for (size_t i = 0, j; i < l->len / 2; i++)\
      {\
        j = l->len - 1 - i;\
        x = l->items[i];\
        y = l->items[j];\
        l->items[i] = y;\
        l->items[j] = x;\
      }\
    }\
    \
    name name##_copy(name *l)\
    {\
      name x = name##_create(l->cap);\
      x.len = l->len;\
      for (size_t i = 0; i < l->len; i++)\
      {\
        x.items[i] = l->items[i];\
      }\
      return x;\
    }\
    \

#endif