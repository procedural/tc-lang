#include "common.h"
#include "emitter.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *g_current_input = NULL;

#ifdef _WIN32
#define NULL_DEV "NUL"
#else
#define NULL_DEV "/dev/null"
#endif

static const char *find_cc(void) {
    static const char *compilers[] = {"gcc", "clang", "cc", "tcc", NULL};
    for (int i = 0; compilers[i]; i++) {
        char cmd[128];
        snprintf(cmd, sizeof(cmd), "%s --version >%s 2>%s", compilers[i], NULL_DEV, NULL_DEV);
        if (system(cmd) == 0) return compilers[i];
    }
    return NULL;
}

int main(int argc, char **argv) {
    const char *input = NULL;
    const char *output = NULL;
    const char *compile_out = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            puts("Tight-C compiler v1.0.0\n"
                 "\n"
                 "Usage: tcc <input.tc> [options]\n"
                 "\n"
                 "Options:\n"
                 "  -o, --output <file>    Write transpiled C to file (.h gets #pragma once)\n"
                 "  -c, --compile <name>   Compile to binary (auto-detects gcc/clang)\n"
                 "  -h, --help             Show this help message\n"
                 "  -v, --version          Show version\n"
                 "\n"
                 "Examples:\n"
                 "  tcc main.tc -o main.c           Transpile to C\n"
                 "  tcc main.tc -c app               Transpile + compile to binary\n"
                 "  tcc main.tc -o main.c -c app     Keep .c and compile\n"
                 "  tcc lib.tc -o lib.h               Emit as header\n");
            return 0;
        } else if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
            puts("tcc 1.0.0");
            return 0;
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile")) {
            if (++i >= argc) die("missing binary output path after -c");
            compile_out = argv[i];
        } else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
            if (++i >= argc) die("missing output path after -o");
            output = argv[i];
        } else {
            input = argv[i];
        }
    }
    if (!input) die("usage: tcc <input.tc> [-o output.c] [-c binary]\n       tcc --help for more info");
    char *source = read_file(input);
    tc_set_source(input, source);
    g_current_input = input;
    TokenVec tokens = lex_source(source);
    DeclVec program = parse_program(tokens.items);
    char *c_code = emit_program(program);

    if (compile_out) {
        // Write to temp .c file, compile with C compiler, then remove temp
        const char *tmp_c = output ? output : "__tcc_tmp.c";
        size_t tlen = strlen(tmp_c);
        bool tmp_is_header = (tlen > 2 && !strcmp(tmp_c + tlen - 2, ".h"));
        if (tmp_is_header) {
            Str wrapped = {0};
            str_add(&wrapped, "#pragma once\n");
            str_add(&wrapped, c_code);
            write_file(tmp_c, wrapped.data);
        } else {
            write_file(tmp_c, c_code);
        }

        const char *cc = find_cc();
        if (!cc) die("error: no C compiler found (tried gcc, clang, cc)");

        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "%s \"%s\" -std=c11 -lm -o \"%s\"", cc, tmp_c, compile_out);
        printf("  %s\n", cmd);
        int ret = system(cmd);

        if (!output) remove(tmp_c);
        if (ret != 0) die("error: C compilation failed (exit %d)", ret);
        printf("  compiled: %s\n", compile_out);
    } else if (output) {
        size_t olen = strlen(output);
        bool is_header = (olen > 2 && !strcmp(output + olen - 2, ".h"));
        if (is_header) {
            Str wrapped = {0};
            str_add(&wrapped, "#pragma once\n");
            str_add(&wrapped, c_code);
            write_file(output, wrapped.data);
        } else {
            write_file(output, c_code);
        }
    } else {
        fputs(c_code, stdout);
    }
    return 0;
}
