#define R iResolution.xy

//a way to sample pixels shorthand 
#define A(U) texture(iChannel0,(U)/R)

//the time step
#define dt 0.9
