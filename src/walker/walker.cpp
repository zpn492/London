#include "walker.hpp"

Walker::Walker(int cols, int rows)
    {
    this->cols = cols;
    this->rows = rows;
    filter = generateGauss(1, 1);
	filter2 = generateGauss(4, 2);
    };

std::vector<std::vector<double> > Walker::generateGauss(int sigma, int k)
    {
    double sum = 0;
    double x, y;
    double centerX = cols/2;
    double centerY = rows/2;
    double sigmaSq = sigma * sigma;
    double cons    = 1 / (2*PI*sigmaSq);
    double cons2   = 2*k*sigmaSq;
    std::vector<std::vector<double> > data;
    for(int i = 0; i < cols; i++)
        {
        // Add new column
        std::vector<double> col;
        data.push_back(col);
        // Calculate value for pos (i(col), j(row))
        for(int j = 0; j < rows; j++)
            {
            x = i - centerX;
            y = j - centerY;
            double v = cons * pow(E, -(x*x + y*y) / cons2);
            sum += v;
            data[i].push_back(v);
            }
        }
    // Normalize
    for(int i = 0; i < cols; i++) for(int j = 0; j < rows; j++) data[i][j] /= sum;

    return data;
    };

double Walker::convolveSinglePixel(std::vector<std::vector<double> > &a, 
    std::vector<std::vector<double> > &b,
    int x,
    int y)
    {
    double sum = 0;

    // find center position of kernel (half of kernel size)
    int kCenterX = b.size() / 2;
    int kCenterY = b[0].size() / 2;

    for(int m=0; m < b[0].size(); ++m)     // kernel rows
        {
        int mm = b[0].size() - 1 - m;      // row index of flipped kernel

        for(int n=0; n < b.size(); ++n)    // kernel columns
            {
            int nn = b.size() - 1 - n;     // column index of flipped kernel

            // index of input signal, used for checking boundary
            int ii = y + (kCenterY - mm);
            int jj = x + (kCenterX - nn);

            // ignore input samples which are out of bound
            if( ii >= 0 && ii < a[0].size() && jj >= 0 && jj < a.size() )
                sum += a[jj][ii] * b[nn][mm];
            }
        }
    //std::cout << "Sum: " << sum << std::endl;
    return sum;
    };

std::vector<std::vector<double> > Walker::convolve2D(std::vector<std::vector<double> > a, 
    std::vector<std::vector<double> > b, bool debug)
    {
    // find center position of kernel (half of kernel size)
    int kCenterX = b.size() / 2;
    int kCenterY = b[0].size() / 2;

    std::vector<std::vector<double> > out;
    
    for(int i = 0; i < a.size(); i++)
        {
        std::vector<double> d;
        out.push_back(d);
        for(int j = 0; j < a[0].size(); j++)
            out[i].push_back(0.1);
        }   
    
    for(int i=0; i < a[0].size(); ++i)             // rows
        {
        for(int j=0; j < a.size(); ++j)            // columns
            {
            out[j][i] += convolveSinglePixel(a, b, j, i);
            /*
            for(int m=0; m < b[0].size(); ++m)     // kernel rows
                {
                int mm = b[0].size() - 1 - m;      // row index of flipped kernel

                for(int n=0; n < b.size(); ++n)    // kernel columns
                    {
                    int nn = b.size() - 1 - n;     // column index of flipped kernel

                    // index of input signal, used for checking boundary
                    int ii = i + (kCenterY - mm);
                    int jj = j + (kCenterX - nn);

                    // ignore input samples which are out of bound
                    if( ii >= 0 && ii < a[0].size() && jj >= 0 && jj < a.size() )
                        {
                        out[j][i] += a[jj][ii] * b[nn][mm];
                        }
                    }
                }
            */
            }
        }
    double sum = 0;
    for(int i = 0; i < a.size(); i++) for(int j = 0; j < a[0].size(); j++) 
        sum += out[i][j];
    for(int i = 0; i < a.size(); i++) for(int j = 0; j < a[0].size(); j++) 
        out[i][j] = out[i][j]*255000 / sum;
    if(debug)
        {
        linalg::Matrix m1(out);
        m1.pretty_print();
        }

    return out;
     };

std::vector<std::vector<double> > Walker::decodeTwoSteps(const char* filename)
    {
    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    std::vector<std::vector<double> > out;
    unsigned w, h;

    //load and decode
    unsigned error = lodepng::load_file(png, filename);
    if(!error) error = lodepng::decode(image, w, h, png);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    //plot the pixels of the PNG file
    unsigned jump = 1;
    if(w / 1024 >= jump) jump = w / 1024 + 1;
    if(h / 1024 >= jump) jump = h / 1024 + 1;

    for(unsigned y = 0; y + jump - 1 < h; y += jump)
        {	
        std::vector<double> d;
        out.push_back(d);
        for(unsigned x = 0; x + jump - 1 < w; x += jump)
            {
                //get RGBA components
                int r = image[4 * y * w + 4 * x + 0]; //red
                int g = image[4 * y * w + 4 * x + 1]; //green
                int b = image[4 * y * w + 4 * x + 2]; //blue
                int a = image[4 * y * w + 4 * x + 3]; //alpha

                //make translucency visible by placing checkerboard pattern behind image
                int checkerColor = 191 + 64 * (((x / 16) % 2) == ((y / 16) % 2));
                r = (a * r + (255 - a) * checkerColor) / 255;
                g = (a * g + (255 - a) * checkerColor) / 255;
                b = (a * b + (255 - a) * checkerColor) / 255;

                out[y].push_back(r);
            }
        }
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
    return out;
    };

