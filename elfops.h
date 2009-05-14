#ifndef MODINITTOOLS_MODULEOPS_H
#define MODINITTOOLS_MODULEOPS_H
#include <stdio.h>

/* All the icky stuff to do with manipulating 64 and 32-bit modules
   belongs here. */
struct kernel_symbol32 {
	char value[4];
	char name[64 - 4];
};

struct kernel_symbol64 {
	char value[8];
	char name[64 - 8];
};

struct elf_file
{
	char *pathname;

	/* File operations */
	struct module_ops *ops;

	/* Convert endian? */
	int conv;

	/* File contents and length. */
	void *data;
	unsigned long len;
};

/* Tables extracted from module by ops->fetch_tables(). */
struct module_tables
{
	unsigned int pci_size;
	void *pci_table;
	unsigned int usb_size;
	void *usb_table;
	unsigned int ieee1394_size;
	void *ieee1394_table;
	unsigned int ccw_size;
	void *ccw_table;
	unsigned int pnp_size;
	void *pnp_table;
	unsigned int pnp_card_size;
	unsigned int pnp_card_offset;
	void *pnp_card_table;
	unsigned int input_size;
	void *input_table;
	unsigned int input_table_size;
	unsigned int serio_size;
	void *serio_table;
	unsigned int of_size;
	void *of_table;
};

struct module_ops
{
	void *(*load_section)(struct elf_file *module,
		const char *secname, unsigned long *secsize);
	struct string_table *(*load_symbols)(struct elf_file *module);
	struct string_table *(*load_dep_syms)(const char *pathname,
		struct elf_file *module, struct string_table **types);
	void (*fetch_tables)(struct elf_file *module,
		struct module_tables *tables);
	char *(*get_aliases)(struct elf_file *module, unsigned long *size);
	char *(*get_modinfo)(struct elf_file *module, unsigned long *size);
	void (*strip_section)(struct elf_file *module, const char *secname);
};

extern struct module_ops mod_ops32, mod_ops64;

int elf_ident(void *file, unsigned long fsize, int *conv);

struct elf_file *grab_elf_file(const char *pathname);
struct elf_file *grab_elf_file_fd(const char *pathname, int fd);
void release_elf_file(struct elf_file *file);

#endif /* MODINITTOOLS_MODULEOPS_H */
