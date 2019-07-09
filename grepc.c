#include <stdio.h>
#include <regex.h>

void displayHelp() {
	printf("help page");
}

int main(int argc, char **argv) {
	if(argc < 2) {
		displayHelp();
		return 0;
	}
	
	regex_t pattern;
	regcomp(&pattern, argv[1], 0);

	for(int i = 2; i < argc; ++i) {
		FILE *file = fopen(argv[i], "r");
		int linecounter = 1;
		char line[256];

		printf("Matches in %s:\n", argv[i]); 
		while (fgets(line, sizeof(line), file)) {
			int result = regexec(&pattern, line, 0, NULL, 0);
			if (!result) {
				printf("%d: %s", linecounter, line);
			}
			linecounter++;
		}
		
		fclose(file);
	}

	regfree(&pattern);
	return 0;
}