void Walker::applyThreshold(std::vector<std::vector<double> > &data)
    {
    double t_size = data.size()*data.size();
	double th = 0;
	for(int i = 0; i < data.size(); i++)
	for(int j = 0; j < data[i].size(); j++)
		th += abs(data[i][j]);
	th = th / t_size;

	for(int i = 0; i < data.size(); i++)
	for(int j = 0; j < data[i].size(); j++)
		if(data[i][j] < th)
			data[i][j] = 255;
		else
			data[i][j] = 0;
    };

void Walker::alignNeighbours(std::vector<std::vector<double> > &data, int thres)
    {
    std::vector<std::vector<double> > data2 = data;

    for(int i = 0; i < data.size(); i++)
	for(int j = 0; j < data[i].size(); j++)
		{
		int counter = 0;
		for(int k = -1; k < 2; k++)
			{
			if(i + k >= 0 && i + k < data.size())
				{
				for(int v = -1; v < 2; v++)
				{
				if(j + v >= 0 && j + v < data[i].size())
					{
					if(data[i+k][j+v] == 0)
						counter += 1;
					}

				}
				}
			}
		if(counter >= thres)
			{
			for(int k = -2; k < 3; k++)
			{
			if(i + k >= 0 && i + k < data.size())
				{
				for(int v = -2; v < 3; v++)
					{
					if(j + v >= 0 && j + v < data[i].size())
						{
						data2[i+k][j+v] = 120;
						}
					}
				}
			}
			}
		else 
			{
			for(int k = -2; k < 3; k++)
			{
			if(i + k >= 0 && i + k < data.size())
				{
				for(int v = -2; v < 3; v++)
					{
					if(j + v >= 0 && j + v < data[i].size())
						{
						data2[i+k][j+v] = 255;
						}
					}
				}
			}
			}
		}
    data = data2;
    };

void Walker::applyEdge(std::vector<std::vector<double> > &data)
    {
    for(int i = 0; i < data.size(); i++)
	for(int j = 0; j < data[i].size(); j++)
		{
		if(data[i][j] == 120)
			continue;
		int counter = 0;
		for(int k = -1; k < 2; k++)
        {
		if(i + k >= 0 && i + k < data.size())
            {			
			for(int v = -1; v < 2; v++)
                {
				if(j + v >= 0 && j + v < data[i].size())
                    {
                    if(data[i+k][j+v] == 120)
						counter += 1;
                    }
                }
            }
        }
		if(counter > 0)
		data[i][j] = 0;
		}
    }

std::vector<std::vector<double> > Walker::DoG()
    {
    linalg::Matrix f(filter);
	linalg::Matrix f2(filter2);
	f2 - f;
    return f2.data;
    };

std::vector<std::vector<double> > Walker::seed(const char *filename)
    {
    std::vector<std::vector<double> > data = decodeTwoSteps(filename);
    data = convolve2D(data, filter);
    data = convolve2D(data, DoG());
    applyThreshold(data);
	alignNeighbours(data, 3);
    applyEdge(data);
    return data;
    };

std::vector<Node> Walker::randomWalk(std::vector<std::vector<double> > &data)
    {
    std::vector<Node> path;
    timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
    int x = rand() % data.size(); 
    int y = rand() % data[0].size();
    int dx, dy;
    int ox, oy;
    ox = 0;
    oy = 0;
    int d = rand() % 2 == 0 ? -1 : 1; // direction 0 ? UP LEFT : DOWN RIGHT
    // Find edge
    while(data[x][y] != 0)
        {
        x += 1*d;
        y += 1*d;
        if(x < 0 || x >= data.size() || y < 0 || y >= data[0].size())
            {
            //std::cout << "out of bounds - random walk" << std::endl;
            break;
            }
        }    
    // Follow edge
    while(true)
        {
        dx = x+1*d;
        dy = y+1*d; 
        if(dx < 0 || dx >= data.size() || dy < 0 || dy >= data[0].size())
            {
            break;
            }

        if(data[dx][y] == 0)
            {
            x = dx;
            ox += 1*d;
            path.push_back(Node(0, ox, oy));
            }
        else if(data[x][dy] == 0)
            {
            y = dy;
            oy += 1*d;
            path.push_back(Node(0, ox, oy));
            }
        else
            break;
        }
    return path;
    };