/*
 * DataSummary.hpp
 *
 *  Created on: Jul 31, 2009
 *      Author: stewie
 */

#ifndef DATASUMMARY_HPP_
#define DATASUMMARY_HPP_

namespace bn {

namespace util {

/**
 * This class represents aggregate informations about a statistic population.
 *
 * @see DerridaRunner for a practical use.
 */
template<class T> struct DataSummary {
	/**
	 * Data label.
	 */
	T label;
	/**
	 * Minimum of the population.
	 */
	double min;
	/**
	 * Population mean.
	 */
	double mean;
	/**
	 * Standard deviation of the population.
	 */
	double stddev;
	/**
	 * Maximum of the population.
	 */
	double max;

	/**
	 * Initializes the fields of a DataSummary.
	 * @param min minimum
	 * @param mean average
	 * @param stddev standard deviation
	 * @param max maximum
	 */
	DataSummary(const T& label, const double min, const double mean,
			const double stddev, const double max) :
		label(label), min(min), mean(mean), stddev(stddev), max(max) {
	}
};

}

}

#endif /* DATASUMMARY_HPP_ */
