#include <volutils.h>

VolumeScalar erode(const VolumeScalar& vol);
VolumeScalar dilate(const VolumeScalar& vol);
VolumeScalar open(const VolumeScalar& vol, int size);
VolumeScalar close(const VolumeScalar& vol, int size);

VolumeScalar erode(const VolumeScalar& vol);
VolumeScalar dilate(const VolumeScalar& vol);
VolumeScalar open(const VolumeScalar& vol, int size);
VolumeScalar close(const VolumeScalar& vol, int size);

void advect(VolumeScalar& vol, float time, int sign, float target_grad);
void erode_grey(VolumeScalar& vol, float t, float target_grad);
void dilate_grey(VolumeScalar& vol, float t, float target_grad);
void open_grey(VolumeScalar& vol, float t, float target_grad, bool writeIntermediateFiles);
void close_grey(VolumeScalar& vol, float t, float target_grad, bool writeIntermediateFiles);
//VISVolume<float> distance(VolumeScalar& vol, float clamp);

// for images, sets distance transform locally
float reinitDistance(VISImage<float>& image, float target_grad);
//float reinitDistance(VISVolume<float>& vol, float target_grad, float minmax = 1.0);
//float reinitDistance(VISVolume<float>& vol, float target_grad);
float reinitDistanceOld(VISVolume<float>& vol, float target_grad);
float reinitDistanceOld(VISVolume<float>& vol, float target_grad, const VISVolume<float> &mask);
float reinitDistance(VISVolume<float>& vol, float target_grad, float minmax, const VISVolume<boolean> &mask = VISVolume<boolean>());
float reinitDistance(VISVolume<float>& vol, float target_grad, float minmax, VISVolIndexVISList &mask_list);
float reinitDistance(VISVolume<float>& vol, float target_grad, float minmax, const VISVolume<boolean> &mask, float reinit_tolerance, int max_reinit_iterations);
float reinitDistance(VISVolume<float>& vol, float target_grad, float minmax, float reinit_tolerance, int max_reinit_iterations);
void createMask(VISVolume<float>& vol, VISVolIndexVISList &list, float gradient);
//float reinitDistance1(VISVolume<float>& vol, float target_grad);
void tighten(VISImage<float>& image, float radius);

int initZeroCrossings(const VISVolume<float>& vol_scalar, VISVolume<float>& vol_out, VISVolume<boolean> &mask, float minmax);

void gaussDiffuse(VolumeScalar &vol, double sigma);

// for images, sets distance transform locally
//float reinitDistance(VISVolume<float>& volume, float target_grad);
void tighten(VISVolume<float>& volume, float radius);

#define NUM_DERIVS (21)
#define DX (0)
#define DY (1)
#define DZ (2)
#define DPX (3)
#define DPY (4)
#define DPZ (5)
#define DMX (6)
#define DMY (7)
#define DMZ (8)
#define DYPX (9)
#define DZPX (10)
#define DXPY (11)
#define DZPY (12)
#define DXPZ (13)
#define DYPZ (14)
#define DYMX (15)
#define DZMX (16)
#define DXMY (17)
#define DZMY (18)
#define DXMZ (19)
#define DYMZ (20)
#define NSIZE (27)

inline VISMatrix calculate_curvature (const float* derivs, 
				      float &curve_trace, float &curve_norm);
void get_neighborhood (int, int, int, float*, const VISVolume<float>&);
void get_derivs(const float*, float*);
void clamp_curvature(VolumeScalar& vol, float epsilon);
void clamp_curvature(VISImage<float>& vol, float epsilon);







