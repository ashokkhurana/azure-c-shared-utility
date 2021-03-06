// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#ifdef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <string.h>
#include "umockvalue.h"

typedef struct UMOCK_VALUE_TYPE_HANDLERS_TAG
{
    char* type;
    UMOCKVALUE_STRINGIFY_FUNC stringify;
    UMOCKVALUE_COPY_FUNC value_copy;
    UMOCKVALUE_FREE_FUNC value_free;
    UMOCKVALUE_ARE_EQUAL_FUNC are_equal;
} UMOCK_VALUE_TYPE_HANDLERS;

static UMOCK_VALUE_TYPE_HANDLERS* type_handlers = NULL;
static size_t type_handler_count = 0;

static UMOCK_VALUE_TYPE_HANDLERS* get_value_type_handlers(const char* type)
{
    UMOCK_VALUE_TYPE_HANDLERS* result;
    size_t i;

    for (i = 0; i < type_handler_count; i++)
    {
        if (strcmp(type_handlers[i].type, type) == 0)
        {
            break;
        }
    }

    if (i < type_handler_count)
    {
        result = &type_handlers[i];
    }
    else
    {
        result = NULL;
    }

    return result;
}

char* umockvalue_stringify_int(const int* value)
{
    char* result;

    if (value == NULL)
    {
        result = NULL;
    }
    else
    {
        // adjust this to account for accurate amount of chars
        char temp_buffer[32];
        int length = sprintf(temp_buffer, "%d", *value);
        if (length < 0)
        {
            result = NULL;
        }
        else
        {
            result = (char*)malloc(length + 1);
            if (result != NULL)
            {
                memcpy(result, temp_buffer, length + 1);
            }
        }
    }

    return result;
}

int umockvalue_copy_int(int* destination, const int* source)
{
    int result;

    if ((destination == NULL) ||
        (source == NULL))
    {
        result = __LINE__;
    }
    else
    {
        *destination = *source;
        result = 0;
    }

    return result;
}

void umockvalue_free_int(int* value)
{
    /* no free required for int */
}

int umockvalue_are_equal_int(const int* left, const int* right)
{
    int result;

    if (left == right)
    {
        result = 1;
    }
    else if ((left == NULL) || (right == NULL))
    {
        result = 0;
    }
    else
    {
        result = ((*left) == (*right)) ? 1 : 0;
    }

    return result;
}

int umockvalue_init(void)
{
    umockvalue_register_type("int", (UMOCKVALUE_STRINGIFY_FUNC)umockvalue_stringify_int, (UMOCKVALUE_ARE_EQUAL_FUNC)umockvalue_are_equal_int, (UMOCKVALUE_COPY_FUNC)umockvalue_copy_int, (UMOCKVALUE_FREE_FUNC)umockvalue_free_int);
    return 0;
}

void umockvalue_deinit(void)
{

}

int umockvalue_register_type(const char* type, UMOCKVALUE_STRINGIFY_FUNC stringify, UMOCKVALUE_ARE_EQUAL_FUNC are_equal, UMOCKVALUE_COPY_FUNC value_copy, UMOCKVALUE_FREE_FUNC value_free)
{
    int result;

    UMOCK_VALUE_TYPE_HANDLERS* new_type_handlers = (UMOCK_VALUE_TYPE_HANDLERS*)realloc(type_handlers, sizeof(UMOCK_VALUE_TYPE_HANDLERS) * (type_handler_count + 1));
    if (new_type_handlers == NULL)
    {
        result = __LINE__;
    }
    else
    {
        size_t type_string_length = strlen(type);
        type_handlers = new_type_handlers;
        type_handlers[type_handler_count].type = (char*)malloc(type_string_length + 1);
        if (type_handlers[type_handler_count].type == NULL)
        {
            result = __LINE__;
        }
        else
        {
            (void)memcpy(type_handlers[type_handler_count].type, type, type_string_length + 1);
            type_handlers[type_handler_count].stringify = stringify;
            type_handlers[type_handler_count].value_copy = value_copy;
            type_handlers[type_handler_count].value_free = value_free;
            type_handlers[type_handler_count].are_equal = are_equal;
            type_handler_count++;

            result = 0;
        }
    }

    return result;
}

char* umockvalue_stringify(const char* type, const void* value)
{
    char* result;
    UMOCK_VALUE_TYPE_HANDLERS* value_type_handlers = get_value_type_handlers(type);

    if (value_type_handlers == NULL)
    {
        result = NULL;
    }
    else
    {
        result = value_type_handlers->stringify(value);
    }

    return result;
}

int umockvalue_are_equal(const char* type, const void* left, const void* right)
{
    int result;
    UMOCK_VALUE_TYPE_HANDLERS* value_type_handlers = get_value_type_handlers(type);

    if (value_type_handlers == NULL)
    {
        result = 0;
    }
    else
    {
        result = value_type_handlers->are_equal(left, right);
    }

    return result;
}

int umockvalue_copy(const char* type, void* destination, const void* source)
{
    int result;
    UMOCK_VALUE_TYPE_HANDLERS* value_type_handlers = get_value_type_handlers(type);
    if (value_type_handlers == NULL)
    {
        result = __LINE__;
    }
    else
    {
        result = value_type_handlers->value_copy(destination, source);
    }
    return result;
}

void umockvalue_free(const char* type, void* value)
{
    UMOCK_VALUE_TYPE_HANDLERS* value_type_handlers = get_value_type_handlers(type);
    if (value_type_handlers != NULL)
    {
        value_type_handlers->value_free(value);
    }
}
