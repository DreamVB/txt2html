#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string get_css_style(string stylesheet){
	fstream css;
	std::string str_line;
	std::string buffer;
	
	css.open(stylesheet, ios::in);

	if (css.is_open()){
		//Add new line
		buffer = "\n";

		while (std::getline(css, str_line)){
			//Build buffer.
			buffer += str_line + "\n";
		}
		//Add new line.
		buffer += "\n";
		//Close open file.
		css.close();
	}
	//Clear up.
	str_line.clear();
	//Return buffer.
	return buffer;
}

void write_head(fstream& f, bool has_style, string css_file){
	std::string style_data;

	f << "<!DOCTYPE html>" << std::endl;
	f << "<html>" << std::endl;
	f << "<head>" << std::endl;
	f << "<meta http-equiv=\"" << "Content-Type\"" << " content=\""
		<< "text/html ; charset=UTF-8\"" << " />" << std::endl;
	
	f << "<title>Page Title</title>" << std::endl;

	//Test if we need to include style code
	if (has_style){
		//Get style data.
		style_data = get_css_style(css_file);

		if (style_data.length() > 0){
			f << "<style>" << style_data.c_str() << "</style>" << std::endl;
		}
		//Clear up.
		style_data.clear();
	}

	f << "</head>" << std::endl;
}

void write_footer(fstream& f){
	f << "</body>" << std::endl;
	f << "<!-- Converted using txt2html created by Ben a.k.a Dreamvb -->" << std::endl;
	f << "</html>" << std::endl;
}


int main(int argc, char *argv[]){

	std::string src_file;
	std::string out_file;
	std::string str_line;
	std::string css_file;
	bool include_style = false;

	fstream fin;
	fstream fout;

	if (argc < 2){
		std::cout << "Text2Html version 1.1" << std::endl;
		std::cout << "The syntax of the command is incorrect." << std::endl
			<< "Use: " << argv[0] << " input.txt output.html [sytle-sheet.css]" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	include_style = (argc == 4);

	//Check if we need to include a style sheet file.
	if (include_style){
		//Set style sheet file.
		css_file = argv[3];
	}

	//Get source file
	src_file = argv[1];
	out_file = argv[2];

	//Open source file for reading.
	fin.open(argv[1], std::ios::in);
	if (!fin.is_open()){
		std::cout << "Error reading input file." << std::endl;
		exit(EXIT_FAILURE);
	}

	//Open outpt file for write
	fout.open(argv[2], std::ios::out);
	//Test if file was ok and created.
	if (!fout.good()){
		std::cout << "Error writeing to output file." << std::endl;
		//Close open file.
		fin.close();
		exit(EXIT_FAILURE);
	}

	//Write html file header
	write_head(fout, include_style, css_file);

	//Write start of body
	fout << "<body>" << std::endl << std::endl;
	
	if (include_style){
		fout << "<div class=\"" << "wrap" << "\"" << ">" << std::endl;
		fout << "<div class=\"" << "main-body-text" << "\"" << ">" << std::endl;
	}

	//Write te body content
	while (std::getline(fin,str_line)){
		//Check if length is > 0 and first char is #
		if (str_line.length() > 0 && str_line[0] == '#'){
			//Remove the char.
			str_line.erase(0, 1);
			//Set bold
			fout << "<b>" << str_line << "</b><br />" << std::endl;
		}
		//Check if length is > 0 and first char is *
		else if (str_line.length() > 0 && str_line[0] == '*'){
			//Remove the char.
			str_line.erase(0, 1);
			//Set italic
			fout << "<i>" << str_line << "</i><br />" << std::endl;
		}
		//Check for hoz lines
		else if (str_line == "- - -"){
			fout << "<hr />" << std::endl;
		}
		else{
			//Write each line to the output file.
			fout << str_line << "<br />" << std::endl;
		}
		
	}
	if (include_style){
		fout << "</div>" << std::endl;
		fout << "</div>" << std::endl;
	}
	//Write footer
	write_footer(fout);

	//Close files.
	fout.close();
	fin.close();

	return EXIT_SUCCESS;
}