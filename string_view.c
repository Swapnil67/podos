// * Stolen From https://github.com/tsoding

#ifndef SV_H_
#define SV_H_

#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

typedef struct {
    size_t count;
    const char *data;
} String_View;
#define SV_PRINT(sv) fprintf(stdout, "%.*s\n", (int) sv.count, sv.data) 

String_View cstr_to_sv(const char *str);
String_View sv_chop_by_delim(String_View *sv, char delim);
String_View sv_trim_left(String_View sv);
String_View sv_trim_right(String_View sv);
String_View sv_trim(String_View sv);
int sv_eq(String_View a, String_View b);

void save_sv_to_file(String_View sv, const char *file_path);
String_View read_from_file_to_sv(const char* file_path);

#endif // * SV_H

#ifdef SV_IMPLEMENTATION

String_View cstr_to_sv(const char *str) {
    return (String_View) {
	.count = strlen(str),
	.data = (char *)str,
    };
}

String_View sv_chop_by_delim(String_View *sv, char delim) {
    size_t i = 0;
    while(i < sv->count && sv->data[i] != delim) {
	i += 1;
    }

    String_View result = {
	.count = i,
	.data = sv->data,
    };

    if(i < sv->count) {
	sv->data += (i + 1);
	sv->count -= (i + 1);
    }
    else {
	sv->data += i;
	sv->count -= i;
    }
    
    return result;
}

String_View sv_trim_left(String_View sv) {
    size_t i = 0;
    while(i < sv.count && isspace(sv.data[i])) {
	i += 1;
    }

    return (String_View) {
	.count = sv.count - i,
	.data = sv.data + i
    };
}

String_View sv_trim_right(String_View sv) {
    size_t i = 0;
    while(i < sv.count && isspace(sv.data[sv.count - 1 - i])) {
	i++;
    }

    return (String_View) {
	.count = sv.count - i,
	.data = sv.data
    };
}

String_View sv_trim(String_View sv) {
    return sv_trim_right(sv_trim_left(sv));
}

int sv_eq(String_View a, String_View b) {
    if (a.count != b.count) {	
	return 0;	
    }    
    else {	
	return memcmp(a.data, b.data, a.count) == 0;	
    }    
}

void save_sv_to_file(String_View sv, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    // printf("Ptr: %p\n", f);
    if(f == NULL) {
	fprintf(stderr, "ERROR: could not open file `%s`: %s\n", file_path, strerror(errno));
	exit(1);
    }

    fwrite(sv.data, sv.count, 1, f);
    if(ferror(f)) {
	fprintf(stderr, "ERROR: unable to write to file `%s`: %s\n", file_path, strerror(errno));
	exit(1);
    }

    fclose(f);
}

String_View read_from_file_to_sv(const char* file_path) {
    FILE *f = fopen(file_path, "r");
    if(f == NULL) {
	fprintf(stderr, "ERROR: could not open file `%s`: %s\n", file_path, strerror(errno));
	exit(1);
    }

    if(fseek(f, 0, SEEK_END) < 0) {
	fprintf(stderr, "ERROR: could not read file `%s`: %s\n", file_path, strerror(errno));
	exit(1);	
    }

    long m = ftell(f);
    if(m < 0) {
	fprintf(stderr, "ERROR: could not read file `%s`: %s\n", file_path, strerror(errno));
	exit(1);	
    }    
    // printf("The current location of file pointer is %d bytes from the start of the file\n", m);
    
    // * Set buffer to start of file
    if(fseek(f, 0, SEEK_SET) < 0) {
	fprintf(stderr, "ERROR: could not read file `%s`: %s\n", file_path, strerror(errno));
	exit(1);	
    }

    // * Allocate some memory
    char *buffer = malloc((size_t) m);
    if(buffer == NULL) {
        fprintf(stderr, "ERROR: could not allocate memory for file: %s\n", strerror(errno));
        exit(1);
    }    
    
    size_t n = fread(buffer, 1, (size_t) m, f);
    if(ferror(f)) {
	fprintf(stderr, "ERROR: unable read file `%s`: %s\n", file_path, strerror(errno));
	exit(1);
    }

    fclose(f);
    return (String_View) { .count = n, .data = buffer };
}

#endif // * SV_IMPLEMENTATION