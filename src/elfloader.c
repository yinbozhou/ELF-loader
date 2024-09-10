/* Copyright © 2014, Owen Shepherd
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted without restriction.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "elfload.h"
#include "elf.h" 

FILE *f;
void *buf;
size_t elf_file_size = 0;

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
            xil_printf("Not an ELF file!\n");
            return false;
        }

        elf_file_size = ehdr->e_shoff + (ehdr->e_shnum * ehdr->e_shentsize);
        xil_printf("ELF file size calculated: %zu bytes\n", elf_file_size);
    }

    if (offset + nb > elf_file_size) {
        printf("Read beyond ELF file size!\n");
        return false;
    }

    memcpy(dest, elf_base_addr + offset, nb);
    return true;
}

static void *alloccb(
    el_ctx *ctx,
    Elf_Addr phys,
    Elf_Addr virt,
    Elf_Addr size)
{
    (void) ctx;
    (void) phys;
    (void) size;
    return (void*) virt;
}

static void check(el_status stat, const char* expln)
{
    if (stat) {
        fprintf(stderr, "%s: error %d\n", expln, stat);
        exit(1);
    }
}

static void go(entrypoint_t ep)
{
    ep(puts);
}
//argc = argument count 
//argv = argument vector 


int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s [elf-to-load]\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "rb");
    if (!f) {
        perror("opening file");
        return 1;
    }

    el_ctx ctx;
    // the function implementation of the pread in the structer ctx would be fpread, which defined above
    ctx.pread = fpread;

    check(el_init(&ctx), "initialising");

    if (posix_memalign(&buf, ctx.align, ctx.memsz)) {
        perror("memalign");
        return 1;
    }

    if (mprotect(buf, ctx.memsz, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("mprotect");
        return 1;
    }

    ctx.base_load_vaddr = ctx.base_load_paddr = (uintptr_t) buf;

    check(el_load(&ctx, alloccb), "loading");
    check(el_relocate(&ctx), "relocating");

    uintptr_t epaddr = ctx.ehdr.e_entry + (uintptr_t) buf;

    entrypoint_t ep = (entrypoint_t) epaddr;

    printf("Binary entrypoint is %" PRIxPTR "; invoking %p\n", (uintptr_t) ctx.ehdr.e_entry, ep);

    //go(ep);

    fclose(f);

    free(buf);

    return 0;
}
