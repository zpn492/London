# Walker
Walker Is an experiment trying to archieve random walk. 
It consists for two steps, one generate a seed, basicly an image with [DoG](https://en.wikipedia.org/wiki/Difference_of_Gaussians) applied. Second goes like this, choose a pseudo random position within the seed, find nearest edge, follow that edge until it is no more. Foreach position in the following sequence, add a step the to return path.
<br /> <br />
> Approach
```
Generate seed:
* Generate gaussian kernel (A) 5x5, sigma 1
* Generate gaussian kernel (B) 5x5, sigma 2
* Create DoG = kernel (B) - kernel (A)
* Take a random image (i used city_160.png)
* Convolve kernel (A) onto image
* Convolve DoG onto image
* Apply threshold (all pixels is either 255 or 120)
* Align neighbourhoods (set all within a neighbourhood to 255 or 120)
* Find all edges between 255 and 120, set em to 0.

The seed will only be generated once.

Find a path:
* use srand() to create random pos(x, y) with in seed // <stdlib.h>
* use srand() to choose direction d -1 (up left) or 1 (down right)
* from pos(x, y) search for an edge in direction d
* if one was found set x = 0; y = 0;
* follow the edge in direction d until position is no longer an edge
* Foreach step in direction d, set x += 1*d; y+=1*d; & add pos(x, y) to path
* return path; // Ex. (0,0), (1, 0), (1,1) etc.;
```