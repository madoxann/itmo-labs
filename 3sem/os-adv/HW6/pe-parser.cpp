#include <iostream>
#include <fstream>
#include <cstring>

#include "return_codes.h"
#include "pe-defines.h"

void print_next_string(std::ifstream& pe) {
    while (int ch = pe.get())
        std::cout << (char) ch;
    std::cout << std::endl;
}

bool is_pe(std::ifstream &pe) {
    uint32_t header;
    pe.seekg(0x3C);
    pe.read(reinterpret_cast<char *>(&header), 4);
    pe.seekg(header);
    pe.read(reinterpret_cast<char *>(&header), 4);

    return header == IMAGE_NT_SIGNATURE;
}

void print_functions(std::ifstream &pe, bool is_imported) {
    int flag = is_imported ? IMAGE_DIRECTORY_ENTRY_IMPORT : IMAGE_DIRECTORY_ENTRY_EXPORT;

    uint32_t nt_offset;
    pe.seekg(0x3C);
    pe.read(reinterpret_cast<char *>(&nt_offset), 4);
    pe.seekg(nt_offset);

    IMAGE_NT_HEADERS64 nt_headers64;
    pe.read(reinterpret_cast<char *>(&nt_headers64), sizeof(IMAGE_NT_HEADERS64));

    DWORD importRVA = nt_headers64.OptionalHeader.DataDirectory[flag].VirtualAddress;
    PIMAGE_SECTION_HEADER import_table = nullptr;
    IMAGE_SECTION_HEADER section_loc;

    pe.read(reinterpret_cast<char *>(&section_loc), sizeof(IMAGE_SECTION_HEADER));
    for (int i = 0; i < nt_headers64.FileHeader.NumberOfSections; i++, pe.read(reinterpret_cast<char *>(&section_loc),
                                                                               sizeof(IMAGE_SECTION_HEADER))) {
        if (section_loc.VirtualAddress <= importRVA &&
            importRVA < section_loc.VirtualAddress + section_loc.Misc.VirtualSize) {
            import_table = &section_loc;
            break;
        }
    }

    pe.seekg(import_table->PointerToRawData +
             nt_headers64.OptionalHeader.DataDirectory[flag].VirtualAddress -
             import_table->VirtualAddress);
    if (is_imported) {
        IMAGE_IMPORT_DESCRIPTOR imp_desc;
        pe.read(reinterpret_cast<char *> (&imp_desc), sizeof(IMAGE_IMPORT_DESCRIPTOR));

        for (; imp_desc.Name; pe.read(reinterpret_cast<char *> (&imp_desc), sizeof(IMAGE_IMPORT_DESCRIPTOR))) {
            uint32_t pos_rec = pe.tellg();
            pe.seekg(import_table->PointerToRawData + imp_desc.Name - import_table->VirtualAddress);
            print_next_string(pe);

            pe.seekg(import_table->PointerToRawData + imp_desc.DUMMYUNIONNAME.OriginalFirstThunk -
                     import_table->VirtualAddress);
            IMAGE_THUNK_DATA64 thunk_data64;
            for (pe.read(reinterpret_cast<char *> (&thunk_data64),
                         sizeof(IMAGE_THUNK_DATA64)); thunk_data64.u1.AddressOfData; pe.read(
                    reinterpret_cast<char *> (&thunk_data64), sizeof(IMAGE_THUNK_DATA64))) {
                uint32_t pos_thunk = pe.tellg();

                pe.seekg(import_table->PointerToRawData + thunk_data64.u1.AddressOfData - import_table->VirtualAddress +
                         2);
                std::cout << "    ";
                print_next_string(pe);

                pe.seekg(pos_thunk);
            }

            pe.seekg(pos_rec);
        }
    } else {
        IMAGE_EXPORT_DIRECTORY export_dir;
        pe.read(reinterpret_cast<char *> (&export_dir), sizeof(IMAGE_EXPORT_DIRECTORY));

        int32_t name;
        pe.seekg(import_table->PointerToRawData + export_dir.AddressOfNames - import_table->VirtualAddress);
        pe.read(reinterpret_cast<char *> (&name), sizeof(DWORD));
        uint32_t pos_rec = pe.tellg();

        for (int i = 0; i < export_dir.NumberOfNames; i++, pe.read(reinterpret_cast<char *> (&name), sizeof(DWORD)), pos_rec = pe.tellg()) {
            pe.seekg(import_table->PointerToRawData + name - import_table->VirtualAddress);
            print_next_string(pe);

            pe.seekg(pos_rec);
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cerr << "Insufficient args!" << std::endl;
        return ERROR_INVALID_PARAMETER;
    }

    std::ifstream pe(argv[2], std::ios::binary | std::ios::in);
    if (!pe) {
        std::cerr << "Failed to open file!" << std::endl;
        return ERROR_FILE_NOT_FOUND;
    }

    if (!strcmp(argv[1], "is-pe"))
        if (is_pe(pe)) {
            std::cout << "PE" << std::endl;
            return 0;
        } else {
            std::cout << "Not PE" << std::endl;
            return ERROR_NOT_FOUND;
        }
    else if (!strcmp(argv[1], "import-functions")) print_functions(pe, true);
    else if (!strcmp(argv[1], "export-functions")) print_functions(pe, false);
    else {
        std::cerr << "Invalid mode selected!" << std::endl;
        return ERROR_INVALID_PARAMETER;
    }

    return 0;
}
