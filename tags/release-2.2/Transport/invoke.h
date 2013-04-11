/*
 * Call function with #DEFINEd number of input/output args
 */

#if (NIN == 0)
# if (NOUT==0)
#  define PROTOTYPE(fnc) fnc(void)
#  define INVOKE(fnc,input,output) fnc()
# endif
# if (NOUT==1)
#  define PROTOTYPE(fnc) fnc(float *out0)
#  define INVOKE(fnc,input,output) fnc((output)[0])
# endif
# if (NOUT==2)
#  define PROTOTYPE(fnc) fnc(float *out0, float *out1)
#  define INVOKE(fnc,input,output) fnc((output)[0], (output)[1])
# endif
# if (NOUT==3)
#  define PROTOTYPE(fnc) fnc(float *out0, float *out1, float *out2)
#  define INVOKE(fnc,input,output) fnc((output)[0], (output)[1], (output)[2])
# endif
# if (NOUT==4)
#  define PROTOTYPE(fnc) fnc(float *out0, float *out1, float *out2, float *out3)
#  define INVOKE(fnc,input,output) fnc((output)[0], (output)[1], (output)[2], (output)[3])
# endif
#endif

#if (NIN == 1)
# if (NOUT==0)
#  define PROTOTYPE(fnc) fnc(float *in0)
#  define INVOKE(fnc,input,output) fnc((input)[0])
# endif
# if (NOUT==1)
#  define PROTOTYPE(fnc) fnc(float *in0, float *out0)
#  define INVOKE(fnc,input,output) fnc((input)[0], (output)[0])
# endif
# if (NOUT==2)
#  define PROTOTYPE(fnc) fnc(float *in0, float *out0, float *out1)
#  define INVOKE(fnc,input,output) fnc((input)[0], (output)[0], (output)[1])
# endif
# if (NOUT==3)
#  define PROTOTYPE(fnc) fnc(float *in0, float *out0, float *out1, float *out2)
#  define INVOKE(fnc,input,output) fnc((input)[0], (output)[0], (output)[1], (output)[2])
# endif
# if (NOUT==4)
#  define PROTOTYPE(fnc) fnc(float *in0, float *out0, float *out1, float *out2, float *out3)
#  define INVOKE(fnc,input,output) fnc((input)[0], (output)[0], (output)[1], (output)[2], (output)[3])
# endif
#endif

#if (NIN == 2)
# if (NOUT==0)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1);
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1])
# endif
# if (NOUT==1)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *out0);
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1],  (output)[0])
# endif
# if (NOUT==2)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *out0, float *out1)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (output)[0], (output)[1])
# endif
# if (NOUT==3)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *out0, float *out1, float *out2)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (output)[0], (output)[1], (output)[2])
# endif
# if (NOUT==4)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *out0, float *out1, float *out2, float *out3)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (output)[0], (output)[1], (output)[2], (output)[3])
# endif
#endif

#if (NIN == 3)
# if (NOUT==0)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2])
# endif
# if (NOUT==1)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *out0)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (output)[0])
# endif
# if (NOUT==2)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *out0, float *out1)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (output)[0], (output)[1])
# endif
# if (NOUT==3)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *out0, float *out1, float *out2)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (output)[0], (output)[1], (output)[2])
# endif
# if (NOUT==4)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *out0, float *out1, float *out2, float *out3)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (output)[0], (output)[1], (output)[2], (output)[3])
# endif
#endif

#if (NIN == 4)
# if (NOUT==0)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *in3)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (input)[3])
# endif
# if (NOUT==1)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *in3, float *out0)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (input)[3], (output)[0])
# endif
# if (NOUT==2)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *in3, float *out0, float *out1)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (input)[3], (output)[0], (output)[1])
# endif
# if (NOUT==3)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *in3, float *out0, float *out1, float *out2)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (input)[3], (output)[0], (output)[1], (output)[2])
# endif
# if (NOUT==4)
#  define PROTOTYPE(fnc) fnc(float *in0, float *in1, float *in2, float *in3, float *out0, float *out1, float *out2, float *out3)
#  define INVOKE(fnc,input,output) fnc((input)[0], (input)[1], (input)[2], (input)[3], (output)[0], (output)[1], (output)[2], (output)[3])
# endif
#endif

