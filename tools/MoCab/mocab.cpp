/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
	
void preprocessTemplate(vector<string> &defines, string &setupFile) {
	size_t index, index2;
	while((index=setupFile.find("#ifdef"))!=string::npos) {
		index2=setupFile.find("#endif");
		
		size_t defineIndex = index+6;
		while(setupFile[defineIndex]==' ') defineIndex++;
		size_t endOfDefine = defineIndex;
		while(setupFile[endOfDefine]!=' ' && setupFile[endOfDefine]!='\n') endOfDefine++;
		bool hasDefine = false;
		
		for(size_t i = 0; i < defines.size(); i++) {
			int hits = 0;
			for(size_t j = defineIndex; j < defineIndex+defines[i].length(); j++) {
				if(defines[i][j-defineIndex]==setupFile[j]) {
					hits++;
				}
			}
			if(hits == defines[i].length()) {
				hasDefine = true;
				break;
			}
		}

		if(!hasDefine) {
			setupFile = 
				setupFile.substr(0, index) +
				setupFile.substr(index2+6, setupFile.length()-(index2+6));
		} else {
			setupFile = 
				setupFile.substr(0, index) + 
				setupFile.substr(endOfDefine, index2-endOfDefine) +
				setupFile.substr(index2+6, setupFile.length()-(index2+6));
		}
	}
}

bool readTemplate(const char *filename, string &templateString) {
	char *temp;
	FILE *f = fopen(filename, "r");
	if(!f) { printf("missing template"); return false; }
	fseek(f, 0, SEEK_END);
	int fileSize = ftell(f);
	temp = new char[fileSize+1];
	fseek(f, 0, SEEK_SET);
	fileSize = fread(temp, 1, fileSize, f);
	fclose(f);
	temp[fileSize] = 0;
	templateString = temp;
	delete temp;
	return true;
}

void replaceTemplate(string &templateString, string templateName, string str) {
	int index = 0;
	while((index = templateString.find(templateName, index))!=string::npos) {
		//templateString.replace(index, str.length(), str);
		templateString = 
			templateString.substr(0, index) +
			str +
			templateString.substr(index+templateName.length(), templateString.length());
		index = index+str.length();

	}
}

bool writeFile(const char *filename, string file) {
	FILE *f = fopen(filename, "w");
	if(!f) return false;
	fwrite(&file[0], 1, file.length(), f);
	fclose(f);
	return true;
}

int main(int argc, char **argv) {
	if(argc<2) {
		printf("-----------------------------------------------------\n");
		printf("* MoCab (c) Mobile Sorcery                          *\n");
		printf("* MoSync Cab Installer Generator                    *\n");
		printf("-----------------------------------------------------\n");
		printf("* usage:                                            *\n");
		printf("*                                                   *\n");
		printf("* mocab [options]                                   *\n");
		printf("*                                                   *\n");
		printf("* options:                                          *\n");
		printf("* -name <name of application>                       *\n");
		printf("* -define <identifier>                              *\n");
		printf("* -template <filepath>                              *\n");
		printf("* -output <filepath>                                *\n");
		printf("-----------------------------------------------------\n");
		return 0;
	}

	string name, templatePath, outputPath;
	vector<string> defines;

	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-name")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -name");			
				return 1;
			}
			name = argv[i];
		} else if(strcmp(argv[i], "-define")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -define");			
				return 1;
			}
			defines.push_back(argv[i]);
		} else if(strcmp(argv[i], "-template")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -template");			
				return 1;
			}
			templatePath = argv[i];
		} else if(strcmp(argv[i], "-output")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -output");			
				return 1;
			}
			outputPath = argv[i];
		}
	}

	if(name.length() == 0 || templatePath.length() == 0 || outputPath.length() == 0) {
		printf("must specify name, template and output\n");
		return 1;
	}

	string setupFile;
	if(!readTemplate(templatePath.c_str(), setupFile)) {
		printf("Couldn't read template!\n");
		return 1;
	}
	preprocessTemplate(defines, setupFile);
	replaceTemplate(setupFile, "%INSERTNAMEHERE%", name);
	if(!writeFile(outputPath.c_str(), setupFile)) {
		printf("Couldn't write file!\n");
		return 1;
	}

	return 0;
}
