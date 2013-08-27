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

/** 
* \file DateTime.h
* \brief Utility for data and time handling
* \author Lars-Åke Vinberg
*/

#ifndef DATETIME_H_
#define DATETIME_H_

#include <maapi.h>
#include <maassert.h>
#include <mastring.h>
#include <matime.h>

#include "TimeSpan.h"

namespace MAPUtil
{
	/**
	 * \brief DateTime is a C++ class for time stamps.
	 *
	 */
	class DateTime
	{
	public:
		/**
		 * Constructs a DateTime with default value.
		 */
		DateTime( );
		/**
		 * Constructs a DateTime from the specified ticks.
		 *
		 * Ticks is the number of seconds since the beginning of January 1, 1970.
		 */
		DateTime( time_t t );
		/**
		 * Constructs a DateTime from the specified tm structure.
		 */
		DateTime( tm* tim );
		/**
		 * Constructs a DateTime from the specified year, month, and day.
		 * Time of day is set to 00:00:00.
		 */
		DateTime( int year, int month, int day );
		/**
		 * Constructs a DateTime from the specified year, month, day, hour, minute, and second.
		 */
		DateTime( int year, int month, int day, int hour, int minute, int second );
		/**
		 * Constructs a DateTime by parsing the specified string.
		 */
		DateTime( const char* dateString );
		/**
		 * Destructs a DateTime.
		 */
		~DateTime( ); // MUST NOT be virtual, to avoid vtable
		/**
		 * Returns the maximum value that can be represented by a DateTime.
		 */
		static DateTime maxValue( );
		/**
		 * Returns the smallest value that can be represented by a DateTime.
		 */
		static DateTime minValue( );
		/**
		 * returns current local time as a DateTime.
		 */
		static DateTime now( );
		/**
		 * returns today's date in local time, as a DateTime.
		 */
		static DateTime today( );
		/**
		 * Returns time of day as a TimeSpan from midnight.
		 */
		TimeSpan timeOfDay( ) const;
		/**
		 * Returns number of ticks since January 1, 1970.
		 * A tick is one second.
		 */
		time_t getTicks( ) const;
		/**
		 * Returns the year component of a DateTime.
		 */
		int getYear( ) const;
		/**
		 * Returns the month component of a DateTime.
		 */
		int getMonth( ) const;
		/**
		 * Returns the weekday component of a DateTime.
		 */
		int getWeekday( ) const;
		/**
		 * Returns the day of month component of a DateTime.
		 */
		int getDay( ) const;
		/**
		 * Returns the hour component of a DateTime.
		 */
		int getHour( ) const;
		/**
		 * Returns the minute component of a DateTime.
		 */
		int getMinute( ) const;
		/**
		 * Returns the second component of a DateTime.
		 */
		int getSecond( ) const;
		/**
		 * Adds a TimeSpan to current value and returns as a new DateTime.
		 */
		DateTime add( TimeSpan timeSpan ) const;
		/**
		 * Adds specified number of seconds to current value and returns as a new DateTime.
		 */
		DateTime addYears( int years ) const;
		/**
		 * Adds specified number of monts to current value and returns as a new DateTime.
		 */
		DateTime addMonths( int months ) const;
		/**
		 * Adds specified number of days to current value and returns as a new Datetime.
		 */
		DateTime addDays( double days ) const;
		/**
		 * Adds specified number of hours to current value and returns as a new DateTime.
		 */
		DateTime addHours( double hours ) const;
		/**
		 * Adds specified number of minutes to current value and returns as a new DateTime.
		 */
		DateTime addMinutes( double minutes ) const;
		/**
		 * Adds specified number of seconds to current value and returns as a new DateTime.
		 */
		DateTime addSeconds( double seconds ) const;
		/**
		 * Adds ticks to current value and returns as a new DateTime.
		 */
		DateTime addTicks( int ticks ) const;
		/**
		 * Returns time span between current value and specified DateTime as a TimeSpan.
		 */
		TimeSpan subtract( DateTime ts ) const;
		/**
		 * Subtracts a timespan from current value and returns as a new DateTime.
		 */
		DateTime subtract( TimeSpan span ) const;
	
	private:
		/**
		 * The internal representation of the time stamp.
		 */
		time_t mTicks;
	};
	/**
	 * Compares two DateTime instances.
	 * Returns true if the first instance is larger than the second.
	 */
	inline bool operator > ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) > b.getTicks( ); 
	}
	/**
	 * Compares two DateTime instances.
	 * Returns true if the first instance is smaller than the second.
	 */
	inline bool operator < ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) < b.getTicks( ); 
	}
	/**
	 * Compares two DateTime instances.
	 * Returns true if the first instance is larger than or equal to the second.
	 */
	inline bool operator >= ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) >= b.getTicks( ); 
	}
	/**
	 * Compares two DateTime instances.
	 * Returns true if the first instance is smaller than or equal to the second.
	 */
	inline bool operator <= ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) <= b.getTicks( ); 
	}
	/**
	 * Compares two DateTime instances.
	 * Returns true if the first instance is equal to the second.
	 */
	inline bool operator == ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) == b.getTicks( ); 
	}
	/**
	 * Compares two DateTime instances.
	 * Returns true if the first instance is not equal to the second.
	 */
	inline bool operator != ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) != b.getTicks( ); 
	}
	/**
	 * Subtracts a DateTime from another DateTime.
	 * Returns the difference as a TimeSpan.
	 */
	inline TimeSpan operator - ( const DateTime d1, const DateTime d2 ) 
	{
		return TimeSpan( (int)d1.getTicks( ) - (int)d2.getTicks( ) ); 
	}
	/**
	 * Subtracts a TimeSpan from a DateTime. 
	 * Returns the resulting DateTime.
	 */
	inline DateTime operator - ( const DateTime d, const TimeSpan t ) 
	{ 
		return DateTime( (int)d.getTicks( ) - (int)t.getTicks( ) ); 
	}
	/**
	 * Adds a TimeSpan to a DateTime.
	 * Returns the resulting DateTime.
	 */
	inline DateTime operator + ( const DateTime d, const TimeSpan t ) 
	{ 
		return DateTime( d.getTicks( ) + t.getTicks( ) ); 
	}
}

#endif // DATETIME_H_
