/*
 * vector_dist_key.hpp
 *
 *  Created on: Mar 10, 2015
 *      Author: i-bird
 */

#ifndef VECTOR_DIST_KEY_HPP_
#define VECTOR_DIST_KEY_HPP_



/*! \brief Grid key for a distributed grid
 *
 * Grid key for a distributed grid
 *
 */

class vect_dist_key_dx
{
	//! Local grid iterator

	size_t key;

public:

	/*! \brief Get the key
	 *
	 * \return the local key
	 *
	 */
	size_t getKey()
	{
		return key;
	}

	/*! \brief Convert the key into a message
	 *
	 */
	std::string to_string()
	{
		std::stringstream ts;

		ts << "x[0]=" << key;

		return ts.str();
	}

	vect_dist_key_dx(size_t key)
	:key(key)
	{
	}
};



#endif /* VECTOR_DIST_KEY_HPP_ */
