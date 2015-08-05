/*
 * grid_dist_id_iterator_sub.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: Pietro Incardona
 */

#ifndef GRID_DIST_ID_ITERATOR_HPP_
#define GRID_DIST_ID_ITERATOR_HPP_

/*! \brief This structure store the Box that define the domain inside the Ghost + domain box
 *
	\verbatim

                          (Ghost + Domain)
     +------------------+
     |                  |
     |  +------------+ <---------- (Domain)
     |  |            |  |
     |  |  Domain    |  |
     |  |  Box       |  |
     |  |            |  |
     |  |            |  |
     |  +------------+  |
     |                  |
     +------------------+
(0,0) local coordinate ---> ( x, y )

	\endverbatim

 *
 *  * Domain
 *
 * \tparam dim dimensionality
 *
 */
template<unsigned int dim>
struct GBoxes
{
	//! Ghost + Domain ghost
	Box<dim,long int> GDbox;
	//! Domain box
	Box<dim,long int> Dbox;
	//! origin of GDbox in global coordinates
	Point<dim,long int> origin;
};

#define FREE 1
#define FIXED 2

#include "grid_dist_key.hpp"
#include "VCluster.hpp"



/*! \brief Distributed grid iterator
 *
 * Iterator across the local elements of the distributed grid
 *
 * \tparam dim dimensionality of the grid
 * \tparam device_grid type of basic grid
 * \tparam impl implementation
 *
 */
template<unsigned int dim, typename device_grid, int impl >
class grid_dist_iterator
{

};


/*! \brief Distributed grid iterator
 *
 * Iterator across the local elements of the distributed grid
 *
 * \tparam dim dimensionality of the grid
 * \tparam device_grid type of basic grid
 * \tparam impl implementation
 *
 */
template<unsigned int dim, typename device_grid>
class grid_dist_iterator<dim,device_grid,FREE>
{
	//! grid list counter
	size_t g_c;

	//! List of the grids we are going to iterate
	Vcluster_object_array<device_grid> & gList;

	//! Extension of each grid: domain and ghost + domain
	const openfpm::vector<GBoxes<device_grid::dims>> & gdb_ext;

	//! Actual iterator
	grid_key_dx_iterator_sub<dim> a_it;

	//! margin of the grid iterator
	size_t m;

	/*! \brief from g_c increment g_c until you find a valid grid
	 *
	 */
	void selectValidGrid()
	{
		// When the grid has size 0 potentially all the other informations are garbage
		while (g_c < gList.size() && (gList[g_c].size() == 0 || gdb_ext.get(g_c).Dbox.isValid() == false ) ) g_c++;

		// get the next grid iterator
		if (g_c < gList.size())
		{
			a_it.reinitialize(gList[g_c].getIterator(gdb_ext.get(g_c).Dbox.getKP1(),gdb_ext.get(g_c).Dbox.getKP2()));
		}
	}

	public:

	/*! \brief Constructor of the distributed grid
	 *
	 * \param gk std::vector of the local grid
	 *
	 */
	grid_dist_iterator(Vcluster_object_array<device_grid> & gk, const openfpm::vector<GBoxes<device_grid::dims>> & gdb_ext)
	:g_c(0),gList(gk),gdb_ext(gdb_ext),m(0)
	{
		// Initialize the current iterator
		// with the first grid
		selectValidGrid();
	}

	// Destructor
	~grid_dist_iterator()
	{
	}

	/*! \brief Get the next element
	 *
	 * \return the next grid_key
	 *
	 */

	inline grid_dist_iterator<dim,device_grid,FREE> operator++()
	{
		++a_it;

		// check if a_it is at the end

		if (a_it.isNext() == true)
			return *this;
		else
		{
			// switch to the new grid
			g_c++;

			selectValidGrid();
		}

		return *this;
	}

	/*! \brief Check if there is the next element
	 *
	 * \return true if there is the next, false otherwise
	 *
	 */
	inline bool isNext()
	{
		// If there are no other grid stop

		if (g_c >= gList.size())
			return false;

		return true;
	}

	/*! \brief Get the actual key
	 *
	 * \return the actual key
	 *
	 */
	inline grid_dist_key_dx<dim> get()
	{
		return grid_dist_key_dx<dim>(g_c,a_it.get());
	}
};


/*! \brief Distributed grid iterator
 *
 * Iterator across the local elements of the distributed grid
 *
 * \tparam dim dimensionality of the grid
 * \tparam device_grid type of basic grid
 * \tparam impl implementation
 *
 */
template<unsigned int dim, typename device_grid>
class grid_dist_iterator<dim,device_grid,FIXED>
{
	//! grid list counter
	size_t g_c;

	//! List of the grids we are going to iterate
	Vcluster_object_array<device_grid> & gList;

	//! Extension of each grid: domain and ghost + domain
	const openfpm::vector<GBoxes<device_grid::dims>> & gdb_ext;

	//! Actual iterator
	grid_key_dx_iterator<dim> a_it;

	/*! \brief from g_c increment g_c until you find a valid grid
	 *
	 */
	void selectValidGrid()
	{
		// When the grid has size 0 potentially all the other informations are garbage
		while (g_c < gList.size() && (gList[g_c].size() == 0 || gdb_ext.get(g_c).Dbox.isValid() == false ) ) g_c++;

		// get the next grid iterator
		if (g_c < gList.size())
		{
			a_it.reinitialize(gList[g_c].getIterator(gdb_ext.get(g_c).Dbox.getKP1(),gdb_ext.get(g_c).Dbox.getKP2()));
		}
	}

	public:

	/*! \brief Constructor of the distributed grid
	 *
	 * \param gk std::vector of the local grid
	 *
	 */
	grid_dist_iterator(Vcluster_object_array<device_grid> & gk, const openfpm::vector<GBoxes<device_grid::dims>> & gdb_ext)
	:g_c(0),gList(gk),gdb_ext(gdb_ext)
	{
		// Initialize the current iterator
		// with the first grid
		selectValidGrid();
	}

	// Destructor
	~grid_dist_iterator()
	{
	}

	/*! \brief Get the next element
	 *
	 * \return the next grid_key
	 *
	 */

	grid_dist_iterator<dim,device_grid,FIXED> operator++()
	{
		++a_it;

		// check if a_it is at the end

		if (a_it.isNext() == true)
			return *this;
		else
		{
			// switch to the new grid
			g_c++;
			selectValidGrid();
		}

		return *this;
	}

	/*! \brief Check if there is the next element
	 *
	 * \return true if there is the next, false otherwise
	 *
	 */
	bool isNext()
	{
		// If there are no other grid stop

		if (g_c >= gList.size())
			return false;

		return true;
	}

	/*! \brief Get the actual key
	 *
	 * \return the actual key
	 *
	 */
	grid_dist_key_dx<dim> get()
	{
		return grid_dist_key_dx<dim>(g_c,a_it.get());
	}
};

#endif /* GRID_DIST_ID_ITERATOR_SUB_HPP_ */
