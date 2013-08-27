/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Trig.h"
#include <maapi.h>
#include <madmath.h>

     const double sq2p1 = 2.414213562373095048802e0;
     const double sq2m1  = .414213562373095048802e0;
     const double p4  = .161536412982230228262e2;
     const double p3  = .26842548195503973794141e3;
     const double p2  = .11530293515404850115428136e4;
     const double p1  = .178040631643319697105464587e4;
     const double p0  = .89678597403663861959987488e3;
     const double q4  = .5895697050844462222791e2;
     const double q3  = .536265374031215315104235e3;
     const double q2  = .16667838148816337184521798e4;
     const double q1  = .207933497444540981287275926e4;
     const double q0  = .89678597403663861962481162e3;
     const double PIO2 = 1.5707963267948966135E0;
     const double nan = (0.0/0.0);

    // reduce
     double mxatan(double arg)
    {
        double argsq, value;

        argsq = arg*arg;
        value = ((((p4*argsq + p3)*argsq + p2)*argsq + p1)*argsq + p0);
        value = value/(((((argsq + q4)*argsq + q3)*argsq + q2)*argsq + q1)*argsq + q0);
        return value*arg;
    }

    // reduce
      double msatan(double arg)
    {
        if(arg < sq2m1)
            return mxatan(arg);
        if(arg > sq2p1)
            return PIO2 - mxatan(1/arg);
            return PIO2/2 + mxatan((arg-1)/(arg+1));
    }

    // implementation of atan
      double atan(double arg)
    {
        if(arg > 0)
            return msatan(arg);
        return -msatan(-arg);
    }

    // implementation of atan2
      double atan2(double arg1, double arg2)
    {
        if(arg1+arg2 == arg1)
        {
            if(arg1 >= 0)
            return PIO2;
                return -PIO2;
        }
        arg1 = atan(arg1/arg2);
        if(arg2 < 0)
       {
            if(arg1 <= 0)
                return arg1 + M_PI;
            return arg1 - M_PI;
        }
        return arg1;
    
    }

    // implementation of asin
     double asin(double arg)
    {
        double temp;
        int sign;

        sign = 0;
        if(arg < 0)
        {
            arg = -arg;
            sign++;
        }
        if(arg > 1)
            return nan;
        temp = sqrt(1 - arg*arg);
        if(arg > 0.7)
            temp = PIO2 - atan(temp/arg);
        else
            temp = atan(arg/temp);
        if(sign > 0)
            temp = -temp;
        return temp;
    }

    // implementation of acos
      double acos(double arg)
    {
        if(arg > 1 || arg < -1)
            return nan;
        return PIO2 - asin(arg);
    }

 