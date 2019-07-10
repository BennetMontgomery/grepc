#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>

void displayHelp() {
	printf("help page");
}

//my compiler refuses to recognize strlwr, so it is implemented here using the solution from
//https://stackoverflow.com/questions/23618316/undefined-reference-to-strlwr
char *strlwr(char *str) {
	unsigned char *p = (unsigned char *)str;

	while (*p) {
		*p = tolower ((unsigned char)*p);
		p++;
	}

	return str;
}

int main(int argc, char **argv) {
	//sending user to help page if they fail to give a pattern/file
	if(argc < 2) {
		displayHelp();
		return 0;
	}
	
	//flag trackers (0 = inactive, 1 = active)
	int ignoreCase = 0;

	regex_t pattern;

	int i = 1;
	/*
		The following collects options by iterating through parameters until one doesn't
		start with "-" or the options terminator "--" is hit.
	*/
	for(; i < argc; ++i) {
		regcomp(&pattern, "^-", 0);
		if(!regexec(&pattern, argv[i], 0, NULL, 0)) {
			regcomp(&pattern, "^--", 0);
			if(!regexec(&pattern, argv[i], 0, NULL, 0)) {
				//"--" acts as flags list terminator
				if(!strcmp(argv[i], "--")) {
					break;
				} else if(!strcmp(argv[i], "--ignore-case")) {
					ignoreCase = 1;
				} else {
					fprintf(stderr, "Error: invalid string flag %s\n", argv[i]);
				}
			} else {
				for(int j = 1; argv[i][j] != '\0'; ++j) {
					if(argv[i][j] == 'i' || argv[i][j] == 'y') {
						ignoreCase = 1;
					} else {
						fprintf(stderr, "Error: invalid char flag %c\n", argv[i][j]);
					}
				}
			}
		} else {
			break;
		} 
	}

	//collecting regex and then adjusting regex in case ignoreCase flag is active
	if(!ignoreCase) {
		regcomp(&pattern, argv[i], 0);
	} else {
		regcomp(&pattern, strlwr(argv[i]), 0);
	}

	//checking for regex in files
	for(i += 1; i < argc; ++i) {
		FILE *file = fopen(argv[i], "r");
		int linecounter = 1;
		char line[256];
		char *processedLine;

		printf("Matches in %s:\n", argv[i]); 
		while (fgets(line, sizeof(line), file)) {
			processedLine = line;
			if (ignoreCase) {
				processedLine = strlwr(line);
			}
			if (!regexec(&pattern, processedLine, 0, NULL, 0)) {
				printf("%d: %s", linecounter, line);
			}
			linecounter++;
		}
		
		fclose(file);
	}

	regfree(&pattern);
	return 0;
}
