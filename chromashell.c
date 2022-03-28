#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include "chromashell.h"

int main(int argc, char **argv)
{
    const char *basename = (const char *) argv[0];
    bool arg_parsed = false;
    struct segment *segments = NULL;
    unsigned int n_segments = 0;

    for (int arg_index = 1; arg_index < argc; ++arg_index)
    {
        const char *arg = argv[arg_index];
        if (strcmp(arg, "-s") == 0 || strcmp(arg, "--segment") == 0)
        {
            ++arg_index;
            if (arg_index < argc)
            {
                char **optargs = NULL;
                int n_optargs = split_optargs(&optargs, argv[arg_index], ",", 2);
                if (n_optargs >= 2)
                {
                    char *s_color = optargs[0];
                    char *s_height = optargs[1];
                    if (strlen(s_color) == 6 && is_hex(s_color) && is_uint(s_height))
                    {
                        struct segment seg;
                        char s_red[3], s_green[3], s_blue[3];
                        sprintf(s_red, "%c%c", s_color[0], s_color[1]);
                        sprintf(s_blue, "%c%c", s_color[2], s_color[3]);
                        sprintf(s_green, "%c%c", s_color[4], s_color[5]);
                        seg.red = (unsigned int) strtol(s_red, NULL, 16);
                        seg.green = (unsigned int) strtol(s_green, NULL, 16);
                        seg.blue = (unsigned int) strtol(s_blue, NULL, 16);
                        seg.height = (unsigned int) atoi(s_height);

                        segments = realloc(segments, sizeof(struct segment) * ++n_segments);
                        if (segments != NULL)
                        {
                            memcpy(&segments[n_segments - 1], &seg, sizeof(struct segment));
                            free(optargs);
                            arg_parsed = true;
                        }
                        else
                        {
                            return err_alloc();
                        }
                    }
                    else
                    {
                        display_help(basename);
                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    free(optargs);
                    return err_no_arg(basename, arg);
                }
            }
            else
            {
                return err_no_arg(basename, arg);
            }
        }
        else if (strcmp(arg, "--examples") == 0)
        {
            display_examples(basename);
            return EXIT_SUCCESS;
        }
        else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
        {
            display_help(basename);
            return EXIT_SUCCESS;
        }
        else if (strcmp(arg, "--version") == 0)
        {
            display_version();
            return EXIT_SUCCESS;
        }
        else
        {
            return err_no_opt(basename, arg);
        }
    }

    if (!arg_parsed)
    {
        display_help(basename);
        return EXIT_FAILURE;
    }
    else if (n_segments > 0)
    {
        struct winsize winsz;
        ioctl(0, TIOCGWINSZ, &winsz);
        for (int i = 0; i < n_segments; ++i)
            print_segment(&winsz, &segments[i]);
    }

    return EXIT_SUCCESS;
}

int split_optargs(char ***dest, char *optarg, const char *delim, unsigned int max_args)
{
    char **buf = *dest;
    char *tok = strtok(optarg, delim);
    int n_args = 0;
    while (tok != NULL && n_args < max_args)
    {
        buf = realloc(buf, sizeof(char *) * ++n_args);
        if (buf != NULL)
        {
            buf[n_args - 1] = tok;
            tok = strtok(NULL, delim);
        }
        else
        {
            exit(err_alloc());
        }
    }
    
    *dest = buf;
    return n_args;
}

bool is_hex(const char *s)
{
    bool ret = true;
    char *digits = "0123456789abcdef";
    for (int i = 0; i < strlen(s); ++i)
        if (strchr(digits, tolower(s[i])) == NULL)
            ret = false;

    return ret;
}

bool is_uint(const char *s)
{
    bool ret = true;
    char *digits = "0123456789";
    for (int i = 0; i < strlen(s); ++i)
        if (strchr(digits, s[i]) == NULL)
            ret = false;

    return ret;
}

void print_segment(struct winsize *winsz, struct segment *seg)
{
    printf("\033[38;2;%i;%i;%im", seg->red, seg->blue, seg->green);
    for (int line = 0; line < seg->height; ++line)
        for (int col = 0; col < winsz->ws_col; ++col)
            printf("\u2588");

    printf("\n\033[0m");
}

void display_examples(const char *basename)
{
    printf(
        "Transgender flag:\n"
        " %s -s 7ACBF5,2 -s EAACB8,2 -s FFFFFF,2 -s EAACB8,2 -s 7ACBF5,2\n",
        basename);
}

void display_help(const char *basename)
{
    printf(
        "Usage: %s OPTIONS ...\n"
        "Display lines of color in a true color terminal.\n"
        " -s,   --segment RRGGBB,HEIGHT         specify segment hex color and height in lines\n"
        "       --examples                      display example configuration and exit\n"
        " -h,   --help                          display this help and exit\n"
        "       --version                       display version information and exit\n",
        basename);
}

void display_version(void)
{
    printf(
        "ChromaShell %s\n"
        "Copyright (C) %s Adora Zero.\n"
        "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n\n"
        "Written by Adora.\n",
        VERSION_STRING, VERSION_YEAR);
}

int err_alloc(void)
{
    printf("Error (re)allocating memory\n");

    return EXIT_FAILURE;
}

int err_no_arg(const char *basename, const char *opt)
{
    printf(
        "%s: missing required argument for '%s'\n"
        "Try '%s --help' for more information.\n",
        basename, opt, basename);

    return EXIT_FAILURE;
}

int err_no_opt(const char *basename, const char *opt)
{
    printf(
        "%s: unrecognized option '%s'\n"
        "Try '%s --help' for more information.\n",
        basename, opt, basename);

    return EXIT_FAILURE;
}
