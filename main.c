#include<stdio.h>

#define SV_IMPLEMENTATION
#include "./string_view.c"

static void fetch_todos(const char* file_path) {
    // * Read file as String_View
    String_View str_ans = read_from_file_to_sv(file_path);
    
    while(str_ans.count != 0) {
	String_View value = sv_chop_by_delim(&str_ans, '\n');
	
	if(sv_eq(value, cstr_to_sv("// TODO"))) {
	    printf("------ TODO found ----- \n");
	    
	    // * TODO Content
	    String_View body = sv_chop_by_delim(&str_ans, '\n');
	    while(!sv_eq(body, cstr_to_sv("// TODOEND"))) {
		SV_PRINT(body);
		body = sv_chop_by_delim(&str_ans, '\n');
		str_ans = sv_trim(str_ans);
	    }

	    sv_chop_by_delim(&str_ans, '\n');
	    printf("------ TODOEND found ----- \n");
	}	
    }    
}

int main(void) {
    fetch_todos("todos.c");
    return 0;
}
