# About the project
This is the magic implementation of dynamic string. Based on [dalloc](https://github.com/SkyEng1neering/dalloc) allocator, that solves memory fragmentation problem. So you can use it in your embedded project and not to be afraid of program crash by reason of memory fragmentation.

You can use __ustring__ like string from STL, it supports all base methods of std::string.

# Dependencies
__ustring__ based on [uvector](https://github.com/SkyEng1neering/uvector) and [dalloc](https://github.com/SkyEng1neering/dalloc) allocator, so you should include it to your project.

# Usage
## Using ustring with single heap area
[dalloc](https://github.com/SkyEng1neering/dalloc) allocator is configurable, it can work with only one memory area that you define, or you can select which memory area should be used for each your allocation.

If you want to use in your project only one heap area, you should define "USE_SINGLE_HEAP_MEMORY" in file __dalloc_conf.h__. This is the simpliest way to use __ustring__ because it allows you to abstract away from working with memory.

```c++
/* File dalloc_conf.h */
#define USE_SINGLE_HEAP_MEMORY
#define SINGLE_HEAP_SIZE				4096UL //define heap size that you want to have
```

Then you should define uint8_t array in your code, that will be used for storing data. This array should be named "single_heap" and it should be have size SINGLE_HEAP_SIZE (defined in file __dalloc_conf.h__).

```c++
#include "ustring.h"

uint8_t single_heap[SINGLE_HEAP_SIZE] = {0};
```

Why you should implement this array in your code by yourself? Because you may want to store this array for example in specific memory region, or you may want to apply to this array some attributes, like this:

```c++
__attribute__((section(".ITCM_RAM"))) uint8_t single_heap[SINGLE_HEAP_SIZE] = {0};
```
or like this:

```c++
__ALIGN_BEGIN uint8_t single_heap[SINGLE_HEAP_SIZE] __ALIGN_END;
```

So for example that's how looks like example of using __ustring__ with single memory region on STM32 MCU:

```c++
#include "ustring.h"

__ALIGN_BEGIN uint8_t single_heap[SINGLE_HEAP_SIZE] __ALIGN_END;

void main(){
  ustring string1("Hello ");
  ustring string2("World!11");
  
  ustring string3 = string1 + string2;

  printf("String value is %s\n", string3.c_str());
 
  while(1){}
}  
```

By the way you can use __ustring__ also with [uvector](https://github.com/SkyEng1neering/uvector):

```c++
#include "uvector.h"
#include "ustring.h"

__ALIGN_BEGIN uint8_t single_heap[SINGLE_HEAP_SIZE] __ALIGN_END;

void main(){
  ustring string1("this is the string 1");
  ustring string2("this is the string 2");

  uvector<ustring> vector_of_strings;
  
  vector_of_strings.push_back(string1);
  vector_of_strings.push_back(string2);
 
  for(uint32_t i = 0; i < vector_of_strings.size(); i++){
    printf("Vector element %lu: %s\n", i, vector_of_strings.at(i).c_str());
  }
 
  while(1){}
}  
```
Full info about supported methods of __ustring__ you can see in ___ustring.h___ file

## Using ustring with different heap areas

If you want to use several different heap areas, you can define it explicitly:

```c++
/* File dalloc_conf.h */
//#define USE_SINGLE_HEAP_MEMORY //comment this define
```

```c++
#include "ustring.h"

#define HEAP_SIZE			1024

/* Declare an arrays that will be used for dynamic memory allocations */
__ALIGN_BEGIN uint8_t heap_array1[HEAP_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t heap_array2[HEAP_SIZE] __ALIGN_END;

/* Declare an dalloc heap structures, it will contains all allocations info */
heap_t heap1;
heap_t heap2;

void main(){
  /* Init heap memory */
  heap_init(&heap1, (void*)heap_array1, HEAP_SIZE);
  heap_init(&heap2, (void*)heap_array2, HEAP_SIZE);

  ustring string1("String1", &heap1);
  ustring string2("String2", &heap2);
  
  printf("String1 value is %s\n", string1.c_str());
  printf("String2 value is %s\n", string2.c_str());
 
  while(1){}
}  
  
```
## P.S.
In any time you can check what exactly is going on in your heap memory using functions:
```c++
/* If you use different heap areas in your project */
void print_dalloc_info(heap_t *heap_struct_ptr);
void dump_dalloc_ptr_info(heap_t* heap_struct_ptr);
void dump_heap(heap_t* heap_struct_ptr);
```
```c++
/* If you use single heap area in your project */
void print_def_dalloc_info();
void dump_def_dalloc_ptr_info();
void dump_def_heap();
```
