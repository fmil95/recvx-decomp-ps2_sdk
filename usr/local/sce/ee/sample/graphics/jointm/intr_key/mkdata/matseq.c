/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 * 
 * 
 *                         Version 0.5.0
 *                           Shift-JIS
 * 
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : joint matrix interpolation mkdata
 *                     Description : matrix sequence data
 * 
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */

#include <eekernel.h>                    
#include <eeregs.h>
#include <sifdev.h>
#include <libvu0.h>
#include <libdma.h>

int objnum=10;
int keynum=5;


// 内挿コマ数
unsigned long count[]={40,40,40,40,40};

// キーフレームマトリクス
sceVu0FMATRIX matrix[10][6]=
{
  {/* obj #0 */
    {/* pose #0 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.000000,     0.000000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    0.882949,     0.129403,    -0.451283,     0.000000, },
        {    0.000000,     0.961262,     0.275637,     0.000000, },
        {    0.469470,    -0.243373,     0.848745,     0.000000, },
        {    0.000000,     0.000000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    0.766045,     0.177176,    -0.617886,     0.000000, },
        {    0.000000,     0.961262,     0.275637,     0.000000, },
        {    0.642786,    -0.211150,     0.736370,     0.000000, },
        {    0.000000,     0.000000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    0.882949,     0.129403,    -0.451283,     0.000000, },
        {    0.000000,     0.961262,     0.275637,     0.000000, },
        {    0.469470,    -0.243373,     0.848745,     0.000000, },
        {    0.000000,     0.000000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.927185,     0.103255,    -0.360093,     0.000000, },
        {    0.000000,     0.961262,     0.275637,     0.000000, },
        {    0.374605,    -0.255566,     0.891267,     0.000000, },
        {   -0.150000,     0.000000,    -0.300000,     1.000000, },
    },
    {/* pose #5 */
        {    0.927185,     0.103255,    -0.360093,     0.000000, },
        {    0.000000,     0.961262,     0.275637,     0.000000, },
        {    0.374605,    -0.255566,     0.891267,     0.000000, },
        {   -0.418879,     0.000000,    -0.802851,     1.000000, },
    },
  },
  {/* obj #1 */
    {/* pose #0 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.000000,    -3.500000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    0.898794,     0.000000,     0.438370,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {   -0.438370,     0.000000,     0.898794,     0.000000, },
        {    0.000000,    -3.500000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    0.694659,     0.000000,     0.719339,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {   -0.719339,     0.000000,     0.694659,     0.000000, },
        {    0.000000,    -3.500000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    0.898794,     0.000000,     0.438370,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {   -0.438370,     0.000000,     0.898794,     0.000000, },
        {    0.000000,    -3.500000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.730632,    -0.269791,     0.627208,     0.000000, },
        {    0.295194,     0.953146,     0.066123,     0.000000, },
        {   -0.615660,     0.136837,     0.776040,     0.000000, },
        {    0.000000,    -3.500000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    0.984808,     0.006060,     0.173541,     0.000000, },
        {    0.000000,     0.999391,    -0.034898,     0.000000, },
        {   -0.173647,     0.034367,     0.984208,     0.000000, },
        {    0.000000,    -3.500000,     0.000000,     1.000000, },
    },
  },
  {/* obj #2 */
    {/* pose #0 */
        {    0.921061,    -0.389418,     0.000000,     0.000000, },
        {    0.389418,     0.921061,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {   -0.211069,    -0.606480,    -0.766572,     0.000000, },
        {   -0.225697,     0.793286,    -0.565471,     0.000000, },
        {    0.951058,     0.053660,    -0.304319,     0.000000, },
        {    1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {   -0.538240,    -0.840668,     0.059791,     0.000000, },
        {   -0.575543,     0.314813,    -0.754746,     0.000000, },
        {    0.615668,    -0.440647,    -0.653286,     0.000000, },
        {    1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {   -0.233611,    -0.863962,    -0.446089,     0.000000, },
        {   -0.249802,     0.496712,    -0.831189,     0.000000, },
        {    0.939694,    -0.082741,    -0.331857,     0.000000, },
        {    1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.325239,    -0.939635,    -0.106329,     0.000000, },
        {    0.584779,     0.288217,    -0.758264,     0.000000, },
        {    0.743137,     0.184438,     0.643219,     0.000000, },
        {    1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    0.423912,    -0.870619,     0.249643,     0.000000, },
        {    0.905703,     0.407491,    -0.116845,     0.000000, },
        {    0.000000,     0.275635,     0.961262,     0.000000, },
        {    1.500000,    -3.000000,     0.000000,     1.000000, },
    },
  },
  {/* obj #3 */
    {/* pose #0 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {   -0.089458,     0.211763,     0.973218,     0.000000, },
        {    0.106611,     0.973558,    -0.202038,     0.000000, },
        {   -0.990268,     0.085682,    -0.109669,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {   -0.261442,     0.431097,     0.863599,     0.000000, },
        {    0.311575,     0.884513,    -0.347213,     0.000000, },
        {   -0.913548,     0.178300,    -0.365568,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {   -0.089458,     0.211763,     0.973218,     0.000000, },
        {    0.106611,     0.973558,    -0.202038,     0.000000, },
        {   -0.990268,     0.085682,    -0.109669,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {   -0.987858,    -0.079269,    -0.133617,     0.000000, },
        {   -0.069077,     0.994457,    -0.079271,     0.000000, },
        {    0.139161,    -0.069078,    -0.987858,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {   -0.995135,    -0.035427,    -0.091929,     0.000000, },
        {   -0.069586,     0.913299,     0.401302,     0.000000, },
        {    0.069742,     0.405746,    -0.911321,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
  },
  {/* obj #4 */
    {/* pose #0 */
        {    0.921061,     0.389418,     0.000000,     0.000000, },
        {   -0.389418,     0.921061,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {   -1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    0.888191,    -0.308826,     0.340211,     0.000000, },
        {    0.454149,     0.702468,    -0.547985,     0.000000, },
        {   -0.069755,     0.641222,     0.764178,     0.000000, },
        {   -1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    0.954389,     0.285390,    -0.087710,     0.000000, },
        {   -0.290301,     0.818394,    -0.495940,     0.000000, },
        {   -0.069755,     0.498782,     0.863916,     0.000000, },
        {   -1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    0.876799,     0.207619,     0.433726,     0.000000, },
        {    0.338004,     0.375448,    -0.863013,     0.000000, },
        {   -0.342019,     0.903290,     0.259017,     0.000000, },
        {   -1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.615484,     0.007054,     0.788117,     0.000000, },
        {    0.710072,     0.428972,    -0.558374,     0.000000, },
        {   -0.342019,     0.903290,     0.259017,     0.000000, },
        {   -1.500000,    -3.000000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    0.482833,    -0.067543,     0.873104,     0.000000, },
        {    0.806161,     0.423680,    -0.413037,     0.000000, },
        {   -0.342019,     0.903290,     0.259017,     0.000000, },
        {   -1.500000,    -3.000000,     0.000000,     1.000000, },
    },
  },
  {/* obj #5 */
    {/* pose #0 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.951057,    -0.309017,     0.000000, },
        {    0.000000,     0.309017,     0.951057,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,    -0.241920,    -0.970296,     0.000000, },
        {    0.000000,     0.970296,    -0.241920,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.951057,    -0.309017,     0.000000, },
        {    0.000000,     0.309017,     0.951057,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.866025,     0.187302,     0.463593,     0.000000, },
        {    0.000000,     0.927185,    -0.374604,     0.000000, },
        {   -0.500000,     0.324416,     0.802966,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    0.682437,    -0.423236,     0.595945,     0.000000, },
        {    0.533178,     0.845946,    -0.009776,     0.000000, },
        {   -0.500000,     0.324416,     0.802966,     0.000000, },
        {    0.000000,     2.000000,     0.000000,     1.000000, },
    },
  },
  {/* obj #6 */
    {/* pose #0 */
        {    0.980066,    -0.198669,     0.000000,     0.000000, },
        {    0.198669,     0.980066,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    0.980066,    -0.190973,     0.054761,     0.000000, },
        {    0.198669,     0.942100,    -0.270143,     0.000000, },
        {    0.000000,     0.275638,     0.961262,     0.000000, },
        {    0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    0.797206,    -0.296377,     0.525949,     0.000000, },
        {    0.379550,     0.923542,    -0.054878,     0.000000, },
        {   -0.469471,     0.243374,     0.848744,     0.000000, },
        {    0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    0.980066,    -0.190973,     0.054761,     0.000000, },
        {    0.198669,     0.942100,    -0.270143,     0.000000, },
        {    0.000000,     0.275638,     0.961262,     0.000000, },
        {    0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.837147,    -0.335273,     0.432177,     0.000000, },
        {    0.398566,     0.915029,    -0.062182,     0.000000, },
        {   -0.374606,     0.224306,     0.899643,     0.000000, },
        {    0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    0.902342,    -0.424721,     0.073423,     0.000000, },
        {    0.429606,     0.872443,    -0.232985,     0.000000, },
        {    0.034896,     0.241775,     0.969705,     0.000000, },
        {    0.600000,     0.500000,     0.000000,     1.000000, },
    },
  },
  {/* obj #7 */
    {/* pose #0 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.848048,     0.529919,     0.000000, },
        {    0.000000,    -0.529919,     0.848048,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.848048,     0.529919,     0.000000, },
        {    0.000000,    -0.529919,     0.848048,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.848048,     0.529919,     0.000000, },
        {    0.000000,    -0.529919,     0.848048,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.848048,     0.529919,     0.000000, },
        {    0.000000,    -0.529919,     0.848048,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.848048,     0.529919,     0.000000, },
        {    0.000000,    -0.529919,     0.848048,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
  },
  {/* obj #8 */
    {/* pose #0 */
        {    0.980066,     0.198669,     0.000000,     0.000000, },
        {   -0.198669,     0.980066,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {   -0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    0.980066,     0.190973,    -0.054761,     0.000000, },
        {   -0.198669,     0.942100,    -0.270143,     0.000000, },
        {    0.000000,     0.275638,     0.961262,     0.000000, },
        {   -0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    0.965177,     0.144063,    -0.218355,     0.000000, },
        {   -0.195651,     0.951609,    -0.236981,     0.000000, },
        {    0.173648,     0.271450,     0.946658,     0.000000, },
        {   -0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    0.980066,     0.190973,    -0.054761,     0.000000, },
        {   -0.198669,     0.942100,    -0.270143,     0.000000, },
        {    0.000000,     0.275638,     0.961262,     0.000000, },
        {   -0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    0.908702,     0.089776,    -0.407677,     0.000000, },
        {   -0.184203,     0.962614,    -0.198604,     0.000000, },
        {    0.374606,     0.255567,     0.891266,     0.000000, },
        {   -0.600000,     0.500000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    0.792891,     0.032187,    -0.608513,     0.000000, },
        {   -0.160727,     0.974288,    -0.157892,     0.000000, },
        {    0.587785,     0.222996,     0.777677,     0.000000, },
        {   -0.600000,     0.500000,     0.000000,     1.000000, },
    },
  },
  {/* obj #9 */
    {/* pose #0 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     1.000000,     0.000000,     0.000000, },
        {    0.000000,     0.000000,     1.000000,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #1 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.866025,     0.500000,     0.000000, },
        {    0.000000,    -0.500000,     0.866025,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #2 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.866025,     0.500000,     0.000000, },
        {    0.000000,    -0.500000,     0.866025,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #3 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.866025,     0.500000,     0.000000, },
        {    0.000000,    -0.500000,     0.866025,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #4 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.866025,     0.500000,     0.000000, },
        {    0.000000,    -0.500000,     0.866025,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
    {/* pose #5 */
        {    1.000000,     0.000000,     0.000000,     0.000000, },
        {    0.000000,     0.866025,     0.500000,     0.000000, },
        {    0.000000,    -0.500000,     0.866025,     0.000000, },
        {    0.000000,     2.800000,     0.000000,     1.000000, },
    },
  },
};
