#include "filehandler.h"

bool filehandler::exists_file(const char *filename)
    {
    std::ifstream tfile(filename);
        if(tfile.is_open())
            {
            tfile.close();
            return true;
            }
	printf("File not found, %s\n", filename);
    return false;
    };

std::streampos filehandler::file_size(const char *filename)
	{
	if(!filehandler::exists_file(filename))
		return 0;
    std::streampos begin, end;
	std::ifstream mfile(filename, std::ios::binary);
	begin = mfile.tellg();
	mfile.seekg(0, std::ios::end);
	end = mfile.tellg();
	mfile.close();
	return end - begin;
	};

/* Read an entire file into a string */
std::string filehandler::get_file_contents(const char *filename)
	{   
	if(!filehandler::exists_file(filename))
		return std::string("");
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
		{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
		}
	throw(errno);
	};

std::string filehandler::get_file_chunk(const char *filename, 
    size_t begin, size_t end)
    {
    if(!filehandler::exists_file(filename))
		return std::string("");
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
		{
		std::string contents;
		in.seekg(begin);
		contents.resize(end-begin);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
		}
	throw(errno);
    };

bool filehandler::create_file(const char *filename)
    {
	if(!filehandler::exists_file(filename))
		{
		std::ofstream mfile;
		mfile.open(filename);
		mfile.close();
		printf("Create file, %s\n", filename);
		return true;
		}
	return false;
    };

bool filehandler::delete_file(const char *filename)
    {
	if(remove(filename) != 0)
		{
		return false;
		}
		printf("Delete file, %s\n", filename);
	return true;
    };

std::vector<std::string> filehandler::split_string(std::string &stemp, char delimeter)
    {
    std::vector<std::string> tokens;
    size_t pos;
    while((pos = stemp.find(delimeter))!=std::string::npos)
        {
        std::string ss = stemp.substr(0, pos);
        stemp = stemp.erase(0, pos + 1);
        tokens.push_back(ss);
        }
        tokens.push_back(stemp);
    return tokens;
    };

std::vector<std::string> filehandler::divide_string(std::string s1, int times)
	{	
	std::vector<std::string> data1, data2;
	std::string s2="";
	
	int t = s1.length();
	
	s2 = s1.substr(s1.length()/2+1);	
	s1 = s1.substr(0, s1.length()/2+1);
	
	// Make sure splitting by chunks won't mess up input data
	// find \n
	// if \n != last char
	if(s1[s1.length()-1] != '\n')
		{
		s2 = s1.substr(s1.find_last_of('\n')+1) + s2;
		s1 = s1.substr(0, s1.find_last_of('\n'));
		}
	
	if(times != 1 && t > 10000)
		{
		data1 = divide_string(s1, times-1);
		data2 = divide_string(s2, times-1);
		}
	else
		{
		data1.push_back(s1);
		data2.push_back(s2);
		}
	
	data1.insert(data1.end(), data2.begin(), data2.end());
	
	return data1;
	}

void filehandler::write_file(const char *filename, std::string text)
    {
    std::ofstream outf;
    outf.open(filename);

    outf << text.c_str();
    outf.close();
    };

void filehandler::print_file(const char *filename)
    {
    if(!filehandler::exists_file(filename))
        return;
    std::ifstream mfile(filename);
    std::string line;

    while(getline(mfile, line))
        printf("%s \n", line.c_str());

    mfile.close();
    };

std::string filehandler::int_to_string(int value)
    {
    std::ostringstream oss;
    oss << value;
    return oss.str();
    };

std::string filehandler::double_to_string(double value)
    {
    std::ostringstream oss;
    oss << value;
    return oss.str();
    };
	
void filehandler::replace(std::string &s, char before, char after)
	{
	std::replace(s.begin(), s.end(), before, after);
	};

std::string filehandler::space(std::string s, int space)
        {
        int reminder = space - s.length();
        for (int i = 0; i < reminder; i++)
            s += " ";
        return s;
        };

/* Run a function with a series of input
 * For each result, store x values in a historgram or  */
void filehandler::histogram(const char *filename, std::vector<std::string> labels, std::vector<double> dataset)
    {
    if(labels.size() != dataset.size())
        std::cout << "Labels" << labels.size() << " and dataset: ";
        std::cout << dataset.size() << " should be of same size" << std::endl;
    
    std::string tmp = "var ctx = document.getElementById(\"myChart\").getContext('2d');";
    tmp += "var myChart = new Chart(ctx, {type: 'bar', data: {";

    tmp += "labels: [";
    for(int i = 0; i < labels.size()-1; i++)
        {
        tmp += "\""; tmp += labels[i]; tmp += "\", ";
        }
    tmp += "\""; tmp += labels[labels.size()-1]; tmp += "\"";
    tmp += "],";
    tmp += "datasets: [{label: 'Some label',";
    tmp += " data: [";
    for(int j = 0; j < dataset.size()-1; j++)
        {
        tmp += filehandler::int_to_string(dataset[j]); tmp += ", ";
        }
    tmp += filehandler::int_to_string(dataset[dataset.size()-1]);
    tmp += "], ";
    tmp += "borderWidth: 1}]}, options: { scales: { yAxes: [{ ticks: { beginAtZero:true } }] } } });";
    filehandler::write_file(filename, tmp.c_str());
    };

/* Run a function with a series of input
 * For each result, store x values in a historgram or  */
void filehandler::linechart(const char *filename, std::vector<std::string> labels, std::vector<double> dataset)
    {
    std::string tmp = "var ctx = document.getElementById(\"myChart\").getContext('2d');";
    tmp += "var myChart = new Chart(ctx, {type: 'line', data: {";

    tmp += "labels: [";
    for(int i = 0; i < labels.size()-1; i++)
        {
        tmp += "\""; tmp += labels[i]; tmp += "\", ";
        }
    tmp += "\""; tmp += labels[labels.size()-1]; tmp += "\"";
    tmp += "],";
    tmp += "datasets: [{label: 'Some label', pointColor: 'rgba(220,220,220,1', pointStrokeColor: '#fff', ";
    tmp += " data: [";
    for(int j = 0; j < dataset.size()-1; j++)
        {
        tmp += filehandler::double_to_string(dataset[j]); tmp += ", ";
        }
    tmp += filehandler::double_to_string(dataset[dataset.size()-1]);
    tmp += "], ";
    tmp += "borderWidth: 1}]}, options: { scales: { yAxes: [{ ticks: { beginAtZero:true } }] } } });";
    filehandler::write_file(filename, tmp.c_str());
    };