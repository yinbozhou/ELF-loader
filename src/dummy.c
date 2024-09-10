#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "elfload.h"
#include "elf.h" 
#include "lwip/sockets.h"
#include "netif/xadapter.h"
#include "lwipopts.h"
#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"

#define THREAD_STACKSIZE 1024

// 任务句柄
TaskHandle_t TaskHandle_ELFLoaderTask;

// 队列句柄
extern QueueHandle_t xQueue;
size_t elf_file_size = 0;
void *buf;
typedef void (*entrypoint_t)(int (*putsp)(const char*));

//read the ELF file at the specific address and check if it is correct form of the ELF
static bool fpread(el_ctx *ctx, void *dest, size_t nb, size_t offset) {
    (void) ctx;

    const unsigned char *elf_base_addr = (const unsigned char *)0x40000000;

    
    if (elf_file_size == 0) {
        
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_base_addr;

        if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
            ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
            ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
            ehdr->e_ident[EI_MAG3] != ELFMAG3) {
            xil_printf("%d\n",ehdr->e_ident[EI_MAG0]);
            xil_printf("%d\n",ehdr->e_ident[EI_MAG1]);
            xil_printf("%d\n",ehdr->e_ident[EI_MAG2]);
            xil_printf("%d\n",ehdr->e_ident[EI_MAG3]);
            
            xil_printf("Not an ELF file!\n");
            return false;
        }

        elf_file_size = ehdr->e_shoff + (ehdr->e_shnum * ehdr->e_shentsize);
        xil_printf("ELF file size calculated: %zu bytes\n", elf_file_size);
        xil_printf("ELF file size calculated: %lu bytes\n", (unsigned long)elf_file_size);

    }

    if (offset + nb > elf_file_size) {
        printf("Read beyond ELF file size!\n");
        return false;
    }

    memcpy(dest, elf_base_addr + offset, nb);
    return true;
}

static void *alloccb(el_ctx *ctx, Elf_Addr phys, Elf_Addr virt,Elf_Addr size)
{
    (void) ctx;
    (void) phys;
    (void) size;
    return (void*) virt;
}

static void check(el_status stat, const char* expln)
{
    if (stat) {
        xil_printf("%s: error %d\n", expln, stat);
        exit(1);
    }
}

int aligned_malloc(void** ptr, size_t alignment, size_t size) {
    if (ptr == NULL) return 1; // error if the pointer is null

    void* p = pvPortMalloc(size + alignment - 1 + sizeof(void*));
    if (p == NULL) {
        xil_printf("Error: Memory allocation failed in aligned_malloc\n");
        return 1;
    } 
    xil_printf("Allocated memory at base address: %p\n", p);
    // adjust the pointer and make sure the align
    size_t offset = (size_t)p % alignment;
    if (offset != 0) {
        offset = alignment - offset;
    }
    xil_printf("Alignment offset: %zu bytes\n", offset);
    void* aligned_ptr = (void*)((size_t)p + offset + sizeof(void*));
    xil_printf("Aligned memory address: %p\n", aligned_ptr);
    
    *((void**)((size_t)aligned_ptr - sizeof(void*))) = p;

    *ptr = aligned_ptr; // 通过引用更新传入的指针
    xil_printf("Memory address returned to user: %p\n", *ptr);
    return 0; 
}

void aligned_free(void* ptr) {
    if (ptr != NULL) {
        void* p = *((void**)((size_t)ptr - sizeof(void*)));
        xil_printf("Original base address to be freed: %p\n", p);
        vPortFree(p);
    }
}

static void go(entrypoint_t ep)
{
    ep(puts);
}


void ELFLoaderTask(void *p){
     int receivedmessage;
     xQueueReceive(xQueue,&receivedmessage,portMAX_DELAY);
     
     el_ctx ctx;
     ctx.pread=fpread;

     check(el_init(&ctx),"initialising");

     xil_printf("check init is executed successfully\n");
     
     if(aligned_malloc(&buf,ctx.align,ctx.memsz)){
        xil_printf("memory allocation failed\n");
        vTaskDelete(NULL);
        return;
    }
    xil_printf("aligned_malloc is executed successfully\n");
    ctx.base_load_vaddr = ctx.base_load_paddr = (uintptr_t) buf;
    
    check(el_load(&ctx, alloccb), "loading");

    xil_printf("check el_load is executed successfully\n");

    check(el_relocate(&ctx), "relocating");

    xil_printf("check el_relocate is executed successfully\n");

    uintptr_t epaddr = ctx.ehdr.e_entry + (uintptr_t) buf;

    entrypoint_t ep = (entrypoint_t) epaddr;

    xil_printf("Binary entrypoint is %" PRIxPTR "; invoking %p\n", (uintptr_t) ctx.ehdr.e_entry, ep);

    go(ep);
    aligned_free(buf);
    vTaskDelete(NULL);

}


