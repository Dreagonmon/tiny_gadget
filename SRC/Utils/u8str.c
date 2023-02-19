#include "u8str.h"

U8Size u8_string_size(U8String text) {
    if (text == NULL) return 0;
    U8Size len = 0;
    while (text[len] != '\0') {
        len ++;
    }
    return len;
}

U8Size u8_string_group_size(U8StringGroup text_group) {
    if (text_group == NULL) return 0;
    U8StringGroup pointer = text_group;
    U8Size len = 0;
    while (*pointer != '\0') {
        while (*pointer != '\0') {
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return len;
}

U8Size u8_string_group_list_size(U8StringGroupList text_group_list) {
    if (text_group_list == NULL) return 0;
    U8StringGroupList pointer = text_group_list;
    U8Size len = 0;
    while (*pointer != '\0') {
        while (*pointer != '\0') {
            while (*pointer != '\0') {
                pointer ++;
            }
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return len;
}

U8String u8_string_group_get(U8StringGroup text_group, U8Size index) {
    if (text_group == NULL) return NULL;
    U8StringGroup pointer = text_group;
    U8Size len = 0;
    while (*pointer != '\0') {
        if (index == len) {
            return pointer;
        }
        while (*pointer != '\0') {
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return NULL;
}

U8StringGroup u8_string_group_list_get(U8StringGroupList text_group, U8Size index) {
    if (text_group == NULL) return NULL;
    U8StringGroupList pointer = text_group;
    U8Size len = 0;
    while (*pointer != '\0') {
        if (index == len) {
            return pointer;
        }
        while (*pointer != '\0') {
            while (*pointer != '\0') {
                pointer ++;
            }
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return NULL;
}
