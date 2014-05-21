#include <volutils.h>
#include <volumefile.h>
#include <morphology.h>

int main(int argc, char** argv)
{

  if (argc < 3)
    {
      cout << "usage: morphsmooth radius infile outfile" << endl;
      exit(-1);
    }

  float radius;
  int morph_size;
  VolumeScalar vol_in, vol_out;
  VISImageFile imfile;
  VISVolumeFile volfile;
  int args = 1;
  sscanf(argv[args++], "%f", &radius);
  cout << "radius is " << radius << endl;
  float vol_min, vol_max;
  //  sscanf(argv[1], "%d", &morph_size);

  VolScale scale;

  vol_in = readScalarVolumeFile(argv[args++],scale);
  cout << "min input " << (vol_min = vol_in.min()) << " and max " << (vol_max = vol_in.max()) << endl;
  
  //  vol_in = 2.0*((vol_in - vol_min)/(vol_max - vol_min)) - 1.0;
  cout << "adjust " << (2.0f*(vol_min/(vol_max - vol_min)) + 1.0f) << endl;
  cout << "mult " << (2.0f/(vol_max - vol_min)) << endl;
  vol_in = vol_in*(2.0f/(vol_max - vol_min)) - (2.0f*(vol_min/(vol_max - vol_min)) + 1.0f);
  // this is for flipping the data from miriah's stuff....
  //  vol_in *= -1.0f;
  //  vol_in.setBorder(-1.0f, 4);

  cout << "min input " << (vol_min = vol_in.min()) << " and max " << (vol_max = vol_in.max()) << endl;

  //  imfile.write((vol_in.image()).becomeFlat(), "vol_in.fts");

  //  vol_in -= 0.5f;

  tighten(vol_in, radius);

  //  volfile.write_float(vol_in, argv[4]);
  writeScalarVolumeFile(argv[args++], vol_in, scale);
  //  volfile.write_float(vol_in, "vol_tight.vol");
  // imfile.write((vol_in.image()).becomeFlat(), "vol_out.fts");

  cout << "min out " << vol_in.min() << " and max " << vol_in.max() << endl;

  cout << "done" << endl;
}