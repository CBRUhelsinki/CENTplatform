/**
 * @file
 * @author  John Doe <jdoe@example.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The time class represents a moment of time.
 */
 
class Time {
 
    public:
 
       /**
	    * @brief short constructor description
        * Constructor that sets the time to a given value.
        * 
		* @todo implement it
		* @warning it may depend on Your system time 
		* @see now()
        * @param timemillis Number of milliseconds
        *        passed since Jan 1, 1970.
        */
       Time (int timemillis);
	   
       /**
        * Get the current time.
        * @code
		* for(;;)
		* {
		*   if(Time::now() == -1)//if the system time fails
		*     break;
		* }
		* @endcode
		* @note this method is fun
		* @bug it does not show the current time
        * @return A time object set to the current time.
        */
       static Time now ();
	   
	   /**
	    * Get the current time
	    * @deprecated replaced with static implementation
		* @see now
		*/
	   int currentTime();
};