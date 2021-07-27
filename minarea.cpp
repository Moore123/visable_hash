#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "murmurhash3.hpp"
#include "xxhash.hpp"
#include "fasthash.hpp"

#include <iostream>
#include <random>

#include <random>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <random>
#include <time.h>

//uint64_t MurmurHash64A(const void *key, int len, unsigned int seed)
//XXH_PUBLIC_API XXH64_hash_t XXH64(const void* input, size_t length, XXH64_hash_t seed);
//uint64_t fasthash64(const void *buf, size_t len, uint64_t seed)

using namespace cv;
using namespace std;

extern "C" {

int count_bits(uint32_t n)
{
	unsigned int c=0;				  // c accumulates the total bits set in v
    while(n) {
        n=n>>1;
        c++;    
    }
	return c;
}

typedef unsigned long long int UINT64;
UINT64 getRandom(const UINT64 & begin = 0, const UINT64 & end = 100) {
  return begin >=
    end ? 0 : begin + (UINT64) rand() ^ ((end - begin) % RAND_MAX);
};

static void help() {
  cout << "This program draws hash visible pics of a method\n"
    << "-h  this help Message\n"
    << "-b  bit width [color]x3:width:height\n"
    << "-f  output filename otherwise rep-change display\n"
    << "-p  points\n"
    << "-r  0 pure random\n"
    << "    1 MurmurHash64B\n"
    << "    2 XXH64\n"
    << "    3 fasthash64\n"
    << "-t  test\n"
    << "-x  width\n"
    << "-y  height\n"
    << "Press ESC, 'q' or 'Q' to exit.\n\n";
}

string generate(int max_length) {
  string possible_characters =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  random_device rd;
  mt19937 engine(rd());
  uniform_int_distribution <> dist(0, possible_characters.size() - 1);
  string ret = "";
  for (int i = 0; i < max_length; i++) {
    int random_index = dist(engine);    //between 0 and possible_characters.size()-1
    ret += possible_characters[random_index];
  }
  return ret;
}

std::string random_string(uint32_t xlen) {
std::string
str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,."
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`!@$%^z*()_+`!@#$%^z*()_+,.");

  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(str.begin(), str.end(), generator);

  return str.substr(0, xlen);   // assumes 32 < number of characters in str         
}

#define minmax(v,x,y) v=(v<x?x:v); \
         v = ( v>y?y:v );

int main(int argc, char** argv) {
  const char *nvalue = "World";
  int rrandom=0, tvalue = 1, xwidth = 50, yheight = 50; 
  int xshift,yshift,xmask,ymask;
  uint32_t rgb_r, rgb_g, rgb_b;
  long num_points=100;
  uint64_t xtmp;
  char *bitwidth = NULL, *fname=NULL;
  uint32_t magic_num=0x5bd1e995;

  int c;
  if (argc < 2) {
    help();
    exit(0);
  }

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution < uint64_t > dis;

  while ((c = getopt(argc, argv, "b:f:hn:r:t:p:x:y:")) != -1) {
    switch (c) {
      case 'n':
        if (optarg)
          nvalue = optarg;
        break;
      case 'r':
        if ( (rrandom = atoi(optarg)) == 0 ) 
            srand (time(NULL));
        break;

      case 't':
        if (optarg)
          tvalue = atoi(optarg);
        minmax(tvalue, 16, 1024);
 #if 0
        for (int x = 0; x < tvalue; x++) {
          string tmps = generate(tvalue);
// MurmurHash64B(const void *key, int len, unsigned int seed)
          xtmp=MurmurHash64B((const void *)tmps.c_str(), tvalue,magic_num);
          cout << x << ":\t"<< std::hex << xtmp << " | " << tmps << "\n";

        }
        exit(0); 
#endif
        break;

      case 'h':
        help();
        exit(0);
        break;

      case 'b':
        if (optarg)
          bitwidth = strdup(optarg);
        break;

      case 'f':
        if (optarg)
          fname = strdup(optarg);
        break;


      case 'p':
        if (optarg)
          num_points = abs(atol(optarg));
        break;

      case 'x':
        if (optarg)
          xwidth = atoi(optarg);
        minmax(xwidth, 128, 4096);
        xshift = count_bits(xwidth);
        xmask = (1<<xshift)-1;
        break;
      case 'y':
        if (optarg)
          yheight = atoi(optarg);
        minmax(yheight, 128, 4096);
        yshift = count_bits(yheight);
        ymask = (1<<yshift)-1;
        break;

      default:
        break;
    }
  }

  Mat img(xwidth, yheight, CV_8UC3, Scalar::all(0));
  RNG & rng = theRNG();

  for (;;) {
    int i, count = rng.uniform(10, 101);
    vector < Point > points;
    vector < Point3d > xyzPoints;

    // Generate a random set of points
    for (i = 0; i < num_points; i++) {
      Point pt;
      Point3d xrgb;
      
      string tmps = generate(tvalue);
        switch(rrandom) {
            case 0:
               xtmp = dis(gen);
               break;
            case 1:
                xtmp=MurmurHash64B((const void *)tmps.c_str(), tvalue,magic_num);
                break;
            case 2:
                xtmp=XXH64((const void *)tmps.c_str(), tvalue,magic_num);
                break;
            case 3:
                xtmp=fasthash64((const void *)tmps.c_str(), tvalue,magic_num);
                break;
        }
      //magic_num = ( xtmp >> 10 ) & 0xFFFFFFFF;
      //else xtmp = getRandom(0, ULLONG_MAX);

      pt.y = (xtmp & (uint64_t)xmask)%xwidth;
      xtmp = (xtmp >> xshift);
      pt.x = (xtmp & (uint64_t)ymask)%yheight;
      xtmp = (xtmp >> yshift);
      xrgb.x = xtmp & (uint64_t)0xFF;
      xtmp = (xtmp >> 8);
      xrgb.y = xtmp & (uint64_t)0xFF;
      xtmp = (xtmp >> 8);
      xrgb.z = xtmp & (uint64_t)0xFF;

      points.push_back(pt);
      xyzPoints.push_back(xrgb);
    }

    // Find the minimum area enclosing bounding box
    Point2f vtx[4];
    RotatedRect box = minAreaRect(points);
    box.points(vtx);

    // Find the minimum area enclosing triangle
    vector < Point2f > triangle;
    minEnclosingTriangle(points, triangle);

    // Find the minimum area enclosing circle
    Point2f center;
    float radius = 0;
    minEnclosingCircle(points, center, radius);

    img = Scalar::all(0);

    // Draw the points
    for (i = 0; i < num_points; i++) {
      //  Vec3b & color = image.at<Vec3b>(y,x);
      Vec3b & color = img.at<Vec3b>(points[i].y , points[i].x);
      color[0]=xyzPoints[i].x;
      color[1]=xyzPoints[i].y;
      color[2]=xyzPoints[i].z;
      //circle(img, points[i], 1, Scalar(xyzPoints[i].x , xyzPoints[i].y , xyzPoints[i].z), FILLED, LINE_AA);
    }

    if ( fname != NULL ) {
      imwrite(fname, img);
      break;
    }
    else 
      imshow("Rectangle, triangle & circle", img);

    char key = (char) waitKey();
    if (key == 27 || key == 'q' || key == 'Q')  // 'ESC'
      break;
  }

  return 0;
}


} // end extern "C"
